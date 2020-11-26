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

#include "draw_game.h"

#include "game.h"
#include "renderer.h"

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

#define I_TBL 9 * 16
#define I_TB 10 * 16
#define I_TBR 11 * 16

static struct GameDrawState {
	bool titlescreen_state;
	bool play_state;
	bool gameover_state;
} draw_state;

extern const struct TileSet interface_tiles;

extern const struct TileSet blocks_tiles;

extern const struct GlyphMap font_glyphs;

static void drawCurrentPiece(const struct Game *game);

static void drawPiece(int piece, int rotation, int x, int y,
                      const struct Game *game);

static void clearCurrentPiece(const struct Game *game);

static void clearPiece(int piece, int rotation, int x, int y,
                       const struct Game *game);

static void drawStaticInterface(const struct Game *game);

static void drawInterfaceContent(const struct Game *game);

static void drawGameGrid(const struct Game *game, int start, int end);

static void drawInterfaceBox(int x, int y, int width, int height,
                             const uint8_t *box);

static void drawText(int x, int y, const char *text);

static void drawNumber(int x, int y, int number);

static const uint8_t piece_box[] = {
    I_TL, I_T, I_T, I_T, I_T, I_T, I_T, I_TR,  // 1
    I_L,  I_C, I_C, I_C, I_C, I_C, I_C, I_R,   // 2
    I_L,  I_C, I_C, I_C, I_C, I_C, I_C, I_R,   // 3
    I_L,  I_C, I_C, I_C, I_C, I_C, I_C, I_R,   // 4
    I_L,  I_C, I_C, I_C, I_C, I_C, I_C, I_R,   // 5
    I_BL, I_B, I_B, I_B, I_B, I_B, I_B, I_BR   // 6
};

static uint8_t const text_box[] = {
    I_TL,  I_T,  I_T,  I_T,  I_T,  I_T,  I_T,  I_TR,   // 1
    I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,    // 2
    I_BL2, I_B2, I_B2, I_B2, I_B2, I_B2, I_B2, I_BR2,  // 3
    I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,    // 4
    I_BL,  I_B,  I_B,  I_B,  I_B,  I_B,  I_B,  I_BR    // 5
};

static uint8_t const text_boxes_level_and_lines[] = {
    I_TL,  I_T,  I_T,  I_T,  I_T,  I_T,  I_T,  I_TR,   // 1
    I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,    // 2
    I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,    // 3
    I_TBL, I_TB, I_TB, I_TB, I_TB, I_TB, I_TB, I_TBR,  // 4
    I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,    // 5
    I_L,   I_C,  I_C,  I_C,  I_C,  I_C,  I_C,  I_R,    // 6
    I_BL,  I_B,  I_B,  I_B,  I_B,  I_B,  I_B,  I_BR    // 7
};

static void clearPiece(int piece, int rotation, int posx, int posy,
                       const struct Game *game)
{
	const struct Pos *blocks = game->pieces[piece].rotations[rotation].blocks;
	for (int i = 0; i < 4; i++) {
		int x = (blocks[i].x + posx) * CELL_SIZE;
		int y = (blocks[i].y + posy) * CELL_SIZE;
		if ((x >= 0) && (y >= 0)) {
			clearTile(COLOR2, x, y);
		}
	}
}

static void drawPiece(int piece, int rotation, int posx, int posy,
                      const struct Game *game)
{
	const struct Pos *blocks = game->pieces[piece].rotations[rotation].blocks;
	for (int i = 0; i < 4; i++) {
		int x = (blocks[i].x + posx) * CELL_SIZE;
		int y = (blocks[i].y + posy) * CELL_SIZE;
		if ((x >= 0) && (y >= 0)) {
			renderTile(&blocks_tiles.data[piece * 16], x, y);
		}
	}
}

static void clearDirtyPiece(const struct Game *game)

{
	struct Pos pos = game->dirty_piece_pos;
	clearPiece(game->dirty_piece.piece, game->dirty_piece.rotation,
	           pos.x + LEFT_BORDER_WIDTH, pos.y, game);
}

static void drawCurrentPiece(const struct Game *game)

{
	struct Pos pos = game->piece_pos;
	drawPiece(game->current_piece.piece, game->current_piece.rotation,
	          pos.x + LEFT_BORDER_WIDTH, pos.y, game);
}

static void drawStaticInterface(const struct Game *game)
{
	int left = LEFT_BORDER_WIDTH + GRID_WIDTH + RIGHT_BORDER_WIDTH;
	int x = left;
	int y = 0;
	drawInterfaceBox(x, y, 8, 5, text_box);
	x = left + 1;
	y++;
	drawText(x, y, "Score");
	x = left;
	y += 4;
	drawInterfaceBox(x, y, 8, 7, text_boxes_level_and_lines);
	x = left + 1;
	y++;
	drawText(x, y, "Level");
	x = left + 1;
	y += 3;
	drawText(x, y, "Lines");
	x = left;
	y += 3;
	drawInterfaceBox(x, y, 8, 6, piece_box);
}

static void drawInterfaceContent(const struct Game *game)
{
	int left = LEFT_BORDER_WIDTH + GRID_WIDTH + RIGHT_BORDER_WIDTH;
	int x = left + 6;
	int y = 3;
	drawNumber(x, y, game->score);
	y += 4;
	drawNumber(x, y, game->level);
	y += 3;
	drawNumber(x, y, game->lines);
	x = left + 3;
	y += 5;
	clearPiece(game->current_piece.piece, game->current_piece.rotation, x, y,
	           game);
	drawPiece(game->next_piece.piece, game->next_piece.rotation, x, y, game);
}

static void drawBorder(int left, int top, int right, int bottom)
{
	for (int y = top * CELL_SIZE; y < bottom * CELL_SIZE; y += CELL_SIZE) {
		for (int x = left * CELL_SIZE; x < right * CELL_SIZE; x += CELL_SIZE) {
			renderTile(&interface_tiles.data[15 * 16], x, y);
		}
	}
}

static void drawInterfaceBox(int posx, int posy, int width, int height,
                             const uint8_t *box)
{
	int x = posx * CELL_SIZE;
	int y = posy * CELL_SIZE;
	int k = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			renderTile(&interface_tiles.data[box[k]], x, y);
			x += CELL_SIZE;
			k++;
		}
		x = posx * CELL_SIZE;
		y += CELL_SIZE;
	}
}

static void drawGameGrid(const struct Game *game, int start, int end)
{
	int posx = CELL_SIZE * LEFT_BORDER_WIDTH;
	int posy = start * CELL_SIZE;
	for (int y = start; y <= end; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (game->field[y][x] > 0) {
				renderTile(&blocks_tiles.data[(game->field[y][x] - 1) * 16],
				           posx, posy);
			} else {
				clearTile(COLOR2, posx, posy);
			}
			posx += CELL_SIZE;
		}
		posy += CELL_SIZE;
		posx = CELL_SIZE * LEFT_BORDER_WIDTH;
	}
}

static void drawText(int posx, int posy, const char *text)
{
	int x = posx * CELL_SIZE;
	int y = posy * CELL_SIZE;
	while (*text != 0) {
		int offset = (*text - 0x20) * GLYPH_HEIGHT;
		renderGlyph(&font_glyphs.data[offset], x, y);
		x += CELL_SIZE;
		text++;
	}
}

static void drawNumber(int posx, int posy, int number)
{
	int x = posx * CELL_SIZE;
	int y = posy * CELL_SIZE;

	int remaining = number;
	do {
		int c = (remaining % 10) + 16;
		renderGlyph(&font_glyphs.data[c * GLYPH_HEIGHT], x, y);
		remaining = remaining / 10;
		x -= CELL_SIZE;
	} while (remaining > 0);
}

static void drawStaticUi(const struct Game *game)
{
	clear(COLOR2);
	drawBorder(0, 0, LEFT_BORDER_WIDTH, GRID_HEIGHT);
	drawBorder(LEFT_BORDER_WIDTH + GRID_WIDTH, 0,
	           LEFT_BORDER_WIDTH + GRID_WIDTH + RIGHT_BORDER_WIDTH,
	           GRID_HEIGHT);
	drawStaticInterface(game);
	drawInterfaceContent(game);
}

static void drawChanges(const struct Game *game)
{
	if (game->field_is_dirty) {
		drawGameGrid(game, game->dirty_line_start, game->dirty_line_end);
	}
	if (game->piece_is_dirty) {
		clearDirtyPiece(game);
		drawCurrentPiece(game);
	}
	if (game->interface_is_dirty) {
		drawInterfaceContent(game);
	}
}

static void drawTitleScreen()
{
	clear(COLOR2);
	drawText(3, 7, "Falling Blocks");
	drawText(2, 9, "Press Start/Space");
}
static void drawGamoverScreen()
{
	clear(COLOR2);
	drawText(6, 7, "Game Over");
	drawText(2, 9, "Press Start/Space");
}

void gameInitDrawing()
{
	draw_state.titlescreen_state = false;
	draw_state.play_state = false;
	draw_state.gameover_state = false;
	drawTitleScreen();
}

void gameDraw(const struct Game *game)
{
	if (game->mode == GAMEMODE_TITLESCREEN) {
		if (!draw_state.titlescreen_state) {
			draw_state.titlescreen_state = true;
			draw_state.play_state = false;
			draw_state.gameover_state = false;
			drawTitleScreen();
		}
	} else if (game->mode == GAMEMODE_PLAY) {
		if (!draw_state.play_state) {
			draw_state.play_state = true;
			draw_state.titlescreen_state = false;
			draw_state.gameover_state = false;
			drawStaticUi(game);
			drawChanges(game);
		} else {
			drawChanges(game);
		}
	} else {
		if (!draw_state.gameover_state) {
			draw_state.gameover_state = true;
			draw_state.titlescreen_state = false;
			draw_state.play_state = false;
			drawGamoverScreen();
		}
	}
}
