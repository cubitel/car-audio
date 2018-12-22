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
#include "isotp.h"

isotp_t *isotpOpen(can_t *can, uint32_t remoteId, uint32_t localId)
{
	return 0;
}

void isotpClose(isotp_t *conn)
{
}

int isotpSend(isotp_t *conn, const void *buf, uint32_t len)
{
	uint8_t frame[8];

	if (len <= 7) {
		/* Single frame message */
		frame[0] = len;
		memcpy(&frame[1], buf, len);
		canSend(conn->can, conn->remoteId, frame, len + 1);
		return len;
	}

	return 0;
}

int isotpRecv(isotp_t *conn, void *buf, uint32_t len)
{
	return 0;
}
