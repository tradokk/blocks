#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "draw_game.h"
#include "generic_renderer.h"
#include "platform.h"

#define WINDOW_WIDTH (SCREEN_WIDTH * 4)
#define WINDOW_HEIGHT (SCREEN_HEIGHT * 4)

struct SDLPlatform {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *screen_texture;
	bool is_running;
} sdl_platform;

void platformInit()
{
	sdl_platform.is_running = true;
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "initialization failed!\n");
		fprintf(stderr, "description: %s", SDL_GetError());
		SDL_Quit();
	}
	sdl_platform.window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED,
	                                       SDL_WINDOWPOS_UNDEFINED,
	                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (sdl_platform.window == NULL) {
		fprintf(stderr, "window creation failed!\n");
		SDL_Quit();
	}

	sdl_platform.renderer = SDL_CreateRenderer(
	    sdl_platform.window, -1,
	    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (sdl_platform.renderer == NULL) {
		fprintf(stderr, "renderer creation failed!\n");
		SDL_Quit();
	}

	sdl_platform.screen_texture = SDL_CreateTexture(
	    sdl_platform.renderer, SDL_PIXELFORMAT_ARGB8888,
	    SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	clear(0xFF000000);
}
static void processInput(struct Game *game)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				sdl_platform.is_running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_UP ||
				    event.key.keysym.sym == SDLK_k) {
					game->rotate_right = true;
				} else if ((event.key.keysym.sym == SDLK_LEFT) ||
				           (event.key.keysym.sym == SDLK_h)) {
					game->key_left = true;
				} else if ((event.key.keysym.sym == SDLK_RIGHT) ||
				           (event.key.keysym.sym == SDLK_l)) {
					game->key_right = true;
				} else if ((event.key.keysym.sym == SDLK_DOWN) ||
				           (event.key.keysym.sym == SDLK_j)) {
					game->key_down = true;
				} else if (event.key.keysym.sym == SDLK_ESCAPE) {
					sdl_platform.is_running = false;
				} else if (event.key.keysym.sym == SDLK_x) {
					game->rotate_right = true;
				} else if (event.key.keysym.sym == SDLK_y) {
					game->rotate_right = true;
				}
				break;

			case SDL_KEYUP:
				if ((event.key.keysym.sym == SDLK_LEFT) ||
				    (event.key.keysym.sym == SDLK_h)) {
					game->key_left = false;
				} else if ((event.key.keysym.sym == SDLK_RIGHT) ||
				           (event.key.keysym.sym == SDLK_l)) {
					game->key_right = false;
				} else if ((event.key.keysym.sym == SDLK_DOWN) ||
				           (event.key.keysym.sym == SDLK_j)) {
					game->key_down = false;
				}
				break;
			default:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
		}
	}
}
static void render()
{
	SDL_UpdateTexture(sdl_platform.screen_texture, 0, render_buffer,
	                  SCREEN_WIDTH * 4);
	SDL_RenderCopy(sdl_platform.renderer, sdl_platform.screen_texture, 0, 0);
	SDL_RenderPresent(sdl_platform.renderer);
}

void platformLoop(struct Game *game)
{
	unsigned int last_ticks = SDL_GetTicks();
	while (sdl_platform.is_running) {
		processInput(game);
		gameUpdate(game);
		gameDraw(game);
		render();
		unsigned int ticks = SDL_GetTicks();
		unsigned int elapsed = SDL_min(0, ticks - last_ticks);
		last_ticks = ticks;
		SDL_Delay(GAME_INTERVAL - elapsed);
	}
}

void platformExit()
{
	SDL_Quit();
}
