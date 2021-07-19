#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Food.h"

#define FOOD_RADIUS 5
#define FOOD_SIZE 11
#define EAT_RADIUS (FOOD_RADIUS + 3)

static int food_x = 0;
static int food_y = 0;

void FoodReset(void)
{
    food_x = 0;
    food_y = 0;
}

bool FoodExists(void)
{
    if (food_x != 0 && food_y != 0)
        return true;

    return false;
}

bool FoodEaten(int head_x, int head_y)
{
    if ((head_x - food_x) <= EAT_RADIUS && (head_x - food_x) >= -EAT_RADIUS &&
        (head_y - food_y) <= EAT_RADIUS && (head_y - food_y) >= -EAT_RADIUS)
        return true;

    return false;
}

void RandFood(int left, int top, int right, int bottom)
{
    srand(time(0));

    int width = right - left;
    int height = bottom - top;

    food_x = (rand() % (width - 2 * FOOD_RADIUS)) + left;
    food_y = (rand() % (height - 2 * FOOD_RADIUS)) + top;
}

void DrawFood(HDC hdc)
{
    HBRUSH hbrush = CreateSolidBrush(RGB(225, 255, 48));
    SelectObject(hdc, hbrush);
    Ellipse(hdc, food_x, food_y, food_x + FOOD_SIZE, food_y + FOOD_SIZE);
    DeleteObject(hbrush);
}