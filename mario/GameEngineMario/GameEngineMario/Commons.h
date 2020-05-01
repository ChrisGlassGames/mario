#pragma once
#ifndef _COMMONS_H
struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float xTemp, float yTemp)
	{
		x = xTemp;
		y = yTemp;
	}
};


struct Rect2D
{
	float Width;
	float Height;
	float x;
	float y;

	//Rect2D()
	//{
	//	x = 0.0f;
	//	y = 0.0f;
	//	Width = 1.0f;
	//	Height = 1.0f;
	//}

	Rect2D(float xTemp, float yTemp, float w, float h)
	{
		x = xTemp;
		y = yTemp;
		Width = w;
		Height = h;
	}
};


enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT
};
#endif