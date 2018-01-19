#ifndef PLATFORM_H
#define PLATFORM_H

#include <game.h>

void platformInit();
// void getScreenWidth();
// void getScreenHeight();
void platformLoop(struct Game *game);
void platformExit();

#endif
