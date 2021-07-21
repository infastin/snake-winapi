#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "resource.h"
#include "Game.h"
#include "Draw.h"

const char AppWindowName[] = "brrr";

Game* game;
HWND hwnd;
HDC hdc;
PAINTSTRUCT ps;
SnakeMovement move = MOVE_STILL;

void LoseMessage(wchar_t *title, wchar_t *text)
{
    KillTimer(hwnd, 1);
    int id = MessageBox(hwnd, title, text, MB_YESNO | MB_ICONINFORMATION);

    if (id == IDNO)
        PostQuitMessage(0);
    else
    {
        RestartGame(game);
        move = MOVE_STILL;
        int cpuTime = GameSpeed(game);
        SetTimer(hwnd, 1, cpuTime, NULL);
    }
}

void DrawEllipse(int left, int top, int right, int bottom, Color rgb)
{
    HBRUSH hbrush = CreateSolidBrush(RGB(rgb.red, rgb.green, rgb.blue));
    SelectObject(hdc, hbrush);
    Ellipse(hdc, left, top, right, bottom);
    DeleteObject(hbrush);
}

void DrawRectangle(int left, int top, int right, int bottom, Color rgb)
{
    HBRUSH hbrush = CreateSolidBrush(RGB(rgb.red, rgb.green, rgb.blue));
    SelectObject(hdc, hbrush);
    Rectangle(hdc, left, top, right, bottom);
    DeleteObject(hbrush);
}

void DrawTextOut(int x, int y, wchar_t* str, int len)
{
    TextOut(hdc, x, y, str, len);
}

SnakeMovement ParseKey(int key)
{
    SnakeMovement move;

    switch (key)
    {
        case VK_LEFT:
            move = MOVE_LEFT;
            break;
        case VK_RIGHT:
            move = MOVE_RIGHT;
            break;
        case VK_UP:
            move = MOVE_UP;
            break;
        case VK_DOWN:
            move = MOVE_DOWN;
            break;
        default:
            move = MOVE_STILL;
            break;
    }

    return move;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int cpuTime;

    switch (msg)
    {
        case WM_CREATE:
            game = InitGame(400, 430, 5);
            cpuTime = GameSpeed(game);
            SetTimer(hwnd, 1, cpuTime, NULL);

            break;
        case WM_TIMER:
            cpuTime = GameSpeed(game);
            SetTimer(hwnd, 1, cpuTime, NULL);

            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        case WM_KEYDOWN:
            move = ParseKey(wParam);
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

            PlayGame(game, DrawRectangle, DrawEllipse, DrawTextOut, move, LoseMessage);

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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 430,
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