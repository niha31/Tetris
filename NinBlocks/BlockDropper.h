#pragma once

#include "BlockData.h"
#include "Level.h"

#include <SDL.h>
#include <vector>

class BlockDropper
{
private:
	std::vector<BlockData*>* blockData;
	int*** data;
	int colourID;
	int positionX, positionY;
	int rotationIndex;

	SDL_Renderer* renderer;
	Texture2D* tilesheet = nullptr;
	Level* level;

	Uint32 timer;

public:
	BlockDropper(SDL_Renderer* renderer, std::vector<BlockData*>* blockData, Level* level, Texture2D* tilesheet);
	~BlockDropper();

	void Update(Uint32 deltaTime);
	void DrawDropBlock();
	void UpdateDropBlock();
	void SpinBlockClockwise();
	void SpinBlockAntiClockwise();
	void MoveBlockLeft();
	void MoveBlockRight();
	void SpawnBlock();
};

