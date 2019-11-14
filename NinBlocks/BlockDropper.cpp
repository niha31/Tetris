#include "BlockDropper.h"



BlockDropper::BlockDropper(SDL_Renderer* renderer, std::vector<BlockData*>* blockData, Level* level, Texture2D* tilesheet)
{
	this->renderer = renderer;
	this->blockData = blockData;
	this->level = level;
	this->tilesheet = tilesheet;

	timer = 0;

	SpawnBlock();
}


BlockDropper::~BlockDropper()
{
}

void BlockDropper::DrawDropBlock() {
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			SDL_Rect sourceRect = { 0, 0, 16, 16 };
			sourceRect.x = StaticFunctions::GetTextureOffset(data[rotationIndex][j][i] == 1 ? colourID : 0);
			if (sourceRect.x == -1) {
				continue;
			}
			SDL_Rect destRect = { (positionX + i) * TILE_WIDTH, (positionY + j) * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
			SDL_RenderCopy(renderer, this->tilesheet->GetData(), &sourceRect, &destRect);
		}
	} 
}

void BlockDropper::Update(Uint32 deltaTime) {
	if (!level->gameOver) {
		timer += deltaTime;

		if (timer > BLOCK_WAIT_TIME) {
			timer -= BLOCK_WAIT_TIME;
			UpdateDropBlock();
		}
	}
}

void BlockDropper::UpdateDropBlock() {
	positionY++;

	bool canPlace = true;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (data[rotationIndex][j][i] == 1) {
				int mainGridPosX = i + positionX;
				int mainGridPosY = j + positionY;

				//if (mainGridPosX >= 0 && mainGridPosY >= 0 && mainGridPosX < 10 && mainGridPosY < 24){
				if (level->data[mainGridPosY][mainGridPosX] > 0 || mainGridPosY > 23) {
					canPlace = false;
					goto endloop;
				}
				//}
			}
		}
	}
	endloop:

	if (canPlace) {
		return;
	}
	else {
		positionY--;
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				if (data[rotationIndex][j][i] == 1) {
					int mainGridPosX = i + positionX;
					int mainGridPosY = j + positionY;

					if (mainGridPosY < 4) {
						level->gameOver = true;
					}

					level->data[mainGridPosY][mainGridPosX] = colourID;
				}
			}
		}
		if (!level->gameOver) {
			SpawnBlock();
			level->CheckLines();
		}
	}
}

void BlockDropper::SpawnBlock() {
	rotationIndex = 0;
	positionX = 3;
	positionY = 0;
	
	int blockType = rand() % 7;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			data = blockData->at(blockType)->data;
		}
	}

	colourID = blockType + 1;
}

void BlockDropper::SpinBlockClockwise() {
	int oldRotationIndex = rotationIndex;
	rotationIndex = (rotationIndex + 1) % 4;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (data[rotationIndex][j][i] == 1) {
				int mainGridPosX = i + positionX;
				int mainGridPosY = j + positionY;

				if (mainGridPosX < 0 || mainGridPosX > 9 || mainGridPosY < 0 || mainGridPosY > 23 || level->data[mainGridPosY][mainGridPosX] > 0) {
					rotationIndex = oldRotationIndex;
					return;
				}
			}
		}
	}
}

void BlockDropper::SpinBlockAntiClockwise() {
	int oldRotationIndex = rotationIndex;
	rotationIndex--;
	if (rotationIndex == -1) rotationIndex = 3;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (data[rotationIndex][j][i] == 1) {
				int mainGridPosX = i + positionX;
				int mainGridPosY = j + positionY;

				if (mainGridPosX < 0 || mainGridPosX > 9 || mainGridPosY < 0 || mainGridPosY > 23 || level->data[mainGridPosY][mainGridPosX] > 0) {
					rotationIndex = oldRotationIndex;
					return;
				}
			}
		}
	}
}

void BlockDropper::MoveBlockLeft() {
	positionX--;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (data[rotationIndex][j][i] == 1) {
				int mainGridPosX = i + positionX;
				int mainGridPosY = j + positionY;
				if (mainGridPosX < 0 || level->data[mainGridPosY][mainGridPosX] > 0) {
					positionX++;
					return;
				}
			}
		}
	}
}

void BlockDropper::MoveBlockRight() {
	positionX++;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (data[rotationIndex][j][i] == 1) {
				int mainGridPosX = i + positionX;
				int mainGridPosY = j + positionY;
				if (mainGridPosX > 9 || level->data[mainGridPosY][mainGridPosX] > 0) {
					positionX--;
					return;
				}
			}
		}
	}
}