#pragma once

#include "Food.h"
#include "Snake.h"

typedef void (LoseFunc)(wchar_t* title, wchar_t* text);

typedef struct _Game Game;

Game* InitGame(int width, int height, int radius);
int GameSpeed(Game* game);
void PlayGame(Game* game, DrawRectangleFunc dr, DrawEllipseFunc de, DrawTextOutFunc dto, SnakeMovement move, LoseFunc l);
void RestartGame(Game* game);