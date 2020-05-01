#include "GameScreen.h"
#include <iostream>

using namespace std;

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}


GameScreen::~GameScreen()
{
	mRenderer = nullptr;
}

void GameScreen::Render()
{

}

void GameScreen::Update(float deltaTime, SDL_Event e)
{

}

void GameScreen::CharacterCollisions(Character* character1, Character* character2)
{
}

void GameScreen::CheckPaused(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_p:
			Mix_PlayChannel(-1, pauseSound, 0);
			paused = !paused;
			break;
		}
		break;
	}
}





bool GameScreen::LoadAudio()
{
	musicOverworld = Mix_LoadMUS("Sounds/Music/Mario_Overworld.ogg");
	if (musicOverworld == NULL)
	{
		cout << "Failed to load overworld theme! Error: " << Mix_GetError() << endl;
		return false;
	}

	musicUnderground = Mix_LoadMUS("Sounds/Music/Mario_Underworld.ogg");
	if (musicUnderground == NULL)
	{
		cout << "Failed to load underground theme! Error: " << Mix_GetError() << endl;
		return false;
	}

	menuSound = Mix_LoadWAV("Sounds/Music/TitleTheme.wav");
	if (menuSound == NULL)
	{
		cout << "Failed to load game start sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	coinPickup = Mix_LoadWAV("Sounds/SFX/Coin.wav");
	if (coinPickup == NULL)
	{
		cout << "Failed to load coin pickup! Error: " << Mix_GetError() << endl;
		return false;
	}

	deathSFX = Mix_LoadWAV("Sounds/SFX/Die.wav");
	if (deathSFX == NULL)
	{
		cout << "Failed to load death sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	winSound = Mix_LoadWAV("Sounds/SFX/Flagpole.wav");
	if (winSound == NULL)
	{
		cout << "Failed to load flagpole sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	loseSound = Mix_LoadWAV("Sounds/SFX/GameOver.wav");
	if (loseSound == NULL)
	{
		cout << "Failed to load game over sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	pauseSound = Mix_LoadWAV("Sounds/SFX/Pause.wav");
	if (pauseSound == NULL)
	{
		cout << "Failed to load pause sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	pipeSound = Mix_LoadWAV("Sounds/SFX/Pipe.wav");
	if (pipeSound == NULL)
	{
		cout << "Failed to load pipe sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	startSound = Mix_LoadWAV("Sounds/SFX/LevelStart.wav");
	if (startSound == NULL)
	{
		cout << "Failed to load start sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	return true;
}

void GameScreen::LoadTextFont()
{
	fontLarge = TTF_OpenFont("Fonts/MarioFont.ttf", 72);
	fontMedium = TTF_OpenFont("Fonts/MarioFont.ttf", 36);
	fontTiny = TTF_OpenFont("Fonts/MarioFont.ttf", 24);
	fontTiny = TTF_OpenFont("Fonts/MarioFont.ttf", 18);

	white = { 255, 255, 255, 255 };

	mainTextSurface = TTF_RenderText_Solid(fontMedium, "'ENTER' TO START", white);
	mainText = SDL_CreateTextureFromSurface(mRenderer, mainTextSurface);
	SDL_QueryTexture(mainText, NULL, NULL, &mainTextRect.w, &mainTextRect.h);

	SDL_FreeSurface(mainTextSurface);
	mainTextSurface = nullptr;

	level1TextSurface = TTF_RenderText_Solid(fontMedium, "LEVEL 1 START", white);
	level1Text = SDL_CreateTextureFromSurface(mRenderer, level1TextSurface);
	SDL_QueryTexture(level1Text, NULL, NULL, &level1TextRect.w, &level1TextRect.h);

	SDL_FreeSurface(level1TextSurface);
	level1TextSurface = nullptr;

	level2TextSurface = TTF_RenderText_Solid(fontMedium, "LEVEL 2 START", white);
	level2Text = SDL_CreateTextureFromSurface(mRenderer, level2TextSurface);
	SDL_QueryTexture(level2Text, NULL, NULL, &level2TextRect.w, &level2TextRect.h);

	SDL_FreeSurface(level2TextSurface);
	level2TextSurface = nullptr;

	pauseTextSurface = TTF_RenderText_Solid(fontLarge, "paused", white);
	pauseText = SDL_CreateTextureFromSurface(mRenderer, pauseTextSurface);
	SDL_QueryTexture(pauseText, NULL, NULL, &pauseTextRect.w, &pauseTextRect.h);

	SDL_FreeSurface(pauseTextSurface);
	pauseTextSurface = nullptr;

	mScoreTextSurface = TTF_RenderText_Solid(fontTiny, "Mario score:", white);
	mScoreText = SDL_CreateTextureFromSurface(mRenderer, mScoreTextSurface);
	SDL_QueryTexture(mScoreText, NULL, NULL, &mScoreTextRect.w, &mScoreTextRect.h);

	SDL_FreeSurface(mScoreTextSurface);
	mScoreTextSurface = nullptr;

	mNumberString = to_string(marioScore);

	mNumberTextSurface = TTF_RenderText_Solid(fontTiny, mNumberString.c_str(), white);
	mNumberText = SDL_CreateTextureFromSurface(mRenderer, mNumberTextSurface);
	SDL_QueryTexture(mNumberText, NULL, NULL, &mNumberTextRect.w, &mNumberTextRect.h);

	SDL_FreeSurface(mNumberTextSurface);
	mNumberTextSurface = nullptr;

	gmScoreTextSurface = TTF_RenderText_Solid(fontTiny, "green score:", white);
	gmScoreText = SDL_CreateTextureFromSurface(mRenderer, gmScoreTextSurface);
	SDL_QueryTexture(gmScoreText, NULL, NULL, &gmScoreTextRect.w, &gmScoreTextRect.h);

	SDL_FreeSurface(gmScoreTextSurface);
	gmScoreTextSurface = nullptr;


	gmNumberString = to_string(luigiScore);

	gmNumberTextSurface = TTF_RenderText_Solid(fontTiny, gmNumberString.c_str(), white);
	gmNumberText = SDL_CreateTextureFromSurface(mRenderer, gmNumberTextSurface);
	SDL_QueryTexture(gmNumberText, NULL, NULL, &gmNumberTextRect.w, &gmNumberTextRect.h);

	SDL_FreeSurface(mNumberTextSurface);
	mNumberTextSurface = nullptr;

	//level complete, and lose

	gameOverTextSurface = TTF_RenderText_Solid(fontLarge, "game over", white);
	gameOverText = SDL_CreateTextureFromSurface(mRenderer, gameOverTextSurface);
	SDL_QueryTexture(gameOverText, NULL, NULL, &gameOverTextRect.w, &gameOverTextRect.h);

	SDL_FreeSurface(gameOverTextSurface);
	gameOverTextSurface = nullptr;

	nextLevelTextSurface = TTF_RenderText_Solid(fontMedium, "'ENTER' - LEVEL 2", white);
	nextLevelText = SDL_CreateTextureFromSurface(mRenderer, nextLevelTextSurface);
	SDL_QueryTexture(nextLevelText, NULL, NULL, &nextLevelTextRect.w, &nextLevelTextRect.h);

	SDL_FreeSurface(nextLevelTextSurface);
	nextLevelTextSurface = nullptr;

	winTextSurface = TTF_RenderText_Solid(fontMedium, "'YOU WIN!\n'ESC' - EXIT", white);
	winText = SDL_CreateTextureFromSurface(mRenderer, winTextSurface);
	SDL_QueryTexture(winText, NULL, NULL, &winTextRect.w, &winTextRect.h);

	SDL_FreeSurface(winTextSurface);
	winTextSurface = nullptr;
}

void GameScreen::UpdateScores()
{
	mNumberString = to_string(marioScore);

	mNumberTextSurface = TTF_RenderText_Solid(fontTiny, mNumberString.c_str(), white);
	mNumberText = SDL_CreateTextureFromSurface(mRenderer, mNumberTextSurface);
	SDL_QueryTexture(mNumberText, NULL, NULL, &mNumberTextRect.w, &mNumberTextRect.h);

	SDL_FreeSurface(mNumberTextSurface);
	mNumberTextSurface = nullptr;

	gmNumberString = to_string(luigiScore);

	gmNumberTextSurface = TTF_RenderText_Solid(fontTiny, gmNumberString.c_str(), white);
	gmNumberText = SDL_CreateTextureFromSurface(mRenderer, gmNumberTextSurface);
	SDL_QueryTexture(gmNumberText, NULL, NULL, &gmNumberTextRect.w, &gmNumberTextRect.h);

	SDL_FreeSurface(mNumberTextSurface);
	mNumberTextSurface = nullptr;
}
