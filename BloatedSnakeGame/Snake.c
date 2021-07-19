#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "Snake.h"

#define SNAKE_RADIUS 5
#define SNAKE_SIZE 11
#define MOVE_DISTANCE 5
#define DEATH_RADIUS (SNAKE_RADIUS + 2)

Segment* AddSegment(Segment* older, int x, int y)
{
    Segment* newer = malloc(sizeof(Segment));
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

Snake* InitSnake(void)
{
    Snake* snake = malloc(sizeof(Snake));

    snake->head = AddSegment(NULL, 130, 120);
    snake->tail = snake->head;

    snake->move_x = 0;
    snake->move_y = 0;

    return snake;
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

void DrawSnake(Snake *snake, HDC hdc)
{
    HBRUSH hbrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, hbrush);

    Segment* current = snake->head;

    while (current != NULL)
    {
        Ellipse(hdc, current->x, current->y, current->x + SNAKE_SIZE, current->y + SNAKE_SIZE);
        current = current->next;
    }
    DeleteObject(hbrush);
}

void SnakeSpeed(HWND hwnd, int points)
{
    int cpuTime;

    if (points > 30)
        cpuTime = 20;
    else if (points > 20)
        cpuTime = 25;
    else if (points > 14)
        cpuTime = 30;
    else if (points > 9)
        cpuTime = 40;
    else if (points > 4)
        cpuTime = 50;
    else
        cpuTime = 60;

    SetTimer(hwnd, 1, cpuTime, NULL);
}

void ParseKey(Snake *snake, int key)
{
    snake->head->old_x = snake->head->x;
    snake->head->old_y = snake->head->y;

    switch (key)
    {
        case VK_LEFT:
            snake->head->x -= MOVE_DISTANCE;
            snake->move_x = -MOVE_DISTANCE;
            break;
        case VK_RIGHT:
            snake->head->x += MOVE_DISTANCE;
            snake->move_x = MOVE_DISTANCE;
            break;
        case VK_UP:
            snake->head->y -= MOVE_DISTANCE;
            snake->move_y = -MOVE_DISTANCE;
            break;
        case VK_DOWN:
            snake->head->y += MOVE_DISTANCE;
            snake->move_y = MOVE_DISTANCE;
            break;
        default:
            snake->head->x += snake->move_x;
            snake->head->y += snake->move_y;
            break;
    }
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
    if (snake->head->x > right - SNAKE_SIZE + 1 || snake->head->x < left ||
        snake->head->y > bottom - SNAKE_SIZE + 1 || snake->head->y < top)
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