#pragma once

#ifndef _COMMONS_H
#define _COMMONS_H


enum BlockIDs {
	BI_EMPTY,
	BI_RED,
	BI_YELLOW,
	BI_BLUE,
	BI_GREEN,
	BI_PURPLE,
	BI_CYAN,
	BI_ORANGE
};

struct Vector2D
{
	//Vector2D
	float x;
	float y;

	float initialX;
	float initialY;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct  Rect2D
{
	float x;
	float y;
	float width;
	float height;

	Rect2D()
	{
		x = 0.0f;
		y = 0.0f;
		width = 0.0f;
		height = 0.0f;
	}

	Rect2D(float x, float y, float height, float width)
	{
		this->x = x;
		this->y = y;
		this->height = height;
		this->width = width;
	}

};

class StaticFunctions {
public:
	static int GetTextureOffset(int colourID) {
		switch (colourID) {
		case BI_EMPTY:
			return -1;
		case BI_RED:
			return 0;
		case BI_YELLOW:
			return 16;
		case BI_BLUE:
			return 32;
		case BI_GREEN:
			return 48;
		case BI_PURPLE:
			return 64;
		case BI_CYAN:
			return 80;
		case BI_ORANGE:
			return 96;
		}
	}
};
#endif
