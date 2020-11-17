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
#ifndef DRAW_GAME_H
#define DRAW_GAME_H

#define INTERFACE_WIDTH 8
#define INTERFACE_HEIGHT GRID_HEIGHT
#define LEFT_BORDER_WIDTH 1
#define RIGHT_BORDER_WIDTH 1

#include "game.h"
#include "resource.h"

struct Game;

void gameDrawStatic(const struct Game *game);
void gameDrawChanges(const struct Game *game);

#endif
