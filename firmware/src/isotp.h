#ifndef _ISOTP_H
#define _ISOTP_H

#include <stdint.h>
#include "hw/can.h"

typedef struct {
	can_t *         can;
	uint32_t        remoteId;
	uint32_t        localId;
} isotp_t;

isotp_t *isotpOpen(can_t *can, uint32_t remoteId, uint32_t localId);
void isotpClose(isotp_t *conn);

int isotpSend(isotp_t *conn, const void *buf, uint32_t len);
int isotpRecv(isotp_t *conn, void *buf, uint32_t len);

#endif
