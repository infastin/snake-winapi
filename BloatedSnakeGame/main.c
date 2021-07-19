#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "resource.h"
#include "Food.h"
#include "Snake.h"

const char AppWindowName[] = "brrr";

int points = 0;
int move_key = 0;

Snake* snake;

#define LEFT 10
#define TOP 10
#define RIGHT 271
#define BOTTOM 261

void DrawBorders(HDC hdc)
{
    Rectangle(hdc, LEFT, TOP, RIGHT, BOTTOM);
}

void DrawPoints(HDC hdc)
{
    wchar_t points_str[256];
    wsprintf(points_str, L"Points: %d", points);
    TextOut(hdc, 10, 265, points_str, lstrlen(points_str));
}

void restart()
{
    KillSnake(snake);
    snake = InitSnake();
    points = 0;
    move_key = 0;
    FoodReset();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg)
    {
        case WM_CREATE:
            snake = InitSnake();
            SnakeSpeed(hwnd, points);
            break;
        case WM_TIMER:
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        case WM_KEYDOWN:
            move_key = wParam;
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        case WM_CLOSE:
            KillTimer(hwnd, 1);
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            DrawBorders(hdc);
            SnakeSpeed(hwnd, points);

            if (FoodExists() == false)
                RandFood(LEFT, TOP, RIGHT, BOTTOM);

            DrawFood(hdc);
            DrawSnake(snake, hdc);
            DrawPoints(hdc);

            if (move_key == 0)
                break;

            ParseKey(snake, move_key);

            if (IsDead(snake, LEFT, TOP, RIGHT, BOTTOM) == true)
            {
                KillTimer(hwnd, 1);
                int id = MessageBox(hwnd, L"Want to retry?", L"Game Over", MB_YESNO | MB_ICONINFORMATION);

                if (id == IDNO)
                {
                    PostQuitMessage(0);
                }
                else
                {
                    restart();
                    SnakeSpeed(hwnd, points);

                    break;
                }
            }

            SnakeMoved(snake);

            if (FoodEaten(snake->head->x, snake->head->y) == true)
            {
                SnakeAddSegment(snake);
                FoodReset();
                points++;
            }

            EndPaint(hwnd, &ps);
            break;
        default:
           return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = AppWindowName;
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CONTROLPARENT | WS_EX_APPWINDOW,
        AppWindowName,
        L"Bloated Snake Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 330,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}