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
#ifndef GENERIC_RENDERER_H
#define GENERIC_RENDERER_H

#include <stdint.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define COLOR0 0xFF204631
#define COLOR1 0xFFa1bd56
#define COLOR2 0xFFd6e894

#define TEXT_FG_COLOR COLOR0
#define TEXT_BG_COLOR COLOR2

extern uint32_t render_buffer[SCREEN_HEIGHT * SCREEN_WIDTH];

void clear(uint32_t color);
void RenderGlyph(uint8_t *data, int x, int y);
void RenderTile(uint8_t *data, int x, int y);

#endif
