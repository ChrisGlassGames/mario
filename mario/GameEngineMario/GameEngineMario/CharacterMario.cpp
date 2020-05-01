#include "CharacterMario.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map) : Character(renderer, imagePath, startPosition, map)
{
	mFacingDireciton = FACING_RIGHT;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();

	flipAnim = false;
}

CharacterMario::~CharacterMario()
{

}

void CharacterMario::Render()
{
	int left = mCurrentFrame * mSingleSpriteWidth;

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	if (mFacingDireciton == FACING_RIGHT)
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);
	}
}

//Update checks for Mario
void CharacterMario::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);
	//Checks for inputs
	Input(deltaTime, e);

	if (mMovingRight || mMovingLeft)
	{
		//Updates Mario Sprite to where he is facing
		Animate(deltaTime, e);
	}
}

void CharacterMario::Input(float deltaTime, SDL_Event e)
{
	//Checks input through switch statements
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			mMovingLeft = false;
			break;

		case SDLK_d:
			mMovingRight = false;
			break;

		default:
			break;
		}
		break;

	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			Character::Jump();
			break;

		case SDLK_s:
			break;

		case SDLK_a:
			mMovingRight = false;
			mMovingLeft = true;
			break;

		case SDLK_d:
			mMovingLeft = false;
			mMovingRight = true;
			break;

		default:
			break;
		}
		break;
	}
}

void CharacterMario::Animate(float deltaTime, SDL_Event e)
{

	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f)
	{
		mFrameDelay = PLAYER_ANIM_TIME;

		if (!flipAnim)
		{
			mCurrentFrame++;
			if (mCurrentFrame > 2)
			{
				mCurrentFrame = 2;
				flipAnim = true;
			}
		}
		else
		{
			mCurrentFrame--;

			if (mCurrentFrame < 0)
			{
				flipAnim = false;
				mCurrentFrame = 0;
			}
		}
	}
}