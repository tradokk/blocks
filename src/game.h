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

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define CELL_SIZE 8

#define GRID_WIDTH 10
#define GRID_HEIGHT 21

#define GAME_INTERVAL 16
#define MOVE_INTERVAL_COUNT 8
#define SOFT_DROP_INTERVAL_COUNT 3
#define DROP_INTERVAL_COUNT 54

struct Pos {
	int8_t x;
	int8_t y;
};

struct Rotation {
	struct Pos blocks[4];
};

struct Piece {
	int count;
	struct Rotation rotations[4];
};

struct PieceState {
	int piece;
	int rotation;
};

enum Direction { DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT };

struct Game {
	bool key_left;
	bool key_right;
	bool key_down;
	bool rotate;
	int score;
	int lines;
	int level;
	struct PieceState current_piece;
	struct PieceState next_piece;
	int interval_count;
	struct Pos piece_pos;
	int points[4];
	const struct Piece *pieces;
	int8_t field[GRID_HEIGHT][GRID_WIDTH];
};

void gameInit(struct Game *game);

void gameUpdate(struct Game *game);

#endif
