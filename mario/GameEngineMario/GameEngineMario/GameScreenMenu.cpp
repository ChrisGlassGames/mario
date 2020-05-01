#include "GameScreenMenu.h"
#include <iostream>

GameScreenMenu::GameScreenMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpScreen();
}

GameScreenMenu::~GameScreenMenu()
{
	delete mBlackBackground;
	mBlackBackground = nullptr;

	delete mMarioTitle;
	mMarioTitle = nullptr;

	delete musicOverworld;
	musicOverworld = nullptr;

	delete musicUnderground;
	musicUnderground = nullptr;

	delete startSound;
	startSound = nullptr;

	delete coinPickup;
	coinPickup = nullptr;

	delete deathSFX;
	deathSFX = nullptr;

	delete winSound;
	winSound = nullptr;

	delete loseSound;
	loseSound = nullptr;

	delete pipeSound;
	pipeSound = nullptr;

	delete startSound;
	startSound = nullptr;

	SDL_DestroyTexture(mainText);

}

void GameScreenMenu::Render()
{
	mBlackBackground->Render(Vector2D(), SDL_FLIP_NONE);
	mMarioTitle->Render(Vector2D(((SCREEN_WIDTH / 2) - (mMarioTitle->GetWidth() / 2)), 50), SDL_FLIP_NONE);

	mainTextRect.x = (SCREEN_WIDTH / 2) - (mainTextRect.w / 2);
	mainTextRect.y = ((SCREEN_HEIGHT / 2) - (mainTextRect.h / 2)) + 60;
	SDL_RenderCopy(mRenderer, mainText, NULL, &mainTextRect);

}

void GameScreenMenu::Update(float deltaTime, SDL_Event e)
{
}

bool GameScreenMenu::SetUpScreen()
{

	LoadAudio();
	LoadTextFont();

	Mix_PlayChannel(-1, menuSound, -1);

	//Set the mood with a black background
	mBlackBackground = new Texture2D(mRenderer);
	//Just yoink the background from lvl 1, no one will notice.
	if (!mBlackBackground->LoadFromFile("Images/Levels/Level1/Black_Bg_Small.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}

	mMarioTitle = new Texture2D(mRenderer);
	if (!mMarioTitle->LoadFromFile("Images/Levels/Menu/MenuLogo.png"))
	{
		cout << "Failed to load the title card!";
		return false;
	}



	return true;
}
