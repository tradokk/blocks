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

#include "generic_renderer.h"

#include <memory.h>
#include <resource.h>
#include <stdint.h>
#include <string.h>

uint32_t render_buffer[DISPLAY_HEIGHT * DISPLAY_WIDTH];
static const uint32_t screen_colors[] = {COLOR0, COLOR1, COLOR2, 0};

#ifdef UI
extern const struct TileSet cross_tiles;
extern const struct TileSet button_tiles;

static const uint32_t cross_colors[] = {CROSS_COLOR0, CROSS_COLOR1,
                                        CROSS_COLOR2, CROSS_COLOR3};
static const uint32_t button_colors[] = {BUTTON_COLOR0, BUTTON_COLOR1,
                                         BUTTON_COLOR2, BUTTON_COLOR3};

void renderButton(int posx, int posy)
{
	int i = 0;
	for (int y = posy * CELL_SIZE; y < (posy + 4) * CELL_SIZE; y += CELL_SIZE) {
		for (int x = posx * CELL_SIZE; x < (posx + 4) * CELL_SIZE;
		     x += CELL_SIZE) {
			renderTileWithPalette(button_tiles.data + i, x, y, button_colors);
			i += 16;
		}
	}
};

void renderCross(int posx, int posy)
{
	int i = 0;
	for (int y = posy * CELL_SIZE; y < (posy + 8) * CELL_SIZE; y += CELL_SIZE) {
		for (int x = posx * CELL_SIZE; x < (posx + 8) * CELL_SIZE;
		     x += CELL_SIZE) {
			renderTileWithPalette(cross_tiles.data + i, x, y, cross_colors);
			i += 16;
		}
	}
};

#endif

void clear(uint32_t color)
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		render_buffer[i] = color;
	}
}

void clearArea(uint32_t color, int left, int top, int right, int bottom)
{
	for (int j = top * SCREEN_WIDTH; j < bottom * SCREEN_WIDTH;
	     j += SCREEN_WIDTH) {
		for (int i = j + left; i < j + right; i++) {
			render_buffer[i] = color;
		}
	}
}

void renderGlyph(const uint8_t *data, int posx, int posy)
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

void renderTile(const uint8_t *data, int posx, int posy)
{
	renderTileWithPalette(data, posx, posy, screen_colors);
}

void renderTileWithPalette(const uint8_t *data, int posx, int posy,
                           const uint32_t palette[4])
{
	int z = 0;
	for (int y = posy; y < posy + 8; y++) {
		int x = posx;
		for (int j = 0; j < 2; j++) {
			uint8_t val = data[z];
			for (int k = 0; k < 4; k++) {
				if ((x >= 0) && (y >= 0)) {
					render_buffer[y * SCREEN_WIDTH + x] =
					    palette[(val & 0xc0) >> 6];
				}
				val <<= 2;
				x++;
			}
			z++;
		}
	}
}

