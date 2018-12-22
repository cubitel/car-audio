#ifndef _RADIO_API_H
#define _RADIO_API_H

#include "hardware.h"

#define RADIO_DATA_RSSI         0x0101
#define RADIO_DATA_SNR          0x0102
#define RADIO_DATA_STEREO       0x0103
#define RADIO_DATA_RDS_PS_NAME  0x0201
#define RADIO_DATA_RDS_GROUPS   0x0202

#define RADIO_SEEK_DOWN         0
#define RADIO_SEEK_UP           1

typedef struct {
	void (*radioTune)(uint32_t frequency);
	void (*radioSeek)(uint8_t seekUp);
	void (*radioPoll)(void);
	int (*radioGetData)(uint16_t type, uint8_t *buf, uint16_t len);
} radio_t;

void radioInitTuner(void);
void radioTune(uint32_t frequency);
void radioSeek(uint8_t seekUp);
void radioPoll(void);
int radioGetData(uint16_t type, uint8_t *buf, uint16_t len);

radio_t *si47xxInit(void);

#endif
