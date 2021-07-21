#pragma once

#include <stdbool.h>

#include "Draw.h"

typedef enum
{
	MOVE_LEFT,
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_STILL
} SnakeMovement;

typedef struct _Snake Snake;

Snake* InitSnake(int radius);
void SnakeSetStart(Snake* snake, int start_x, int start_y);
void KillSnake(Snake* snake);
void SnakeMoved(Snake* snake);
void DrawSnake(Snake* snake, DrawEllipseFunc de);
void SnakeAddSegment(Snake* snake);
void SnakeMove(Snake* snake, SnakeMovement move);
bool IsDead(Snake* snake, int left, int top, int right, int bottom);
bool IsSnakeMoved(Snake* snake);
void SnakeGetHeadXY(Snake* snake, int* x, int* y);