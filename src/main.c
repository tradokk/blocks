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

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generic_renderer.h"
#include "draw.h"
#include "game.h"

#define WINDOW_WIDTH (SCREEN_WIDTH * 4)
#define WINDOW_HEIGHT (SCREEN_HEIGHT * 4)

struct Game game;
int main()
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "initialization failed!\n");
		fprintf(stderr, "description: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED,
	                                      SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
	                                      WINDOW_HEIGHT, 0);
	if (window == NULL) {
		fprintf(stderr, "window creation failed!\n");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(
	    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		fprintf(stderr, "renderer creation failed!\n");
		SDL_Quit();
		return 1;
	}

	SDL_Texture *screen_texture = SDL_CreateTexture(
	    renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
	    SCREEN_WIDTH, SCREEN_HEIGHT);
	clear(0xFF000000);

	init(&game);
	unsigned int last_ticks = SDL_GetTicks();
	bool is_running = true;
	while (is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					is_running = false;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_UP ||
					    event.key.keysym.sym == SDLK_k) {
						rotate(&game);
					} else if ((event.key.keysym.sym == SDLK_LEFT) ||
					           (event.key.keysym.sym == SDLK_h)) {
						game.key_left = true;
					} else if ((event.key.keysym.sym == SDLK_RIGHT) ||
					           (event.key.keysym.sym == SDLK_l)) {
						game.key_right = true;
					} else if ((event.key.keysym.sym == SDLK_DOWN) ||
					           (event.key.keysym.sym == SDLK_j)) {
						game.key_down = true;
					}

					else if (event.key.keysym.sym == SDLK_ESCAPE) {
						is_running = false;
					}
				    break;

			    case SDL_KEYUP:
				    if ((event.key.keysym.sym == SDLK_LEFT) ||
					    (event.key.keysym.sym == SDLK_h)) {
						game.key_left = false;
					} else if ((event.key.keysym.sym == SDLK_RIGHT) ||
					           (event.key.keysym.sym == SDLK_l)) {
						game.key_right = false;
					} else if ((event.key.keysym.sym == SDLK_DOWN) ||
					           (event.key.keysym.sym == SDLK_j)) {
						game.key_down = false;
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
		update(&game);
		draw(&game);
		SDL_UpdateTexture(screen_texture, 0, render_buffer, SCREEN_WIDTH * 4);
		SDL_RenderCopy(renderer, screen_texture, 0, 0);
		SDL_RenderPresent(renderer);
		unsigned int ticks = SDL_GetTicks();
		unsigned int elapsed = SDL_min(0, ticks - last_ticks);
		last_ticks = ticks;
		SDL_Delay(GAME_INTERVAL - elapsed);
	}

	SDL_Quit();
	return 0;
}
