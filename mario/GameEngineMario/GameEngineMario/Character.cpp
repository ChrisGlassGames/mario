#include "Character.h"
#include "Texture2D.h"



Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map)
{
	mRenderer = renderer;
	mPosition = startPosition;
	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imagePath))
	{
		cout << "Failed to load character!";
	}

	LoadAudio();

	mMovingLeft = false;
	mMovingRight = false;

	mCollisionRadius = 15.0f;

	mCurrentLevelMap = map;
}


Character::~Character()
{
	mRenderer = nullptr;
}

void Character::Render()
{
	//
	//if (mFacingDireciton == FACING_RIGHT)
	//{
	//	mTexture->Render(mPosition, SDL_FLIP_NONE);
	//}
	//else
	//{
	//	mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
	//}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	int headPosition = (int)(mPosition.y + mTexture->GetHeight() - 32) / TILE_HEIGHT;
	int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;

	//Jumping tests
	if (mJumping)
	{
		mPosition.y -= mJumpForce * deltaTime;
		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;

		if (mJumpForce <= 0.0f && mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 1)
		{
			mJumping = false;
		}
	}

	//Gravity timee
	if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0)
	{
		AddGravity(deltaTime);
	}
	else
	{
		mCanJump = true;
	}

	if (mCurrentLevelMap->GetTileAt(headPosition, centralXPosition) == 1)
	{
		mJumpForce = 0.0f;
		mCanJump = false;
	}

	if (mMovingLeft)
	{
		MoveLeft(deltaTime);
	}
	else if (mMovingRight)
	{
		MoveRight(deltaTime);
	}

}

void Character::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}


Vector2D Character::GetPosition()
{
	return mPosition;
}

void Character::AddGravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
	//mCanJump = false;
}

void Character::MoveLeft(float deltaTime)
{
	mSpeedRight = 0.0f;
	mFacingDireciton = FACING_LEFT;

	mPosition.x -= mSpeedLeft * deltaTime;
	mSpeedLeft -= MOVEMENT_ACCELERATION_L * deltaTime;

	if (mSpeedLeft > MAX_SPEED)
	{
		mSpeedLeft = MAX_SPEED;
	}
}

void Character::MoveRight(float deltaTime)
{
	mSpeedLeft = 0.0f;
	mFacingDireciton = FACING_RIGHT;

	mPosition.x += mSpeedRight * deltaTime;
	mSpeedRight += MOVEMENT_ACCELERATION_R * deltaTime;

	if (mSpeedRight > MAX_SPEED)
	{
		mSpeedRight = MAX_SPEED;
	}
}

float Character::GetCollisionRadius()
{
	return mCollisionRadius;
}

Rect2D Character::GetCollisionBox()
{ 
	return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight());
}

void Character::Jump()
{
	if (!mJumping)
	{
		Mix_PlayChannel(-1, jumpSFX, 0);
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

bool Character::IsJumping()
{
	return mJumping;
}

void Character::CancelJump()
{
	mJumpForce = 0;
}

void Character::SetAlive(bool isAlive)
{
	alive = isAlive;
}

bool Character::LoadAudio()
{
	jumpSFX = Mix_LoadWAV("Sounds/SFX/Jump.wav");
	if (jumpSFX == NULL)
	{
		cout << "Failed to load jumpSFX! Error: " << Mix_GetError() << endl;
		return false;
	}
	return true;
}