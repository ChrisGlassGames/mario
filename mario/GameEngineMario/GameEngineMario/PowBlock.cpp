#include "PowBlock.h"
#include <iostream>

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map)
{
	string  imagePath = "Images/Objects/PowBlock.png";
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str()))
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}

	mLevelMap = map;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();
	mNumberOfHitsLeft = 3;
	mPosition = Vector2D((SCREEN_WIDTH * 0.5f) - mSingleSpriteWidth * 0.5f, 260);
}


PowBlock::~PowBlock()
{
	mRenderer = nullptr;
	mTexture = nullptr;
	mLevelMap = nullptr;
}

void PowBlock::Render()
{
	if (mNumberOfHitsLeft > 0)
	{
		int left = mSingleSpriteWidth * (mNumberOfHitsLeft - 1);
		//								{xpos ,yPos, width of sprite , height of sprite}
		SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

		SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
}

Rect2D PowBlock::GetCollisionBox()
{
	return Rect2D((mPosition.x + 16), mPosition.y, mTexture->GetWidth(), mTexture->GetHeight());
}

void PowBlock::TakeAHit()
{
	mNumberOfHitsLeft--;
	
	if (mNumberOfHitsLeft <= 0)
	{
		mNumberOfHitsLeft = 0;

		mLevelMap->ChangeTileAt(8, 7, 0);
		mLevelMap->ChangeTileAt(8, 8, 0);
	}

}

bool PowBlock::IsAvailable()
{
	return mNumberOfHitsLeft > 0; 
}
