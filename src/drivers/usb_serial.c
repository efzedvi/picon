/**
 * @author      : faraz
 * @file        : usb_serial.c
 * @created     : Sun 26 Dec 2021 06:15:16 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#include "picon/log.h"

#include "tusb.h"
#include "pico/unique_id.h"

#define PICON_USB_SERIAL_PRIORITY	(9)

#define USB_RX_QUEUE_SIZE		CFG_TUD_CDC_RX_BUFSIZE
#define USB_TX_QUEUE_SIZE		CFG_TUD_CDC_TX_BUFSIZE
#define PICON_USB_BUF_SIZE		(128)

#define MIN_WAIT_TIME			(100)

// we use stream buffers instead of queues and it's safe because
// access to the device is already serialized by a mutex, otherwise
// queue would have made more sense to handle priority based access
// by using stream buffers we turn this into a first come first serve
// regardless of task priorities

static rtos_stream_buffer_handle_t usb_txq;		// USB transmit queue
static rtos_stream_buffer_handle_t usb_rxq;		// USB receive queue
static int usbs_flags = 0;

#define USB_DELAY_TICKS		(5)


// ---- descriptors

#define USBD_VID (0x2E8A) // Raspberry Pi
#define USBD_PID (0x000a) // Raspberry Pi Pico SDK CDC

#define TUD_RPI_RESET_DESC_LEN  9
#if !PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE
#define USBD_DESC_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)
#else
#define USBD_DESC_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_RPI_RESET_DESC_LEN)
#endif
#define USBD_MAX_POWER_MA (250)

#define USBD_ITF_CDC       (0) // needs 2 interfaces
#if !PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE
#define USBD_ITF_MAX       (2)
#else
#define USBD_ITF_RPI_RESET (2)
#define USBD_ITF_MAX       (3)
#endif

#define USBD_CDC_EP_CMD (0x81)
#define USBD_CDC_EP_OUT (0x02)
#define USBD_CDC_EP_IN (0x82)
#define USBD_CDC_CMD_MAX_SIZE (8)
#define USBD_CDC_IN_OUT_MAX_SIZE (64)

#define USBD_STR_0 (0x00)
#define USBD_STR_MANUF (0x01)
#define USBD_STR_PRODUCT (0x02)
#define USBD_STR_SERIAL (0x03)
#define USBD_STR_CDC (0x04)
#define USBD_STR_RPI_RESET (0x05)

// Note: descriptors returned from callbacks must exist long enough for transfer to complete

static const tusb_desc_device_t usbd_desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USBD_VID,
    .idProduct = USBD_PID,
    .bcdDevice = 0x0100,
    .iManufacturer = USBD_STR_MANUF,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = USBD_STR_SERIAL,
    .bNumConfigurations = 1,
};

#define TUD_RPI_RESET_DESCRIPTOR(_itfnum, _stridx) \
  /* Interface */\
  9, TUSB_DESC_INTERFACE, _itfnum, 0, 0, TUSB_CLASS_VENDOR_SPECIFIC, RESET_INTERFACE_SUBCLASS, RESET_INTERFACE_PROTOCOL, _stridx,

static const uint8_t usbd_desc_cfg[USBD_DESC_LEN] = {
    TUD_CONFIG_DESCRIPTOR(1, USBD_ITF_MAX, USBD_STR_0, USBD_DESC_LEN,
        0, USBD_MAX_POWER_MA),

    TUD_CDC_DESCRIPTOR(USBD_ITF_CDC, USBD_STR_CDC, USBD_CDC_EP_CMD,
        USBD_CDC_CMD_MAX_SIZE, USBD_CDC_EP_OUT, USBD_CDC_EP_IN, USBD_CDC_IN_OUT_MAX_SIZE),

#if PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE
    TUD_RPI_RESET_DESCRIPTOR(USBD_ITF_RPI_RESET, USBD_STR_RPI_RESET)
#endif
};

static char usbd_serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];

static const char *const usbd_desc_str[] = {
    [USBD_STR_MANUF] = "Raspberry Pi",
    [USBD_STR_PRODUCT] = "Pico",
    [USBD_STR_SERIAL] = usbd_serial_str,
    [USBD_STR_CDC] = "Board CDC",
#if PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE
    [USBD_STR_RPI_RESET] = "Reset",
#endif
};

const uint8_t *tud_descriptor_device_cb(void) {
    return (const uint8_t *)&usbd_desc_device;
}

const uint8_t *tud_descriptor_configuration_cb(__unused uint8_t index) {
    return usbd_desc_cfg;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, __unused uint16_t langid) {
    #define DESC_STR_MAX (20)
    static uint16_t desc_str[DESC_STR_MAX];

    // Assign the SN using the unique flash id
    if (!usbd_serial_str[0]) {
        pico_get_unique_board_id_string(usbd_serial_str, sizeof(usbd_serial_str));
    }

    uint8_t len;
    if (index == 0) {
        desc_str[1] = 0x0409; // supported language is English
        len = 1;
    } else {
        if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0])) {
            return NULL;
        }
        const char *str = usbd_desc_str[index];
        for (len = 0; len < DESC_STR_MAX - 1 && str[len]; ++len) {
            desc_str[1 + len] = str[len];
        }
    }

    // first byte is length (including header), second byte is string type
    desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * len + 2));

    return desc_str;
}


// USB Driver task:
static void usb_serial_task(void *arg)
{
	int 		rc, n=0;
	char		ch;
	char            txbuf[PICON_USB_BUF_SIZE];
	uint16_t        txlen = 0;

	for (;;) {
		tud_task();

		if (tud_cdc_connected()) {

			if (tud_cdc_available()) {
				rc = (int) tud_cdc_read(&ch, (uint32_t) 1);
				if (rc == 1) {
					if (rtos_stream_buffer_spaces_available(usb_rxq) > 0) {
						rtos_stream_buffer_send(usb_rxq, &ch, 1, 0);
					}
				}
			}

			tud_task();

			// slurp the tx queue data into a buffer
			txlen = MIN(rtos_stream_buffer_bytes_available(usb_txq), tud_cdc_write_available());
			txlen = MIN(txlen, sizeof(txbuf));

			if (txlen > 0) {
				for(n=0; n<txlen; n++) {
					if (rtos_stream_buffer_receive(usb_txq, txbuf+n, 1, 0) != 1)
						break;
				}

				if (n > 0)
					tud_cdc_write(txbuf, n);
			}
			tud_cdc_write_flush();
		}

		rtos_task_delay(USB_DELAY_TICKS);
	}
}


int picon_usb_serial_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	usb_txq = rtos_stream_buffer_create(USB_TX_QUEUE_SIZE, 1);
	usb_rxq = rtos_stream_buffer_create(USB_RX_QUEUE_SIZE, 1);

	tusb_init();

	rtos_task_create(usb_serial_task, "USBCDC", 600, NULL, PICON_USB_SERIAL_PRIORITY, NULL);
	//TODO: check the return value of task_create

	return 0;
}

int picon_usb_serial_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned int n;
	rtos_tick_type_t	wait_time = RTOS_PORT_MAX_DELAY;

	UNUSED(devf);

	if (!buf) return -1;

	if (usbs_flags & PICON_IO_NONBLOCK)
		wait_time = MIN_WAIT_TIME;

	for(n=0; n<count; n++) {
		if ( rtos_stream_buffer_receive(usb_rxq, buf+n, 1, wait_time) != 1)
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
		if (rtos_stream_buffer_send(usb_txq, buf+n, 1, USB_DELAY_TICKS) != 1)
			break;
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

//---------------------------------------------------------------


