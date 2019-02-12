#ifndef _CARS_API_H
#define _CARS_API_H

#include <stdint.h>
#include "hw/can.h"

#define CAREVENT_VOLUME_CHANGED         1

#define CAR_DATA_ENGINE_RPM             0x0101

#define CAR_TYPE_FORD_CD3XX             1
#define CAR_TYPE_MERCEDES_W164          2

typedef struct {
	void (*canInit)(can_t *can1, can_t *can2);
	void (*canRecv)(uint32_t id, uint8_t *data, uint8_t len);
	void (*canSendTimer)(uint8_t slot, can_t *can1, can_t *can2);
	void (*event)(uint32_t eventId);
	int (*isActive)(void);
	int (*getData)(uint32_t type, uint8_t *buf, uint32_t len);
} car_t;

void carInit(uint32_t type);

void carCanInit(can_t *can1, can_t *can2);
void carCanRecv(uint32_t id, uint8_t *data, uint8_t len);
void carCanSendTimer(uint8_t slot, can_t *can1, can_t *can2);
void carEvent(uint32_t eventId);
int carIsActive(void);
int carGetData(uint32_t type, uint8_t *buf, uint32_t len);

const car_t *ford_cd3xx(void);
const car_t *mercedes_w164(void);

#endif
