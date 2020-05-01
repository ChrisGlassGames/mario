#pragma once
#include "Character.h"
#include "Texture2D.h"
class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterMario();


	void Render();
	void Update(float deltaTime, SDL_Event e);
	void Input(float deltaTime, SDL_Event e);




private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	float mFrameDelay;
	int mCurrentFrame;
	bool flipAnim;

	void Animate(float deltaTime, SDL_Event e);
};

