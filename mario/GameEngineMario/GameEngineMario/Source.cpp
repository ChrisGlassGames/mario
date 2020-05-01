#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include "GameScreenManager.h"


using namespace::std;

//Globals
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
GameScreenManager* gameScreenManager;
Uint32 gOldTime;
Mix_Music* gMusic = NULL;
//Texture2D* gTexture = NULL;

bool InitSDL();
void CloseSDL();
bool Update();
void Render();
void LoadMusic(string path);
//SDL_Texture* LoadTextureFromFile(string path);
//void FreeTexture();

bool closeGame = false;

int main(int argc, char* args[])
{

	if (InitSDL())
	{

		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(gMusic, -1);
		}

		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_MENU);
		gOldTime = SDL_GetTicks();
		bool quit = false;
		while (!quit && !closeGame)
		{
			Render();
			quit = Update();
		}

		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_LEVEL1);
		quit = false;
		while (!quit && !closeGame)
		{
			Render();
			quit = Update();
		}

		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_LEVEL2);
		quit = false;
		while (!quit && !closeGame)
		{
			Render();
			quit = Update();
		}

	}

	//Setup the game screen manager




	CloseSDL();
	return 0;
}

bool InitSDL()
{
	//Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDl did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		//All gooooooooood, create da windo
		gWindow = SDL_CreateWindow("Games Engine Creation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);


		//Did the window get created?
		if (gWindow == NULL)
		{
			//Nay
			cout << "Window was not creatd. Error: " << SDL_GetError();
			return false;
		}
		if (gRenderer != NULL)
		{
			//initialise PNG Loading
			int imageFlags = IMG_INIT_PNG;

			if (!(IMG_Init(imageFlags) & imageFlags))
			{
				cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
				return false;
			}
			//Load background texture
			//gTexture = new Texture2D(gRenderer);
			//if (!gTexture->LoadFromFile("Images/test.bmp"))
			//{
			//	return false;
			//}

			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialise! SDL_ttf Error: \n", TTF_GetError());
				return false;
			}

			//Initialise the Mixer.
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				cout << "Mixer could not initialise. Error: " << Mix_GetError();
				return false;
			}

		}
		else
		{
			cout << "Renderer could not initialise. Error: " << SDL_GetError();
			return false;
		}
	}
	return true;
}

void CloseSDL()
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	//Clear up the texture
	//gTexture->Free();

	//release the renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	//release the texture
	//delete gTexture;
	//gTexture = NULL;

	//Destroy the game screen manager.
	delete gameScreenManager;
	gameScreenManager = nullptr;

	//Release music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
}

bool Update()
{
	//Event Handler
	SDL_Event e;

	//Get events
	SDL_PollEvent(&e);



	Uint32 newTime = SDL_GetTicks();

	switch (e.type)
	{
		//Click on the 'X' to quit.
	case SDL_QUIT:
		closeGame = true;
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_q:
			closeGame = true;
			break;
		case SDLK_ESCAPE:
			closeGame = true;
			break;
		case SDLK_RETURN:
			Mix_HaltMusic();
			Mix_HaltChannel(-1);
			return true;
			break;
		}
	}
	gameScreenManager->Update((float)(newTime - gOldTime) / 1000.0f, e);
	gOldTime = newTime;
	return false;
}

void Render()
{
	//clear the screen - black
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);

	//gTexture->Render(Vector2D(), SDL_FLIP_NONE);
	gameScreenManager->Render();

	//update the screen
	SDL_RenderPresent(gRenderer);


}

void LoadMusic(string path)
{
	gMusic = Mix_LoadMUS(path.c_str());

	if (gMusic == NULL)
	{
		cout << "Failed to load background music! Error: " << Mix_GetError() << endl;
	}
}
//
//SDL_Texture* LoadTextureFromFile(string path)
//{
//	//Remove the memory used for a previous texture
//	FreeTexture();
//
//	SDL_Texture* pTexture = NULL;
//
//	//Load the image.
//	SDL_Surface* pSurface = IMG_Load(path.c_str());
//	if (pSurface != NULL)
//	{
//		pTexture = SDL_CreateTextureFromSurface(gRenderer, pSurface);
//		if (pTexture == NULL)
//		{
//			cout << "Unable to create texture from surface. Error: " << SDL_GetError() << endl;
//		}
//		SDL_FreeSurface(pSurface);
//	}
//	else
//	{
//		cout << "Unable to create texture from surface. Error: " << SDL_GetError() << endl;
//	}
//	return pTexture;
//}
//
//void FreeTexture()
//{
//	//Check if the texture exists before removing it.
//	if (gTexture != NULL)
//	{
//		SDL_DestroyTexture(gTexture);
//		gTexture = NULL;
//	}
//}