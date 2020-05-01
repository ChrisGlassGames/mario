#include "CharacterGoomba.h"

CharacterGoomba::CharacterGoomba(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing) : Character(renderer, imagePath, startPosition, map)
{
	//Set the gomba base settings
	mFacingDireciton = startFacing;
	mPosition = startPosition;
	mInjured = false;

	mSingleSpriteWidth = mTexture->GetWidth() / 2;
	mSingleSpriteHeight = mTexture->GetHeight();
}

CharacterGoomba::~CharacterGoomba()
{

}

void CharacterGoomba::Render()
{
	//Detect Goomba frame and direction and change the sprite accordingly
	int left = mCurrentFrame * mSingleSpriteWidth;

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	if (mFacingDireciton == FACING_RIGHT)
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
	else if (mInjured)
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_VERTICAL);
	}
	else
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void CharacterGoomba::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);
	//Check if goomba injured or which direction it is facing
	if (!mInjured)
	{
		AnimateGoomba(deltaTime, e);
		if (mFacingDireciton == FACING_LEFT)
		{
			mMovingLeft = true;
			mMovingRight = false;
		}
		else if (mFacingDireciton == FACING_RIGHT)
		{
			mMovingRight = true;
			mMovingLeft = false;
		}
	}
	else
	{
		mMovingRight = false;
		mMovingLeft = false;

		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0)
		{
			Flip();
		}
	}
}

void CharacterGoomba::AnimateGoomba(float deltaTime, SDL_Event e)
{
	//Anime the goomba through frame countinng
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f)
	{
		mFrameDelay = ANIM_DELAY;
		mCurrentFrame++;

		if (mCurrentFrame > 1)
		{
			mCurrentFrame = 0;
		}
	}
}

void CharacterGoomba::TakeDamage()
{
	//Damagethe goomba
	mInjured = true;
	mInjuredTime = INJURED_TIME;
	Jump();
}

void CharacterGoomba::Jump()
{
	//Set jump force for goomba and detect it's speed
	if (!mJumping)
	{
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;

		mJumping = true;
		mCanJump = false;
	}
}

void CharacterGoomba::Flip()
{
	//Reset goomba to flip the right way up
	if (mFacingDireciton == 0)
	{
		mFacingDireciton = FACING_RIGHT;
	}
	else
	{
		mFacingDireciton = FACING_LEFT;
	}

	mInjured = false;
	Jump();
}