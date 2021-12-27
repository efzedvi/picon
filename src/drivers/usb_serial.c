/**
 * @author      : faraz
 * @file        : usb_serial.c
 * @created     : Sun 26 Dec 2021 06:15:16 PM EST
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "rtos.h"
#include "task.h"
#include "queue.h"

#include "picon/utils.h"
#include "picon/ioctl.h"
#include "picon/io.h"
#include "picon/usb_serial.h"

#include "tusb.h"


#define PICON_USB_SERIAL_PRIORITY	(9)

#define USB_RX_QUEUE_SIZE		CFG_TUD_CDC_RX_BUFSIZE
#define USB_TX_QUEUE_SIZE		CFG_TUD_CDC_TX_BUFSIZE

#define MIN_WAIT_TIME			(100)


static rtos_queue_handle_t usb_txq;		// USB transmit queue
static rtos_queue_handle_t usb_rxq;		// USB receive queue
static int usbs_flags = 0;

#define USB_DELAY_TICKS		(25)

// USB Driver task:
static void usb_serial_task(void *arg)
{
	int 	rc;
	char	ch;


	for (;;) {
		tud_task();

		if (tud_cdc_connected() && tud_cdc_available()) {

			rc = (int) tud_cdc_read(&ch, (uint32_t) 1);
			if (rc == 1) {
				if (rtos_queue_spaces_available(usb_rxq) > 0) {
					rtos_queue_send(usb_rxq, &ch, 0);
				} 
			}

			if (rtos_queue_messages_waiting(usb_txq)) {
				if (rtos_queue_receive(usb_txq, &ch, RTOS_PORT_MAX_DELAY) != RTOS_PASS) {
					int avail = (int) tud_cdc_write_available();
					if (avail >= 1) {
						tud_cdc_write(&ch, 1);
					} else {
						tud_cdc_write_flush();
					}
				}
			}
		}

		rtos_task_delay(USB_DELAY_TICKS);
	}
}


int picon_usb_serial_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	usb_txq = rtos_queue_create(USB_TX_QUEUE_SIZE, sizeof(char));
	usb_rxq = rtos_queue_create(USB_RX_QUEUE_SIZE, sizeof(char));

	tusb_init();

	rtos_task_create(usb_serial_task, "USBCDC", 500, NULL, PICON_USB_SERIAL_PRIORITY, NULL);
	//TODO: check the return value of task_create

	return 0;
}

int picon_usb_serial_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned int n;
	rtos_tick_type_t	wait_time = RTOS_PORT_MAX_DELAY;

	UNUSED(devf);

	if (!buf) return -1;

	for(n=0; n<count; n++) {
		if ( rtos_queue_receive(usb_rxq, buf++, wait_time) != RTOS_PASS)
			break;
	}

	return n;
}

int picon_usb_serial_write(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned int n;

	UNUSED(devf);

	if (!buf) return -1;

	for(n=0; n<count; n++) {
		if (rtos_queue_send(usb_txq, buf, USB_DELAY_TICKS) != RTOS_PASS) break;

		++buf;
	}

	return n;
}

const void *picon_usb_serial_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);
	UNUSED(flags);

	usbs_flags = flags;

	return (const void *) devf;
}

int picon_usb_serial_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	usbs_flags = 0;

	return 0;
}

int picon_usb_serial_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	int rc=0;

	UNUSED(devf);

#ifdef USB_TTY_CONTROL_C
        switch (request) {
		case PICON_IOC_TTY_SET_INT:
			task_sem = (rtos_semaphore_handle_t) data;
			break;

		case PICON_IOC_TTY_GET_INT:
			if (!data) rc = -EINVAL;
			*((rtos_semaphore_handle_t *) data) = task_sem;
			break;
		default:
			rc = -EINVAL;
			break;
	}
#else
	UNUSED(request);
	UNUSED(data);

	rc = -1;
#endif

	return rc;
}


