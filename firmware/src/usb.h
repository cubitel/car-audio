#ifndef USB_H_
#define USB_H_

#include "usb_conf.h"

void usbInit(void);

uint16_t usb_Ctrl(void *pdev, uint32_t type, uint32_t cmd, uint32_t ifnum, uint8_t* buf, uint32_t len);

#endif /* USB_H_ */
