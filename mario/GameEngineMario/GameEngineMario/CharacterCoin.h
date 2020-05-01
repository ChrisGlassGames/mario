#pragma once

#include "Character.h"

class CharacterCoin : public Character
{
public:
	CharacterCoin(SDL_Renderer* renderer, std::string imagePath, LevelMap* map, Vector2D startPosition);
	~CharacterCoin();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	bool Collect() { return mGetCoin; }

private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	float mFrameDelay;
	int mCurrentFrame;

	bool mGetCoin;
};

