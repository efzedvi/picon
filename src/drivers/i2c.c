/**
 * @author      : faraz (faraz)
 * @file        : i2c.c
 * @created     : Wed 05 Jan 2022 06:18:02 PM EST
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <FreeRTOS.h>
#include "semphr.h"

#include "rtos.h"
#include "picon/utils.h"
#include "picon/ioctl.h"
#include "picon/i2c.h"

#include "pico.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/resets.h"
#include "hardware/clocks.h"
#include "pico/timeout_helper.h"

#define PICON_I2C_MAX		(2)

// MASTER time out in ticks (ms)
#define PICON_I2C_TIMEOUT	(100)

// PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6))
// PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7))

typedef struct _picon_i2c_def {
	i2c_inst_t		*dev;
	i2c_cfg_t		cfg;
} i2c_t;


i2c_t i2c_devs[PICON_I2C_MAX] = {
	{
	.dev=i2c0,
	.cfg = {
		.speed = PICON_I2C_STANDARD_100K,
		.scl=4,
		.sda=5,
		}
	},
	{
	.dev=i2c1,
	.cfg = {
		.speed = PICON_I2C_FAST_400K,
		.scl=-1,
		.sda=-1,
		}
	},
};


// some helper functions

// this is basically a copy of the static i2c_write_blocking_internal function.
// copied here so that instead of blocking in the spinlock by calling
// tight_loop_contents we could yield

static int picon_i2c_write(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop)
{
	rtos_tick_type_t	t0;
	invalid_params_if(I2C, addr >= 0x80); // 7-bit addresses
	invalid_params_if(I2C, I2C_RESERVED_ADDR(addr));
	// Synopsys hw accepts start/stop flags alongside data items in the same
	// FIFO word, so no 0 byte transfers.
	invalid_params_if(I2C, len == 0);
	invalid_params_if(I2C, ((int)len) < 0);

	i2c->hw->enable = 0;
	i2c->hw->tar = addr;
	i2c->hw->enable = 1;

	bool abort = false;
	bool timeout = false;

	uint32_t abort_reason = 0;
	int byte_ctr;

	int ilen = (int)len;
	for (byte_ctr = 0; byte_ctr < ilen; ++byte_ctr) {
		bool first = byte_ctr == 0;
		bool last = byte_ctr == ilen - 1;

		i2c->hw->data_cmd =
				bool_to_bit(first && i2c->restart_on_next) << I2C_IC_DATA_CMD_RESTART_LSB |
				bool_to_bit(last && !nostop) << I2C_IC_DATA_CMD_STOP_LSB |
				*src++;

		// Wait until the transmission of the address/data from the internal
		// shift register has completed. For this to function correctly, the
		// TX_EMPTY_CTRL flag in IC_CON must be set. The TX_EMPTY_CTRL flag
		// was set in i2c_init.

		t0 = rtos_task_get_tick_count();

		do {
			if ( diff_ticks(t0, rtos_task_get_tick_count()) > PICON_I2C_TIMEOUT) {
				timeout = true;
				abort = true;
			} else {
				rtos_task_yield();
			}

		} while (!timeout && !(i2c->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_EMPTY_BITS));

		// If there was a timeout, don't attempt to do anything else.
		if (!timeout) {
			abort_reason = i2c->hw->tx_abrt_source;
			if (abort_reason) {
				// Note clearing the abort flag also clears the reason, and
				// this instance of flag is clear-on-read! Note also the
				// IC_CLR_TX_ABRT register always reads as 0.
				i2c->hw->clr_tx_abrt;
				abort = true;
			}

			if (abort || (last && !nostop)) {
				// If the transaction was aborted or if it completed
				// successfully wait until the STOP condition has occured.

				// TODO Could there be an abort while waiting for the STOP
				// condition here? If so, additional code would be needed here
				// to take care of the abort.

				t0 = rtos_task_get_tick_count();

				do {
					if ( diff_ticks(t0, rtos_task_get_tick_count()) > PICON_I2C_TIMEOUT) {
						timeout = true;
						abort = true;
					} else {
						rtos_task_yield();
					}

				} while (!timeout && !(i2c->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_STOP_DET_BITS));

				// If there was a timeout, don't attempt to do anything else.
				if (!timeout) {
					i2c->hw->clr_stop_det;
				}
			}
		}

		// Note the hardware issues a STOP automatically on an abort condition.
		// Note also the hardware clears RX FIFO as well as TX on abort,
		// because we set hwparam IC_AVOID_RX_FIFO_FLUSH_ON_TX_ABRT to 0.
		if (abort) break;
	}

	int rval;

	// A lot of things could have just happened due to the ingenious and
	// creative design of I2C. Try to figure things out.
	if (abort) {
		if (timeout)
			rval = PICO_ERROR_TIMEOUT;
		else if (!abort_reason || abort_reason & I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_BITS) {
			// No reported errors - seems to happen if there is nothing connected to the bus.
			// Address byte not acknowledged
			rval = PICO_ERROR_GENERIC;
		} else if (abort_reason & I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_BITS) {
			// Address acknowledged, some data not acknowledged
			rval = byte_ctr;
		} else {
			//panic("Unknown abort from I2C instance @%08x: %08x\n", (uint32_t) i2c->hw, abort_reason);
			rval = PICO_ERROR_GENERIC;
		}
	} else {
		rval = byte_ctr;
	}

	// nostop means we are now at the end of a *message* but not the end of a *transfer*
	i2c->restart_on_next = nostop;
	return rval;
}

// this is basically a copy of the static i2c_read_blocking_internal function.
// copied here so that instead of blocking in the spinlock by calling
// tight_loop_contents we could yield
// reading always sends a stop, so no RESTART

static int picon_i2c_read(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len)
{
	rtos_tick_type_t	t0;

	invalid_params_if(I2C, addr >= 0x80); // 7-bit addresses
	invalid_params_if(I2C, I2C_RESERVED_ADDR(addr));
	invalid_params_if(I2C, len == 0);
	invalid_params_if(I2C, ((int)len) < 0);

	i2c->hw->enable = 0;
	i2c->hw->tar = addr;
	i2c->hw->enable = 1;

	bool abort = false;
	bool timeout = false;
	uint32_t abort_reason;
	int byte_ctr;
	int ilen = (int)len;

	for (byte_ctr = 0; byte_ctr < ilen; ++byte_ctr) {
		bool first = byte_ctr == 0;
		bool last = byte_ctr == ilen - 1;

		while (!i2c_get_write_available(i2c))
			rtos_task_yield();

		i2c->hw->data_cmd =
			bool_to_bit(first && i2c->restart_on_next) << I2C_IC_DATA_CMD_RESTART_LSB |
			bool_to_bit(last) << I2C_IC_DATA_CMD_STOP_LSB |
			I2C_IC_DATA_CMD_CMD_BITS; // -> 1 for read

		t0 = rtos_task_get_tick_count();

		do {
			abort_reason = i2c->hw->tx_abrt_source;
			abort 	     = (bool) i2c->hw->clr_tx_abrt;

			if ( diff_ticks(t0, rtos_task_get_tick_count()) > PICON_I2C_TIMEOUT) {
				timeout = true;
				abort = true;
			} else {
				rtos_task_yield();
			}

		} while (!abort && !i2c_get_read_available(i2c));

		if (abort) break;

		*dst++ = (uint8_t) i2c->hw->data_cmd;
	}

	int rval;

	if (abort) {
		if (timeout) {
			rval = PICO_ERROR_TIMEOUT;
		} else if (!abort_reason || abort_reason & I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_BITS) {
			// No reported errors - seems to happen if there is nothing connected to the bus.
			// Address byte not acknowledged
			rval = PICO_ERROR_GENERIC;
		} else {
			rval = PICO_ERROR_GENERIC;
		}

	} else {
		rval = byte_ctr;
	}

	i2c->restart_on_next = false;
	return rval;
}

// The actual exproted functions start here:

int picon_i2c_init(uint8_t ux, void *params)
{
	i2c_t		*i2cp = i2c_devs+ux;		
	i2c_inst_t  	*i2c;
	i2c_cfg_t	*cfg;

	if (ux >= PICON_I2C_MAX || i2cp) return -EINVAL;

	i2c = i2c_devs[ux].dev;

	if (params) {
		cfg = (i2c_cfg_t *) params;
		i2cp->cfg = *cfg;
	}

	if (!IS_PIN_VALID(i2cp->cfg.scl) && !IS_PIN_VALID(i2cp->cfg.sda))
		return -EINVAL;

	if (i2cp->cfg.speed != PICON_I2C_STANDARD_100K &&
	    i2cp->cfg.speed != PICON_I2C_FAST_400K) {

		i2cp->cfg.speed = PICON_I2C_SPEED_SM_100K;
	}

	i2c_init(i2c, i2cp->cfg.speed);
	i2c_set_slave_mode(i2c, false, 0);
	gpio_set_function(i2cp->cfg.sda, GPIO_FUNC_I2C);
	gpio_set_function(i2cp->cfg.scl, GPIO_FUNC_I2C);
	gpio_pull_up(i2cp->cfg.sda);
	gpio_pull_up(i2cp->cfg.scl);

	return 0;
}

const void *picon_i2c_open(const DEVICE_FILE *devf, int flags)
{
	uint8_t		idx;
	i2c_t		*i2cp;

	UNUSED(flags);

	if (!devf) return NULL;

	idx = devf->minor;
	i2cp = i2c_devs + idx;

	if (idx >= PICON_I2C_MAX || !i2cp)
		return NULL;

	if ( picon_i2c_init(idx, NULL) != 0)
		return NULL;

	return (const void *) devf;
}

int picon_i2c_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	return 0;
}

/*
// I2C flags
#define PICON_I2C_M_RD		(1 << 0)
#define PICON_I2C_M_STOP		(1 << 1)

typedef struct _picon_i2c_msg {
	uint16_t	flags;
	uint16_t	len;
	uint8_t		*buf;
} PICON_I2C_MSG;

typedef struct _picon_ioctl_i2c {
	uint8_t		addr;
	PICON_I2C_MSG	*msgs;
	uint8_t	nmsgs;
} PICON_IOCTL_I2C;

*/

int picon_i2c_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	uint8_t		idx;
	i2c_t		*i2cp;
	i2c_inst_t	*i2c;
	PICON_IOCTL_I2C	*xfer = (PICON_IOCTL_I2C *) data;
	PICON_I2C_MSG	*msg;
	uint16_t	i;
	bool		restart=false;
	int		rc;

	if (!devf) return -EINVAL;

	idx = devf->minor;
	i2cp = i2c_devs + idx;

	if (idx >= PICON_I2C_MAX || !i2cp)
		return -EINVAL;

	i2c = i2cp->dev;

	if (request != PICON_IOC_I2C_XFER || !xfer || !xfer->nmsgs) return -EINVAL;

	for(i=0; i < xfer->nmsgs; i++) {
		msg = xfer->msgs + i;

		if (!msg || !msg->buf) return -EINVAL;
		if (I2C_RESERVED_ADDR(xfer->addr)) return -EADDRNOTAVAIL;

		if (msg->flags && PICON_I2C_M_RD) {
			// reading always sends a stop, so no RESTART
			rc = picon_i2c_read(i2c, xfer->addr, msg->buf, msg->len);
			restart = false;
		} else {
			// send RESTART if it's not the last msg
			restart = (i < xfer->nmsgs - 1) ? true : false ;

			rc = picon_i2c_write(i2c, xfer->addr, msg->buf, msg->len, restart);
		}

		if (rc != PICO_OK) {
			switch (rc) {
				case PICO_ERROR_TIMEOUT:
					return -ETIMEDOUT;
				case PICO_ERROR_GENERIC:
					return -EIO;
				case PICO_ERROR_NO_DATA:
					return -ENODATA;
			}
		}
	}

	return 0;
}

