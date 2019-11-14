#include "Texture2D.h"
#include "Commons.h"
#include <SDL_image.h>
#include <iostream>
using namespace::std;

//constructor
Texture2D::Texture2D(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}


//deconstructor
Texture2D::~Texture2D()
{
	//free up the memory
	Free();

	mRenderer = NULL;
}

bool Texture2D::LoadFromFile(string path)
{
	//Remove the memory used for a previous texture
	//FreeTexture();

	mTexture = NULL;

	//Load image
	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface != NULL)
	{
		//set the dimentions
		mWidth = pSurface->w;
		mHeight = pSurface->h;

		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		if (mTexture == NULL)
		{
			cout << "Unable to create texture from surface. Error: " << SDL_GetError() << endl;
		}

		
		//colour key the image - the colour to be trasparent
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));
		SDL_FreeSurface(pSurface);
	}
	else
	{
		cout << "unable to create texture from surface. Error: " << IMG_GetError() << endl;
	}

	return mTexture != NULL;
}

void Texture2D::Free()
{
	if (mTexture = NULL)
	{
		SDL_DestroyTexture(mTexture);
		mWidth = 0.0f;
		mHeight = 0.0f;
	}
}

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle)
{
	//clear the screen - Black
	SDL_Rect sourceRect = { 0,0,mWidth, mHeight };
	SDL_Rect destRect = { newPosition.x, newPosition.y, mWidth, mHeight };
	SDL_Point centre = { mWidth * 0.5f,mHeight * 0.5f };
	SDL_RenderCopyEx(mRenderer, mTexture, &sourceRect, &destRect, angle, &centre, flip);
}

void Texture2D::Render(SDL_Rect scrRect, SDL_Rect desRect, SDL_RendererFlip flip, double angle)
{
	SDL_Point centre = {0,0};
	SDL_RenderCopyEx(mRenderer, mTexture, &scrRect, &desRect, angle, &centre, flip);
}
