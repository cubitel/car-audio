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

#include <string.h>
#include "rds.h"

void rdsInit(rds_t *rds)
{
	memset(rds, 0, sizeof(rds_t));
}

void rdsProcessGroup(rds_t *rds, uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4)
{
	uint8_t gtype = block2 >> 11;
	uint8_t cbits;

	rds->validGroupTypes |= (1 << gtype);

	switch (gtype) {
	case 0:
	case 1:
		/* Groups 0A & 0B */
		cbits = block2 & 3;
		switch (cbits) {
		case 0:
			rds->psNameShadow[0] = block4 >> 8;
			rds->psNameShadow[1] = block4 & 0xFF;
			break;
		case 1:
			rds->psNameShadow[2] = block4 >> 8;
			rds->psNameShadow[3] = block4 & 0xFF;
			break;
		case 2:
			rds->psNameShadow[4] = block4 >> 8;
			rds->psNameShadow[5] = block4 & 0xFF;
			break;
		case 3:
			rds->psNameShadow[6] = block4 >> 8;
			rds->psNameShadow[7] = block4 & 0xFF;
			memcpy(rds->psName, rds->psNameShadow, sizeof(rds->psName));
			break;
		}
		break;
	default:
		/* Ignore unknown groups */
		break;
	}
}
