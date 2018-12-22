#ifndef _CONFIG_H
#define _CONFIG_H

#include "config.pb.h"

int configValid(uint8_t *buf);
int configRead(uint8_t *buf);
void configWrite(void);
void configSaveIfNeeded(int poweroff);
Config_Config *configData(void);

#endif
