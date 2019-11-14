#pragma once

#include "Texture2D.h"
#include "Constants.h"

#include <vector>

class Level
{
private:
	Texture2D* tilesheet = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::string sheetFilePath;

public:
	unsigned int* gScore;
	int data[24][10];
	bool gameOver;

	Level(Texture2D* tilesheet, SDL_Renderer* renderer, unsigned int* gScore);
	~Level();

	void Draw();
	void DrawBack();
	void Randomize();
	void CheckLines();
	void ResetLevel();
};

