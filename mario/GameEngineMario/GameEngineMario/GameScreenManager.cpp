#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameScreenMenu.h"



GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	mRenderer = renderer;
	mCurrentScreen = NULL;

	ChangeScreen(startScreen);
}


GameScreenManager::~GameScreenManager()
{
	mRenderer = nullptr;
	mCurrentScreen = nullptr;
	delete mCurrentScreen;
}

void GameScreenManager::Render()
{
	mCurrentScreen->Render();

}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	if (mCurrentScreen != nullptr)
	{
		delete mCurrentScreen;
	}


	GameScreenMenu* tempScreenMenu;
	GameScreenLevel1* tempScreen1;
	GameScreenLevel2* tempScreen2;
	switch (newScreen)
	{
		//case SCREEN_INTRO:
		//	tempScreen1 = new GameScreenLevel1(mRenderer);
		//	mCurrentScreen = (GameScreen*)tempScreen1;
		//	tempScreen1 = NULL;
		//	break;
	case SCREEN_MENU:
		tempScreenMenu = new GameScreenMenu(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenMenu;
		tempScreenMenu = NULL;
		break;
	case SCREEN_LEVEL1:
		tempScreen1 = new GameScreenLevel1(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreen1;
		tempScreen1 = NULL;
		break;
	case SCREEN_LEVEL2:
		tempScreen2 = new GameScreenLevel2(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreen2;
		tempScreen2 = NULL;
		break;
	}
}

