#pragma once

#ifndef _GAMESCREEN_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>

#include "Collisions.h"
#include "Constants.h"

using namespace std;

class GameScreen
{
public:
	GameScreen(SDL_Renderer* renderer);
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	void CharacterCollisions(Character* character1, Character* character2);
	int marioScore;
	int marioSavedPeach;

	int luigiScore;
	int luigiSavedPeach;

	void CheckPaused(SDL_Event e);
	bool paused;

	//SFX
	bool LoadAudio();

	Mix_Music* musicOverworld;
	Mix_Music* musicUnderground;
	bool startTimer;
	float pauseMusic;
	float startGameTimer;
	Mix_Chunk* gameStartMusic;
	Mix_Chunk* coinPickup;
	Mix_Chunk* deathSFX;
	Mix_Chunk* loseSound;
	Mix_Chunk* pauseSound;
	Mix_Chunk* menuSound;
	Mix_Chunk* pipeSound;
	Mix_Chunk* startSound;
	Mix_Chunk* winSound;

	void LoadTextFont();


	//Prepare for font spam!
	TTF_Font* fontLarge;
	TTF_Font* fontMedium;
	TTF_Font* fontSmall;
	TTF_Font* fontTiny;

	SDL_Color white;

	//Font Time again, bouta go super sayian
	SDL_Surface* mainTextSurface;
	SDL_Texture* mainText;
	SDL_Rect mainTextRect;

	SDL_Surface* level1TextSurface;
	SDL_Texture* level1Text;
	SDL_Rect level1TextRect;

	SDL_Surface* level2TextSurface;
	SDL_Texture* level2Text;
	SDL_Rect level2TextRect;

	SDL_Surface* pauseTextSurface;
	SDL_Texture* pauseText;
	SDL_Rect pauseTextRect;

	SDL_Surface* mScoreTextSurface;
	SDL_Texture* mScoreText;
	SDL_Rect mScoreTextRect;

	SDL_Surface* mNumberTextSurface;
	SDL_Texture* mNumberText;
	SDL_Rect mNumberTextRect;
	string mNumberString;

	SDL_Surface* gmScoreTextSurface;
	SDL_Texture* gmScoreText;
	SDL_Rect gmScoreTextRect;

	SDL_Surface* gmNumberTextSurface;
	SDL_Texture* gmNumberText;
	SDL_Rect gmNumberTextRect;
	string gmNumberString;

	//Level Complete Texts
	SDL_Surface* gameOverTextSurface;
	SDL_Texture* gameOverText;
	SDL_Rect gameOverTextRect;

	SDL_Surface* nextLevelTextSurface;
	SDL_Texture* nextLevelText;
	SDL_Rect nextLevelTextRect;

	SDL_Surface* winTextSurface;
	SDL_Texture* winText;
	SDL_Rect winTextRect;

	void UpdateScores();

protected:
	SDL_Renderer* mRenderer;
};
#endif
