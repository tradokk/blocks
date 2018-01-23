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

#include <draw_game.h>
#include <game.h>
#include <platform/renderer.h>

#define I_TL 0 * 16
#define I_T 1 * 16
#define I_TR 2 * 16
#define I_L 6 * 16
#define I_C 7 * 16
#define I_R 8 * 16
#define I_BL 12 * 16
#define I_B 13 * 16
#define I_BR 14 * 16
#define I_BL2 3 * 16
#define I_B2 4 * 16
#define I_BR2 5 * 16

extern struct TileSet interface_tiles;

extern struct TileSet blocks_tiles;

extern struct TileSet text_tiles;

extern struct GlyphMap numbers_glyphs;

static void drawCurrentPiece(struct Game *game);

static void drawPiece(int piece, int rotation, struct Pos *pos,
                      struct Game *game);

static void drawInterface(struct Game *game);

static void drawGameGrid(struct Game *game);

static void drawInterfaceBox(struct Pos *pos, int width, int height,
                             uint8_t *box);

static void drawText(struct Pos *pos, int line, int length);

static void drawNumber(struct Pos *pos, int number);

static uint8_t piece_box[] = {
    I_TL, I_T,  I_T,   I_T,  I_T,  I_T,  I_TR, I_L,  I_C,   I_C, I_C, I_C,
    I_C,  I_R,  I_BL2, I_B2, I_B2, I_B2, I_B2, I_B2, I_BR2, I_L, I_C, I_C,
    I_C,  I_C,  I_C,   I_R,  I_L,  I_C,  I_C,  I_C,  I_C,   I_C, I_R, I_L,
    I_C,  I_C,  I_C,   I_C,  I_C,  I_R,  I_L,  I_C,  I_C,   I_C, I_C, I_C,
    I_R,  I_BL, I_B,   I_B,  I_B,  I_B,  I_B,  I_BR};

static uint8_t text_box[] = {I_TL,  I_T,  I_T,  I_T,  I_T,  I_T,  I_T,  I_TR,
                             I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,
                             I_BL2, I_B2, I_B2, I_B2, I_B2, I_B2, I_B2, I_BR2,
                             I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,
                             I_BL,  I_B,  I_B,  I_B,  I_B,  I_B,  I_B,  I_BR};
static uint8_t text_box_small[] = {I_TL, I_T, I_T, I_T, I_T, I_T, I_T, I_TR,
                                   I_L,  I_C, I_C, I_C, I_C, I_C, I_C, I_R,
                                   I_L,  I_C, I_C, I_C, I_C, I_C, I_C, I_R,
                                   I_BL, I_B, I_B, I_B, I_B, I_B, I_B, I_BR};

static void drawPiece(int piece, int rotation, struct Pos *pos,
                      struct Game *game)
{
	const struct Pos *blocks = game->pieces[piece].rotations[rotation].blocks;
	for (int i = 0; i < 4; i++) {
		int x = (blocks[i].x + pos->x) * CELL_SIZE;
		int y = (blocks[i].y + pos->y) * CELL_SIZE;
		renderTile(&blocks_tiles.data[piece * 16], x, y);
	}
}

static void drawCurrentPiece(struct Game *game)

{
	drawPiece(game->current_piece.piece, game->current_piece.rotation,
	          &game->piece_pos, game);
}

static void drawInterface(struct Game *game)
{
	struct Pos pos = {GRID_WIDTH, 0};
	drawInterfaceBox(&pos, 8, 5, text_box);
	pos.x = GRID_WIDTH + 1;
	pos.y++;
	drawText(&pos, 1, 5);
	pos.x = GRID_WIDTH + 6;
	pos.y += 2;
	drawNumber(&pos, game->score);

	pos.x = GRID_WIDTH;
	pos.y += 2;
	drawInterfaceBox(&pos, 8, 4, text_box_small);
	pos.x = GRID_WIDTH + 1;
	pos.y++;
	drawText(&pos, 2, 5);
	pos.x = GRID_WIDTH + 6;
	pos.y += 1;
	drawNumber(&pos, game->level);

	pos.x = GRID_WIDTH;
	pos.y += 2;
	drawInterfaceBox(&pos, 8, 4, text_box_small);
	pos.x = GRID_WIDTH + 1;
	pos.y++;
	drawText(&pos, 3, 5);
	pos.x = GRID_WIDTH + 6;
	pos.y += 1;
	drawNumber(&pos, game->lines);

	pos.x = GRID_WIDTH;
	pos.y += 2;
	drawInterfaceBox(&pos, 7, 8, piece_box);
	pos.x = GRID_WIDTH + 1;
	pos.y++;
	drawText(&pos, 0, 5);

	pos.x = GRID_WIDTH + 2;
	pos.y += 4;
	drawPiece(game->next_piece.piece, game->next_piece.rotation, &pos, game);

	pos.x = GRID_WIDTH;
	pos.y = 8;
}

static void drawInterfaceBox(struct Pos *pos, int width, int height,
                             uint8_t *box)
{
	int posx = pos->x * CELL_SIZE;
	int posy = pos->y * CELL_SIZE;
	int i = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// RenderTile(&interface_tilemap.data[box[i]], posx, posy);
			renderTile(&interface_tiles.data[box[i]], posx, posy);
			posx += CELL_SIZE;
			i++;
		}
		posx = pos->x * CELL_SIZE;
		posy += CELL_SIZE;
	}
}

static void drawGameGrid(struct Game *game)
{
	int posx = 0;
	int posy = 0;
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (game->field[y][x] > 0) {
				renderTile(&blocks_tiles.data[(game->field[y][x] - 1) * 16],
				           posx, posy);
			}
			posx += CELL_SIZE;
		}
		posy += CELL_SIZE;
		posx = 0;
	}
}

static void drawText(struct Pos *pos, int line, int length)
{
	int x = pos->x * CELL_SIZE;
	int y = pos->y * CELL_SIZE;
	int offset = 5 * GLYPH_HEIGHT * line;
	for (int i = 0; i < length; i++) {
		renderGlyph(&text_tiles.data[offset], x, y);
		offset += 8;
		x += CELL_SIZE;
	}
}

static void drawNumber(struct Pos *pos, int number)
{
	int x = pos->x * CELL_SIZE;
	int y = pos->y * CELL_SIZE;

	int remaining = number;
	do {
		int c = remaining % 10;
		renderGlyph(&numbers_glyphs.data[c * GLYPH_HEIGHT], x, y);
		remaining = remaining / 10;
		x -= CELL_SIZE;
	} while (remaining > 0);
}

void gameDraw(struct Game *game)
{
	clear(COLOR2);
	drawGameGrid(game);
	drawCurrentPiece(game);
	drawInterface(game);
}
