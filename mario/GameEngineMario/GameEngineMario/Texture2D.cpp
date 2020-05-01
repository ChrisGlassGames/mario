#include "SDL.h"
#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>
#include "Commons.h"
using namespace::std;


Texture2D::Texture2D(SDL_Renderer* renderer)
{
	mRenderer = renderer;
	mTexture = NULL;

	mWidth = NULL;
	mHeight = NULL;
}

Texture2D::~Texture2D()
{
	Free();

	mRenderer = NULL;
}

bool Texture2D::LoadFromFile(string path)
{
	//Remove the memory used for a previous texture
	Free();

	//SDL_Texture* mTexture = NULL; //THIS IS A BAD BOY LINE, TOOK 2 HOURS OF MY LIFE 
	/*mTexture = nullptr;*/

	//Load the image.
	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface != NULL)
	{
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));
		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);

		//set dimensions
		mWidth = pSurface->w;
		mHeight = pSurface->h;

		if (mTexture == NULL)
		{
			cout << "Unable to create texture from surface. Error: " << SDL_GetError() << endl;
		}

		SDL_FreeSurface(pSurface);
	}
	else
	{
		cout << "Unable to create texture from surface. Error: " << SDL_GetError() << endl;
	}
	return mTexture != NULL;
}

int Texture2D::GetWidth() { return mWidth; }
int Texture2D::GetHeight() { return mHeight; }

void Texture2D::Free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture2D::Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle)
{
	SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &destRect, angle, nullptr, flip);
}



void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle)
{
	////Clear the screen
	//SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
	//SDL_RenderClear(mRenderer);

	//Select where to render texture
	SDL_Rect renderLocation = { newPosition.x, newPosition.y, mWidth, mHeight};

	//Render to screen
	SDL_RenderCopyEx(mRenderer, mTexture, NULL, &renderLocation, 0, NULL, flip);

	////Update the screen
	//SDL_RenderPresent(mRenderer);

}