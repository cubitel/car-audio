#ifndef USB_AUDIO_H_
#define USB_AUDIO_H_

#include <stdint.h>

void audioDataRx(void *pdev, uint8_t epnum, uint32_t len);
int16_t *audioGetPlayBuffer(uint32_t len);
uint32_t audioGetRxCount(void);

#endif /* USB_AUDIO_H_ */
