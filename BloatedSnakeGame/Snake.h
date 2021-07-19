#pragma once

#include <windows.h>

typedef struct _Segment Segment;

struct _Segment
{
    Segment* next;
    Segment* prev;

    int x;
    int y;

    int old_x;
    int old_y;
};

typedef struct _Snake Snake;

struct _Snake
{
    Segment* head;
    Segment* tail;

    int move_x;
    int move_y;
};

Snake* InitSnake(void);
void KillSnake(Snake* snake);
void SnakeMoved(Snake* snake);
void DrawSnake(Snake* snake, HDC hdc);
void SnakeAddSegment(Snake* snake);
void SnakeSpeed(HWND hwnd, int points);
void ParseKey(Snake* snake, int key);
bool IsDead(Snake* snake, int left, int top, int right, int bottom);