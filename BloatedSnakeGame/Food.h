#pragma once

#include <Windows.h>
#include <stdbool.h>

extern int food_x;
extern int food_y;

void FoodReset(void);
bool FoodExists(void);
bool FoodEaten(int head_x, int head_y);
void RandFood(int left, int top, int right, int bottom);
void DrawFood(HDC hdc);