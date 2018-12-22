/* Copyright (c) 2018 Vasily Voropaev <vvg@cubitel.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbd_conf.h"
#include "usb_regs.h"

/* Vendor and device ID */
#define USBD_VID                        0x1209
#define USBD_PID                        0x6740

#define USBD_LANGID_STRING              0x409
#define USBD_MANUFACTURER_STRING        "NPK Cubitel"
#define USBD_PRODUCT_STRING             "Car audio and control"
#define USBD_CONFIGURATION_STRING       "Config"
#define USBD_INTERFACE_STRING           "Interface"

USBD_DEVICE USR_desc =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor, 
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SerialStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] __ALIGN_END =
{
	0x12,                       /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
    0x00,                       /* bcdUSB */
    0x02,
    0xEF,                       /* bDeviceClass */
    0x02,                       /* bDeviceSubClass */
    0x01,                       /* bDeviceProtocol */
    USB_OTG_MAX_EP0_SIZE,       /* bMaxPacketSize */
    LOBYTE(USBD_VID),           /* idVendor */
    HIBYTE(USBD_VID),           /* idVendor */
    LOBYTE(USBD_PID),           /* idVendor */
    HIBYTE(USBD_PID),           /* idVendor */
    0x00,                       /* bcdDevice rel. 2.00 */
    0x02,
    USBD_IDX_MFC_STR,           /* Index of manufacturer  string */
    USBD_IDX_PRODUCT_STR,       /* Index of product string */
    USBD_IDX_SERIAL_STR,        /* Index of serial number string */
    USBD_CFG_MAX_NUM            /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __ALIGN_END =
{
     USB_SIZ_STRING_LANGID,
     USB_DESC_TYPE_STRING,
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING),
};

uint8_t *USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length)
{
	*length = sizeof(USBD_DeviceDesc);
	return USBD_DeviceDesc;
}

uint8_t *USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length)
{
	*length =  sizeof(USBD_LangIDDesc);
	return USBD_LangIDDesc;
}

uint8_t *USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length)
{
	USBD_GetString (USBD_PRODUCT_STRING, USBD_StrDesc, length);
	return USBD_StrDesc;
}

uint8_t *USBD_USR_ManufacturerStrDescriptor( uint8_t speed , uint16_t *length)
{
	USBD_GetString (USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
	return USBD_StrDesc;
}

uint8_t *USBD_USR_SerialStrDescriptor( uint8_t speed , uint16_t *length)
{
	uint32_t *uid = (uint32_t *)0x1FFF7A10;
	int i;
	uint32_t b;
	char buf[13];

	buf[0] = '0';
	buf[1] = '0';
	buf[2] = '0';
	buf[3] = '2';

	b = *uid;
	for (i = 4; i < 12; i++) {
		buf[i] = ((b & 0xF) < 10) ? (0x30 | (b & 0xF)) : (55 + (b & 0xF));
		b = b >> 4;
	}
	buf[12] = 0;

    USBD_GetString (buf, USBD_StrDesc, length);
    return USBD_StrDesc;
}

uint8_t *USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length)
{
    USBD_GetString (USBD_CONFIGURATION_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

uint8_t *USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length)
{
    USBD_GetString (USBD_INTERFACE_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}
