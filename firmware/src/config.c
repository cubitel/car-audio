/* Copyright (c) 2018 Vasily Voropaev <vvg@cubitel.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stm32f4xx.h>
#include <string.h>
#include "config.h"
#include "hardware.h"
#include "pb_decode.h"
#include "pb_encode.h"

#define MAX_CONFIG_SIZE         4000

static Config_Config main_config = Config_Config_init_default;
static Config_Config checked_config = Config_Config_init_default;

int configValid(uint8_t *buf)
{
	uint16_t configSize = buf[0] | (buf[1] << 8);
	if ((configSize == 0) || (configSize >= MAX_CONFIG_SIZE)) return 0;

	pb_istream_t istream = pb_istream_from_buffer(&buf[2], configSize);
	if (!pb_decode(&istream, Config_Config_fields, &checked_config)) {
		return 0;
	}

	return 1;
}

int configRead(uint8_t *buf)
{
	if (!configValid(buf)) return 0;

	memcpy(&main_config, &checked_config, sizeof(main_config));

	return 1;
}

void configWrite()
{
	/* Enable write access to backup SRAM */
	PWR->CR |= PWR_CR_DBP;

	/* Write config */
	uint8_t *buf = (uint8_t *)BKPSRAM_BASE;
	pb_ostream_t ostream = pb_ostream_from_buffer(&buf[2], MAX_CONFIG_SIZE);
	pb_encode(&ostream, Config_Config_fields, &main_config);
	uint16_t len = ostream.bytes_written;
	buf[0] = len & 0xFF;
	buf[1] = len >> 8;

	/* Disable write access to backup SRAM */
	PWR->CR &= ~PWR_CR_DBP;
}

void configSaveIfNeeded(int poweroff)
{
	uint8_t *ramConfig = (uint8_t *)BKPSRAM_BASE;
	uint8_t *flashConfig = (uint8_t *)FLASH_CONFIG_ADDRESS;

	if (!configValid(ramConfig)) return;

	uint16_t configSize = ramConfig[0] | (ramConfig[1] << 8);
	if (memcpy(ramConfig, flashConfig, configSize+2) == 0) return;

	flashSaveConfig(ramConfig, configSize+2, poweroff);
}

Config_Config *configData()
{
	return &main_config;
}
