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
#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include "game.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

#ifdef COLOR_BGRA
#define COLOR0 0xFF306230  // Dark
#define COLOR1 0xFF8bac0f  // Light
#define COLOR2 0xFF9bbc0f  // Very Light
#define COLOR3 0xFF0f380f  // Very Dark
#else
// RGBA
#define COLOR0 0xFF306230  // Dark
#define COLOR1 0xFF0fac8b  // Light
#define COLOR2 0xFF0fbc9b  // Very Light
#define COLOR3 0xFF0f380f  // Very Dark
#endif

#define TEXT_FG_COLOR COLOR0
#define TEXT_BG_COLOR COLOR2

void clear(uint32_t color);
void clearArea(uint32_t color, int left, int top, int right, int bottom);
void renderGlyph(const uint8_t *data, int x, int y);
void renderTile(const uint8_t *data, int x, int y);

#endif
