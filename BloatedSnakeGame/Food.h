#pragma once

#include <stdbool.h>
#include "Draw.h"

typedef struct _Food Food;

Food* InitFood(int radius);
void FoodReset(Food* food);
bool FoodExists(Food* food);
bool FoodEaten(Food* food, int x, int y);
void RandFood(Food* food, int left, int top, int right, int bottom);
void DrawFood(Food* food, DrawEllipseFunc de);