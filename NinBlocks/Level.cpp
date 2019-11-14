#include "Level.h"



Level::Level(Texture2D* tilesheet, SDL_Renderer* renderer, unsigned int* gScore) {
	this->renderer = renderer;
	this->tilesheet = tilesheet;
	this->gScore = gScore;
	gameOver = false;
}

Level::~Level() {

}

void Level::Randomize() {
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 10; i++) {
			data[j + 15][i] = rand() % 8;
		}
	}
}

void Level::Draw() {
	//DrawBack();
	for (int j = 0; j < 24; j++) {
		for (int i = 0; i < 10; i++) {
			SDL_Rect sourceRect = { 0, 0, 16, 16 };
			sourceRect.x = StaticFunctions::GetTextureOffset(data[j][i]);
			if (sourceRect.x == -1) {
				if (j < 4) sourceRect.x = 112;
				else continue;
			}
			SDL_Rect destRect = {i * TILE_WIDTH, j * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
			SDL_RenderCopy(renderer, this->tilesheet->GetData(), &sourceRect, &destRect);
		}
	}
}

void Level::CheckLines() {

	std::vector<int> rowsToRemove;

	for (int j = 0; j < 24; j++) {
		bool fullLine = true;

		for (int i = 0; i < 10; i++) {
			if (data[j][i] == 0) {
				fullLine = false;
			}
		}

		if (fullLine) {
			rowsToRemove.push_back(j);
		}
	}

	for (int k = 0; k < rowsToRemove.size(); k++) {
		(*gScore) += 10;
		for (int j = 23; j >= 0; j--) {
			for (int i = 0; i < 10; i++) {
				if (j < rowsToRemove.at(k)) {
					data[j + 1][i] = data[j][i];
				}
			}
		}
	}
}

void Level::DrawBack() {
	SDL_Rect sourceRect = { 0, 0, 16, 16 };
	SDL_Rect destRect = { 0, 0, TILE_WIDTH * 10 + 4, TILE_HEIGHT * 4 };
	SDL_RenderCopy(renderer, this->tilesheet->GetData(), &sourceRect, &destRect);
}

void Level::ResetLevel() {
	gameOver = false;

	(*gScore) = 0;

	for (int j = 0; j < 24; j++) {
		for (int i = 0; i < 10; i++) {
			data[j][i] = 0;
		}
	}
}