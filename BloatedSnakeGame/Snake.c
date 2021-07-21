#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "Snake.h"
#include "Draw.h"

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

struct _Snake
{
    Segment* head;
    Segment* tail;

    int move_x;
    int move_y;

    int radius;
    int size;
};

Segment* AddSegment(Segment* older, int x, int y)
{
    Segment* newer = (Segment*)malloc(sizeof(Segment));
    newer->next = NULL;
    newer->prev = NULL;

    newer->old_x = 0;
    newer->old_y = 0;

    if (older != NULL)
    {
        newer->prev = older;
        older->next = newer;

        newer->x = older->old_x;
        newer->y = older->old_y;
    }
    else
    {
        newer->x = x;
        newer->y = y;
    }

    return newer;
}

Snake* InitSnake(int radius)
{
    Snake* snake = (Snake*)malloc(sizeof(Snake));

    snake->head = AddSegment(NULL, 0, 0);
    snake->tail = snake->head;

    snake->move_x = 0;
    snake->move_y = 0;

    snake->radius = radius;
    snake->size = snake->radius * 2 + 1;

    return snake;
}

void SnakeSetStart(Snake *snake, int start_x, int start_y)
{
    snake->head->x = start_x;
    snake->head->y = start_y;
}

void KillSnake(Snake* snake)
{
    Segment* current = snake->head;

    while (current != NULL)
    {
        Segment* next = current->next;
        free(current);
        current = next;
    }

    free(snake);
}

void SnakeAddSegment(Snake* snake)
{
    Segment* tail = AddSegment(snake->tail, 0, 0);
    snake->tail = tail;
}

void DrawSnake(Snake *snake, DrawEllipseFunc de)
{
    Segment* current = snake->head;

    Color rgb = { 0, 255, 0 };

    while (current != NULL)
    {
        de(current->x, current->y, current->x + snake->size, current->y + snake->size, rgb);
        current = current->next;
    }
}

void SnakeMove(Snake* snake, SnakeMovement move)
{
    snake->head->old_x = snake->head->x;
    snake->head->old_y = snake->head->y;

    switch (move)
    {
        case MOVE_LEFT:
            snake->head->x -= snake->radius;
            snake->move_x = -snake->radius;
            break;
        case MOVE_RIGHT:
            snake->head->x += snake->radius;
            snake->move_x = snake->radius;
            break;
        case MOVE_UP:
            snake->head->y -= snake->radius;
            snake->move_y = -snake->radius;
            break;
        case MOVE_DOWN:
            snake->head->y += snake->radius;
            snake->move_y = snake->radius;
            break;
        case MOVE_STILL:
            snake->head->x += snake->move_x;
            snake->head->y += snake->move_y;
            break;
    }
}

bool IsSnakeMoved(Snake* snake)
{
    if (snake->head->old_x == snake->head->x &&
        snake->head->old_y == snake->head->y)
        return false;

    return true;
}

void SnakeMoved(Snake* snake)
{
    Segment* current = snake->head->next;

    while (current != NULL)
    {
        current->old_x = current->x;
        current->old_y = current->y;

        current->x = current->prev->old_x;
        current->y = current->prev->old_y;

        current = current->next;
    }
}

bool IsDead(Snake* snake, int left, int top, int right, int bottom)
{
    if (snake->head->x > right - snake->size || snake->head->x < left ||
        snake->head->y > bottom - snake->size || snake->head->y < top)
        return true;

    Segment* current = snake->head->next;

    while (current != NULL)
    {
        if (snake->head->x == current->x && snake->head->y == current->y)
            return true;

        current = current->next;
    }

    return false;
}

void SnakeGetHeadXY(Snake* snake, int* x, int* y)
{
    *x = snake->head->x;
    *y = snake->head->y;
}