#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "Snake.h"
#include "Game.h"

struct _Game
{
    Snake* snake;
    Food* food;
    
    int points;
    int left;
    int top;
    int right;
    int bottom;
    int radius;
};

Game* InitGame(int width, int height, int radius)
{
	Game* game = (Game*)malloc(sizeof(Game));

	game->snake = InitSnake(radius);
    game->food = InitFood(radius);
	game->points = 0;

    game->left = radius * 2;
    game->top = radius * 2;
    game->radius = radius;

    int start_x = ((((width - game->left * 2) / 2) - 1) / (radius * 2)) * (radius * 2);
    int start_y = ((((height - game->top * 6) / 2) - 1) / (radius * 2)) * (radius * 2);

    SnakeSetStart(game->snake, start_x, start_y);

    game->right = start_x * 2 + 1;
    game->bottom = start_y * 2 + 1;

    return game;
}

int GameSpeed(Game* game)
{
    int cpuTime;

    if (game->points > 30)
        cpuTime = 20;
    else if (game->points > 20)
        cpuTime = 25;
    else if (game->points > 14)
        cpuTime = 30;
    else if (game->points > 9)
        cpuTime = 40;
    else if (game->points > 4)
        cpuTime = 50;
    else
        cpuTime = 60;

    return cpuTime;
}

void GameMove(Game* game, SnakeMovement move)
{
    SnakeMove(game->snake, move);
}

void DrawGameBorders(Game* game, DrawRectangleFunc dr)
{
    Color rgb = { 255, 255, 255 };
    dr(game->left, game->top, game->right, game->bottom, rgb);
}

void DrawGamePoints(Game* game, DrawTextOutFunc dto)
{
    wchar_t points_str[256];
    swprintf(points_str, 256, L"Points: %d", game->points);
    dto(game->left, game->bottom + 4, points_str, wcslen(points_str));
}

void RestartGame(Game* game)
{
    KillSnake(game->snake);
    game->snake = InitSnake(game->radius);
    game->points = 0;
    FoodReset(game->food);

    int start_x = (game->right - 1) / 2;
    int start_y = (game->bottom - 1) / 2;

    SnakeSetStart(game->snake, start_x, start_y);

    return GameSpeed(game);
}

void PlayGame(Game* game, DrawRectangleFunc dr, DrawEllipseFunc de, DrawTextOutFunc dto, SnakeMovement move, LoseFunc l)
{
    DrawGameBorders(game, dr);

    if (FoodExists(game->food) == false)
        RandFood(game->food, game->left, game->top, game->right, game->bottom);
    
    DrawFood(game->food, de);
    DrawSnake(game->snake, de);
    DrawGamePoints(game, dto);
    SnakeMove(game->snake, move);
    
    if (!IsSnakeMoved(game->snake))
        return;

    if (IsDead(game->snake, game->left, game->top, game->right, game->bottom) == true)
    {
        l(L"Game Over", L"Want to retry?");
        return;
    }

    SnakeMoved(game->snake);

    int head_x, head_y;
    SnakeGetHeadXY(game->snake, &head_x, &head_y);

    if (FoodEaten(game->food, head_x, head_y) == true)
    {
        SnakeAddSegment(game->snake);
        FoodReset(game->food);
        game->points++;
    }
}