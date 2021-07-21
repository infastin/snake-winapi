#pragma once

#include <wchar.h>

typedef struct _Color Color;

struct _Color
{
	int red;
	int green;
	int blue;
};

typedef void (*DrawEllipseFunc)(int left, int top, int right, int bottom, Color rgb);
typedef void (*DrawRectangleFunc)(int left, int top, int right, int bottom, Color rgb);
typedef void (*DrawTextOutFunc)(int x, int y, wchar_t* str, int len);