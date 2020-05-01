#pragma once
#ifndef _CHARCTER_H
#define _CHARACTER_H

#include "SDL.h"
#include <iostream>
#include <SDL_mixer.h>
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include "Constants.h"

class Texture2D;

class Character
{
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	bool LoadAudio();
	Mix_Chunk* jumpSFX;

	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition();
	

	float GetCollisionRadius();

	Rect2D GetCollisionBox();

	
	bool IsJumping();
	void CancelJump();

	FACING mFacingDireciton;

	bool mMovingLeft;
	bool mMovingRight;
	float mSpeedRight;
	float mSpeedLeft;

	void SetAlive(bool isAlive);
	bool GetAlive() { return alive; }

protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;

	bool mCanJump;
	bool mJumping;
	float mJumpForce;

	virtual void AddGravity(float deltaTime);
	virtual void Jump();
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);

	float mCollisionRadius;
	
	bool alive = true;
private:

	LevelMap* mCurrentLevelMap;
};
#endif
