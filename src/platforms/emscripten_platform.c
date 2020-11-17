#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "draw_game.h"
#include "generic_renderer.h"
#include "platform.h"

#define CANVAS_WIDTH DISPLAY_WIDTH
#define CANVAS_HEIGHT DISPLAY_HEIGHT

#define CROSS_POSX 1
#define CROSS_POSY 19

#define BUTTONA_POSX 15
#define BUTTONA_POSY 19
#define BUTTONB_POSX 13
#define BUTTONB_POSY 23

struct EmscriptenPlatform {
	int current_canvas_width;
	int current_canvas_height;
	bool is_running;
} emscripten_platform;

int getCanvasWidth();
int getCanvasHeight();
EM_BOOL buttonPress(struct Game* game, bool state, int targetX, int targetY);

void platformInit()
{
	emscripten_set_canvas_element_size("canvas", CANVAS_WIDTH, CANVAS_HEIGHT);
	emscripten_platform.current_canvas_width = getCanvasWidth();
	emscripten_platform.current_canvas_height = getCanvasHeight();
	srand(time(NULL));
	clear(0xFF000000);
}

EM_BOOL keyDownCallback(int eventType, const EmscriptenKeyboardEvent* e,
                        void* userData)
{
	EM_BOOL handled = EM_FALSE;
	struct Game* game = (struct Game*)userData;
	if (!strcmp(e->code, "ArrowUp") || !strcmp(e->key, "k")) {
		game->rotate_right = true;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "ArrowLeft") || !strcmp(e->key, "h")) {
		game->key_left = true;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "ArrowRight") || !strcmp(e->key, "l")) {
		game->key_right = true;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "ArrowDown") || !strcmp(e->key, "j")) {
		game->key_down = true;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "Escape")) {
		emscripten_platform.is_running = false;
		handled = EM_TRUE;
	}

	else if (!strcmp(e->code, "KeyZ")) {
		game->rotate_left = true;
		handled = EM_TRUE;
	}

	else if (!strcmp(e->code, "KeyX")) {
		game->rotate_right = true;
		handled = EM_TRUE;
	}
	return handled;
}

EM_BOOL keyUpCallback(int eventType, const EmscriptenKeyboardEvent* e,
                      void* userData)
{
	EM_BOOL handled = EM_FALSE;
	struct Game* game = (struct Game*)userData;

	if (!strcmp(e->code, "ArrowLeft") || !strcmp(e->key, "h")) {
		game->key_left = false;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "ArrowRight") || !strcmp(e->key, "l")) {
		game->key_right = false;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "ArrowDown") || !strcmp(e->key, "j")) {
		game->key_down = false;
		handled = EM_TRUE;
	} else if (!strcmp(e->code, "KeyZ")) {
		game->rotate_left = false;
		handled = EM_TRUE;
	}

	else if (!strcmp(e->code, "KeyX")) {
		game->rotate_right = false;
		handled = EM_TRUE;
	}
	return handled;
}
EM_JS(int, getCanvasWidth, (), {
	let canvas = document.getElementById('canvas');
	return canvas.clientWidth;
})

EM_JS(int, getCanvasHeight, (), {
	let canvas = document.getElementById('canvas');
	return canvas.clientHeight;
})

int toBufferWidth(int x)
{
	return (x * CANVAS_WIDTH) / getCanvasWidth();
}
int toBufferHeight(int y)
{
	return (y * CANVAS_HEIGHT) / getCanvasHeight();
}

EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e,
                      void* userData)
{
	struct Game* game = (struct Game*)userData;
	bool state = eventType == EMSCRIPTEN_EVENT_MOUSEDOWN ? true : false;
	return buttonPress(game, state, e->targetX, e->targetY);
}
EM_BOOL touchEndCallback(int eventType, const EmscriptenTouchEvent* e,
                         void* userData)
{
	struct Game* game = (struct Game*)userData;
	game->key_left = false;
	game->key_right = false;
	game->key_down = false;
	game->rotate_right = false;
	game->rotate_left = false;
	return EM_TRUE;
}
EM_BOOL touchStartCallback(int eventType, const EmscriptenTouchEvent* e,
                           void* userData)
{
	struct Game* game = (struct Game*)userData;

	bool state = eventType == EMSCRIPTEN_EVENT_TOUCHSTART ? true : false;
	for (int i = 0; i < e->numTouches; i++) {
		buttonPress(game, state, e->touches[i].targetX, e->touches[i].targetY);
	}
	return EM_TRUE;
}

EM_BOOL buttonPress(struct Game* game, bool state, int targetX, int targetY)
{
	EM_BOOL handled = EM_FALSE;
	int width = 64;
	int height = 64;
	int mouseX = toBufferWidth(targetX);
	int mouseY = toBufferHeight(targetY);
	int x = mouseX - CELL_SIZE * CROSS_POSX;
	int y = mouseY - CELL_SIZE * CROSS_POSY;

	if ((x < width / 3) && (y >= height / 3) && (y < 2 * height / 3)) {
		game->key_left = state;
		handled = EM_TRUE;
	} else if ((x >= 2 * width / 3) && (y >= height / 3) &&
	           (y < 2 * height / 3)) {
		game->key_right = state;
		handled = EM_TRUE;
	} else if ((y < height / 3) && (x >= width / 3) && (x < 2 * width / 3)) {
		game->rotate_right = state;
		handled = EM_TRUE;
	} else if ((y >= 2 * height / 3) && (x >= width / 3) &&
	           (x < 2 * width / 3)) {
		game->key_down = state;
		handled = EM_TRUE;
	}
	width = 32;
	height = 32;
	x = mouseX - CELL_SIZE * BUTTONA_POSX;
	y = mouseY - CELL_SIZE * BUTTONA_POSY;
	if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
		game->rotate_right = state;
		handled = EM_TRUE;
	}
	x = mouseX - CELL_SIZE * BUTTONB_POSX;
	y = mouseY - CELL_SIZE * BUTTONB_POSY;
	if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
		game->rotate_left = state;
		handled = EM_TRUE;
	}
	return handled;
}

EM_JS(void, render,
      (const uint32_t* ptr, size_t buffer_size, int canvas_width,
       int canvas_height),
      {
	      const buff = Module.HEAPU8.subarray(ptr, ptr + (buffer_size));
	      const canvas = document.getElementById("canvas");
	      const ctx = canvas.getContext('2d');
	      const image = ctx.getImageData(0, 0, canvas_width, canvas_height);
	      image.data.set(buff);
	      ctx.putImageData(image, 0, 0);
      })

EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent* uiEvent,
                       void* userData)
{
	if (eventType == EMSCRIPTEN_EVENT_RESIZE) {
		emscripten_platform.current_canvas_width = getCanvasWidth();
		emscripten_platform.current_canvas_height = getCanvasHeight();
	}
	return EM_TRUE;
}

void drawTouchUi()
{
	renderCross(CROSS_POSX, CROSS_POSY);
	renderButton(BUTTONA_POSX, BUTTONA_POSY);
	renderButton(BUTTONB_POSX, BUTTONB_POSY);
}

void mainLoopCallback(void* arg)
{
	struct Game* game = (struct Game*)arg;
	gameUpdate(game);
	gameDrawChanges(game);
	render(render_buffer, sizeof(render_buffer), CANVAS_WIDTH, CANVAS_HEIGHT);
}

void platformLoop(struct Game* game)
{
	emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, game, 1,
	                                keyDownCallback);
	emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, game, 1,
	                              keyUpCallback);

	emscripten_set_mousedown_callback("#canvas", game, 1, mouseCallback);
	emscripten_set_mouseup_callback("#canvas", game, 1, mouseCallback);

	emscripten_set_touchstart_callback("#canvas", game, 1, touchStartCallback);
	emscripten_set_touchend_callback("#canvas", game, 1, touchEndCallback);

	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1,
	                               resizeCallback);

	emscripten_set_main_loop_arg(mainLoopCallback, game, 0, 0);

	gameDrawStatic(game);
	drawTouchUi();
}

void platformExit() {}
