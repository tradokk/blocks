/*	Copyright (C) 2017 David Leiter
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <platform/generic_renderer.h>

#include <stdint.h>
#include <string.h>

static uint32_t colors[] = {COLOR0, COLOR1, COLOR2, 0};

uint32_t render_buffer[SCREEN_HEIGHT * SCREEN_WIDTH];

void clear(uint32_t color)
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		render_buffer[i] = color;
	}
}

void renderGlyph(uint8_t *data, int posx, int posy)
{
	int z = 0;
	for (int y = posy; y < posy + 8; y++) {
		int x = posx;
		uint8_t val = data[z];
		if (x >= 0)
			for (int k = 0; k < 8; k++) {
				if ((x >= 0) && (y >= 0)) {
					render_buffer[y * SCREEN_WIDTH + x] =
					    ((val & 0x80) != 0) ? TEXT_FG_COLOR : TEXT_BG_COLOR;
				}
				val <<= 1;
				x++;
			}

		z++;
	}
}

void renderTile(uint8_t *data, int posx, int posy)
{
	int z = 0;
	for (int y = posy; y < posy + 8; y++) {
		int x = posx;
		for (int j = 0; j < 2; j++) {
			uint8_t val = data[z];
			for (int k = 0; k < 4; k++) {
				if ((x >= 0) && (y >= 0)) {
					render_buffer[y * SCREEN_WIDTH + x] =
					    colors[(val & 0xc0) >> 6];
				}
				val <<= 2;
				x++;
			}
			z++;
		}
	}
}
