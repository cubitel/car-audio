#ifndef __USB_CLASS_H_
#define __USB_CLASS_H_

#include  "usbd_ioreq.h"

#define APP_RX_DATA_SIZE	256


#define USB_CDC_CONFIG_DESC_SIZ                (162)
#define USB_CDC_DESC_SIZ                       (162-9)

#define CDC_DESCRIPTOR_TYPE                     0x21

#define DEVICE_CLASS_CDC                        0x02
#define DEVICE_SUBCLASS_CDC                     0x00


#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

extern USBD_Class_cb_TypeDef  USBD_CDC_cb;

#endif
