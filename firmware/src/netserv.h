#ifndef NETSERV_H
#define NETSERV_H

#include <stdint.h>

void netservTask(void *p);
uint32_t netservMakeStatusMessage(uint8_t *buf, uint32_t size);

#endif
