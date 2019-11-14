
#include <SDL.h>
#include <SDl_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

#include "Constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include "Level.h"
#include "BlockData.h"
#include "BlockDropper.h"


//Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
Uint32 gOldTime;
Mix_Music* gMusic = nullptr;
Level* gLevel = nullptr;
std::vector<BlockData*> gBlockData;
Texture2D* gTilesheet = nullptr;
Texture2D* gPausedTexture = nullptr;
Texture2D* gNumbersTexture = nullptr;
Texture2D* gScoreTexture = nullptr;
Texture2D* gGameOverTexture = nullptr;
BlockDropper* gBlockDropper = nullptr;
Uint32 gDeltaTime;
bool gPaused = false;
unsigned int gScore = 0;

bool InitSDL();
void CloseSDL();
bool Update();
void Render();
void LoadMusic(string path);
void DrawPaused();
void DrawNumbers(unsigned int number, SDL_Point topLeft);
void DrawScore();
void DrawGameOver();

int main(int argc, char* args[])
{
	srand(time(NULL));

	//check if SDL was set up correctly
	if (InitSDL())
	{
		//load music
		LoadMusic("Music/Tetris 99 - Main Theme.mp3");

		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(gMusic, -1);
		}

		//Flag to check if we wish to quit
		bool quit = false;

		gTilesheet = new Texture2D(gRenderer);
		gTilesheet->LoadFromFile("Images/tilesheet.png");

		gLevel = new Level(gTilesheet, gRenderer, &gScore);
		//gLevel->Randomize();

		gBlockData.push_back(new BlockData("L"));
		gBlockData.push_back(new BlockData("reverse L"));
		gBlockData.push_back(new BlockData("line"));
		gBlockData.push_back(new BlockData("zed"));
		gBlockData.push_back(new BlockData("reverse zed"));
		gBlockData.push_back(new BlockData("tee"));
		gBlockData.push_back(new BlockData("square"));

		gBlockDropper = new BlockDropper(gRenderer, &gBlockData, gLevel, gTilesheet);

		gPausedTexture = new Texture2D(gRenderer);
		gPausedTexture->LoadFromFile("Images/PAUSED.png");

		gNumbersTexture = new Texture2D(gRenderer);
		gNumbersTexture->LoadFromFile("Images/numbers.png");

		gScoreTexture = new Texture2D(gRenderer);
		gScoreTexture->LoadFromFile("Images/SCORE.png");

		gGameOverTexture = new Texture2D(gRenderer);
		gGameOverTexture->LoadFromFile("Images/GameOver.png");
		

		//Game loop
		while (!quit)
		{
			quit = Update();	
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(gRenderer);
			Render();
			SDL_RenderPresent(gRenderer);
		}

		gTilesheet->Free();

		for (int i = 0; i < gBlockData.size(); i++) {
			delete gBlockData.at(i);
		}

		delete gLevel;
		delete gBlockDropper;
	}

	return true;
	
	
	gOldTime = SDL_GetTicks();

	CloseSDL();

	return 0;
}

bool InitSDL()
{
	//set up SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		//All good, so attempt to create the window
		gWindow = SDL_CreateWindow("NinBlocks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		//Did the window get created
		if (gWindow == NULL)
		{
			//Nope
			cout << "Window was not created. Error:" << SDL_GetError();
			return false;
		}
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer != NULL)
		
	{
		//Initialise PNG loading
		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
			cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
			return false;
		}
		else
		{
			return true;
		}
	}

	//initialise the Mixer
	if (SDL_Init(SDL_INIT_AUDIO) < 0 )
	{
		cout << "Mixer could not initialise. Error: " << Mix_GetError() << endl;
		return false;
	}
}

void Render()
{
	gLevel->Draw();
	gBlockDropper->DrawDropBlock();
	if (gPaused) DrawPaused();
	DrawNumbers(gScore, { 10, 32 });
	DrawScore();
	if (gLevel->gameOver) DrawGameOver();
}

void CloseSDL()
{
	//Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Release the renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//destroy the game screen manager

	//Release music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	//get the new time
	Uint32 newTime = SDL_GetTicks();
	gDeltaTime = newTime - gOldTime;

	//Event handler
	SDL_Event e;

	//Get events
	SDL_PollEvent(&e);

	//Handle any event
	switch (e.type)
	{
		//Click the 'X' to quit
	case SDL_QUIT:
		return true;
		break;
		
		case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
			case SDLK_r:
				if (gLevel->gameOver) {
					gLevel->ResetLevel();
					gBlockDropper->SpawnBlock();
				}
				break;
			case SDLK_q:
				if (!gPaused) gBlockDropper->SpinBlockAntiClockwise();
				break;
			case SDLK_e:
				if (!gPaused) gBlockDropper->SpinBlockClockwise();
				break;
			case SDLK_LEFT:
				if (!gPaused) gBlockDropper->MoveBlockLeft();
				break;
			case SDLK_RIGHT:
				if (!gPaused) gBlockDropper->MoveBlockRight();
				break;
			case SDLK_DOWN:
				if (!gPaused) gBlockDropper->UpdateDropBlock();
				break;
			default:
				break;
		case SDLK_SPACE:
			if (gPaused)
				gPaused = false;
			else
				gPaused = true;
		}

		break;
		
	}

	if (!gPaused) gBlockDropper->Update(gDeltaTime);

	DrawNumbers(gScore, { 10,10 });

	//set the current time to be the old time
	gOldTime = newTime;
	return false;
}

void LoadMusic(string path)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2448);
	gMusic = Mix_LoadMUS(path.c_str());

	if (gMusic == NULL)
	{
		cout << "Failed to load background music! Error: " << Mix_GetError() << endl;
	}
}

void DrawPaused() {
	SDL_Rect src = { 0, 0, 160, 32 };
	SDL_Rect dst = { 80, 384, 160, 32 };
	SDL_RenderCopy(gRenderer, gPausedTexture->GetData(), &src, &dst);
}

void DrawNumbers(unsigned int number, SDL_Point topLeft) {
	std::string numberAsString = std::to_string(number);
	unsigned int strLength = numberAsString.length();
	
	for (int i = 0; i < strLength; i++) {
		unsigned int srcX = numberAsString[i] - '0';
		SDL_Rect src = { srcX * 20, 0, 20, 32 };
		SDL_Rect dst = { topLeft.x + 20 * i, topLeft.y, 20, 32 };
		SDL_RenderCopy(gRenderer, gNumbersTexture->GetData(), &src, &dst);
	}
}

void DrawScore()
{
	SDL_Rect src = { 0, 0, 100, 32 };
	SDL_Rect dst = { 0, 0, 100, 32 };
	SDL_RenderCopy(gRenderer, gScoreTexture->GetData(), &src, &dst);
}

void DrawGameOver()
{
	SDL_Rect src = { 0, 0, 200, 32 };
	SDL_Rect dst = { 60, 384, 200, 32 };
	SDL_RenderCopy(gRenderer, gGameOverTexture->GetData(), &src, &dst);
}