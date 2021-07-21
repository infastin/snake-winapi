#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Food.h"

struct _Food
{
    int x;
    int y;
    int radius;
    int size;
};

Food* InitFood(int radius)
{
    Food* food = (Food*)malloc(sizeof(Food));

    food->x = 0;
    food->y = 0;
    food->radius = radius;
    food->size = radius * 2 + 1;

    return food;
}

void FoodReset(Food* food)
{
    food->x = 0;
    food->y = 0;
}

bool FoodExists(Food* food)
{
    if (food->x != 0 && food->y != 0)
        return true;

    return false;
}

bool FoodEaten(Food* food, int x, int y)
{
    int eat_radius = food->radius + 3;

    if ((x - food->x) <= eat_radius && (x - food->x) >= -eat_radius &&
        (y - food->y) <= eat_radius && (y - food->y) >= -eat_radius)
        return true;

    return false;
}

void RandFood(Food* food, int left, int top, int right, int bottom)
{
    srand(time(0));

    int width = right - left;
    int height = bottom - top;

    food->x = (rand() % (width - 2 * food->radius)) + left;
    food->y = (rand() % (height - 2 * food->radius)) + top;
}

void DrawFood(Food* food, DrawEllipseFunc de)
{
    Color rgb = { 255, 255, 48 };
    de(food->x, food->y, food->x + food->size, food->y + food->size, rgb);
}