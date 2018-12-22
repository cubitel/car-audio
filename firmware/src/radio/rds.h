#ifndef _RADIO_RDS_H
#define _RADIO_RDS_H

#include <stdint.h>

typedef struct {
	uint8_t psName[8];
	uint8_t psNameShadow[8];
	uint32_t validGroupTypes;
} rds_t;

void rdsInit(rds_t *rds);
void rdsProcessGroup(rds_t *rds, uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4);

#endif
