#include <draw.h>
#include <platform/generic_renderer.h>
#include <platform/platform.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH (SCREEN_WIDTH * 4)
#define WINDOW_HEIGHT (SCREEN_HEIGHT * 4)

struct PS2Platform {
	bool is_running;
} ps2_platform;

void platformInit()
{
	srand(0);
	ps2_platform.is_running = true;
	clear(0xFF000000);
}
static void processInput(struct Game *game)
{
	return;
}
static void render()
{
	return;
}

void platformLoop(struct Game *game)
{
	while (ps2_platform.is_running) {
		processInput(game);
		gameUpdate(game);
		gameDraw(game);
		render();
	}
}

void platformExit()
{
	exit(0);
}
