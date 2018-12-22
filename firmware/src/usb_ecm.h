#ifndef USB_ECM_H_
#define USB_ECM_H_

#include <stdint.h>

void ethControl(void *pdev, uint8_t *buf, uint32_t len);
uint16_t ethControlIn(void *pdev, uint8_t *buf, uint32_t maxlen);
void ecmDataRx(void *pdev, uint8_t epnum, uint32_t len);
void ecmDataTxCompleted(void *pdev, uint8_t epnum);
void ecmEnable(void);
void ecmDisable(void);

#endif /* USB_ECM_H_ */
