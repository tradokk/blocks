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

#include "renderer.h"

#ifdef UI
#define CONTROL_HEIGHT (CELL_SIZE * 10)
#define DISPLAY_WIDTH SCREEN_WIDTH
#define DISPLAY_HEIGHT (SCREEN_HEIGHT + CONTROL_HEIGHT)

#define CROSS_COLOR0 0xfff0f0f0
#define CROSS_COLOR1 0xff222222
#define CROSS_COLOR2 0xff444444
#define CROSS_COLOR3 0xff777777

#define BUTTON_COLOR0 0xfff0f0f0
#define BUTTON_COLOR1 0xff531e85
#define BUTTON_COLOR2 0xff65349a
#define BUTTON_COLOR3 0xff7546ad
#else
#define DISPLAY_WIDTH SCREEN_WIDTH
#define DISPLAY_HEIGHT SCREEN_HEIGHT
#endif

#ifdef UI
void renderButton(int posx, int posy);
void renderCross(int posx, int posy);
#endif

extern uint32_t render_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
void renderTileWithPalette(const uint8_t *data, int posx, int posy,
                           const uint32_t palette[4]);
#endif
