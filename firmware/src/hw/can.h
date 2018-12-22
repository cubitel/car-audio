#ifndef _CAN_H
#define _CAN_H

#include <stdint.h>
#include <stm32f4xx.h>

#define CAN_MODE_OFF            0
#define CAN_MODE_AUTOBAUD       1
#define CAN_MODE_LISTEN         2
#define CAN_MODE_NORMAL         3

#define CAN_SPEED_125           0
#define CAN_SPEED_250           1
#define CAN_SPEED_500           2
#define CAN_SPEED_1000          3
#define CAN_SPEED_MAXID         3

#define CAN_AUTOBAUD_TIMEOUT    500
#define CAN_RX_TIMEOUT          2000

#define CAN_FLAG_WAIT_INIT      1
#define CAN_FLAG_WAIT_NORMAL    2
#define CAN_FLAG_NO_RX_TIMEOUT  4

typedef struct {
	CAN_TypeDef *device;
	uint8_t mode;
	uint8_t speed;
	uint32_t flags;
	uint32_t autobaudTimer;
	uint32_t rxTimer;
	uint32_t rxCount;
	uint32_t rxTotalCount;
} can_t;

typedef struct {
	uint8_t mode;
	uint8_t speed;
	uint8_t flags;
	uint32_t rxCount;
} can_status_t;

void canInit(void);
void canProcess(uint32_t msec);
void canSend(can_t *can, uint32_t id, uint8_t *data, uint8_t len);
int canIsActive(void);
void canGetStatus(can_status_t *status);

uint32_t canGetSignal(uint8_t *data, uint8_t offset, uint8_t size);
void canSetSignal(uint8_t *data, uint8_t offset, uint8_t size, uint32_t value);

#endif
