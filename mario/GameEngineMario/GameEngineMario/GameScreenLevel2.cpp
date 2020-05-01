#include "GameScreenLevel2.h"
#include <iostream>
#include "Texture2D.h"
#include "Collisions.h"


GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(renderer)
{
	playersDead = false;

	paused = false;
	startTimer = false;
	pauseMusic = MUSIC_TIMER;
	enemyTimer = ENEMY_TIME;
	startGameTimer = START_TIMER;

	hitTimer = 0.0f;

	SetUpLevel();
}

GameScreenLevel2::~GameScreenLevel2()
{
	// OBJECTS
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

	delete mLevelTexture;
	mLevelTexture = nullptr;

	delete mTransparency;
	mTransparency = nullptr;

	delete mBlackBackground;
	mBlackBackground = nullptr;

	delete characterMario;
	characterMario = nullptr;

	delete characterLuigi;
	characterLuigi = nullptr;

	delete characterPeach;
	characterPeach = nullptr;

	delete mPowBlock;
	mPowBlock = nullptr;

	mLevelMap = nullptr;

	// ENEMIES
	mEnemyKoopa.clear();
	mEnemyGoomba.clear();
	mCoins.clear();

	// SOUNDS
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

	delete loseSound;
	loseSound = nullptr;

	delete pauseSound;
	pauseSound = nullptr;

	delete pipeSound;
	pipeSound = nullptr;

	delete startSound;
	startSound = nullptr;

	delete winSound;
	winSound = nullptr;

	SDL_DestroyTexture(mScoreText);
	SDL_DestroyTexture(mNumberText);

	SDL_DestroyTexture(gmScoreText);
	SDL_DestroyTexture(gmNumberText);
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	startGameTimer -= 1.0f;

	if (startGameTimer <= 0.0f)
	{
		CheckPaused(e);
	}

	if (startGameTimer == 1000.0f)
	{
		Mix_PlayMusic(musicUnderground, -1);
	}

	if (!paused && !characterPeach->isRescued && startGameTimer <= 0.0f)
	{
		if (pauseMusic == MUSIC_TIMER)
		{
			Mix_ResumeMusic();
		}

		if (mScreenshake)
		{
			mScreenshakeTime -= deltaTime;
			mWobble++;
			mBackgroundYPos = sin(mWobble);
			mBackgroundYPos *= 3.0f;

			// End the shake after the duration.
			if (mScreenshakeTime <= 0.0f)
			{
				mScreenshake = false;
				mBackgroundYPos = 0.0f;
			}
		}

		characterMario->Update(deltaTime, e);
		WallWrap(characterMario);

		characterLuigi->Update(deltaTime, e);
		WallWrap(characterLuigi);

		CharacterCollisions(characterMario, characterLuigi);

		UpdatePeach(deltaTime, e);

		// Update and spawn the enemy characters.
		enemyTimer -= 0.25f;
		switch ((int)enemyTimer)
		{
		case 15000:
			CreateKoopa(Vector2D(64, 32), FACING_RIGHT);
			break;
		case 10000:
			CreateGoomba(Vector2D(414, 32), FACING_LEFT);
			break;
		case 5000:
			CreateGoomba(Vector2D(64, 32), FACING_RIGHT);
			break;
		case 0:
			enemyTimer = ENEMY_TIME;
			CreateKoopa(Vector2D(414, 32), FACING_LEFT);
			break;
		default:
			break;
		}

		UpdateKoopas(deltaTime, e);
		UpdateGoombas(deltaTime, e);
		UpdateCoin(deltaTime, e);

		if (characterMario->GetAlive() == false && characterLuigi->GetAlive() == false && playersDead == false)
		{
			Mix_HaltMusic();
			Mix_HaltChannel(-1);
			Mix_PlayChannel(-1, loseSound, 0);
			playersDead = true;

		}
	}
	else
	{
		Mix_PauseMusic();
	}
}


void GameScreenLevel2::Render()
{
	mLevelTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	characterMario->Render();
	characterLuigi->Render();

	characterPeach->Render();

	// Render Mario Score
	mScoreTextRect.x = 75;
	mScoreTextRect.y = 5;
	SDL_RenderCopy(mRenderer, mScoreText, NULL, &mScoreTextRect);
	mNumberTextRect.x = 220;
	mNumberTextRect.y = 5;
	SDL_RenderCopy(mRenderer, mNumberText, NULL, &mNumberTextRect);

	// Render Luigi Score
	gmScoreTextRect.x = 270;
	gmScoreTextRect.y = 5;
	SDL_RenderCopy(mRenderer, gmScoreText, NULL, &gmScoreTextRect);
	gmNumberTextRect.x = 405;
	gmNumberTextRect.y = 5;
	SDL_RenderCopy(mRenderer, gmNumberText, NULL, &gmNumberTextRect);


	for (unsigned int i = 0; i < mCoins.size(); i++)
	{
		mCoins[i]->Render();
	}
	for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
	{
		mEnemyKoopa[i]->Render();
	}
	for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
	{
		mEnemyGoomba[i]->Render();
	}

	if (paused)
	{
		mTransparency->Render(Vector2D(), SDL_FLIP_NONE);

		pauseTextRect.x = (SCREEN_WIDTH / 2) - (pauseTextRect.w / 2);
		pauseTextRect.y = (SCREEN_HEIGHT / 2) - (pauseTextRect.h / 2);
		SDL_RenderCopy(mRenderer, pauseText, NULL, &pauseTextRect);
	}
	else
	{
		if (startGameTimer >= 0.0f)
		{
			mTransparency->Render(Vector2D(), SDL_FLIP_NONE);

			level2TextRect.x = (SCREEN_WIDTH / 2) - (level2TextRect.w / 2);
			level2TextRect.y = (SCREEN_HEIGHT / 2) - (level2TextRect.h / 2);
			SDL_RenderCopy(mRenderer, level2Text, NULL, &level2TextRect);
		}

		if (characterPeach->isRescued)
		{
			mBlackBackground->Render(Vector2D(), SDL_FLIP_NONE);

			winTextRect.x = (SCREEN_WIDTH / 2) - (winTextRect.w / 2);
			winTextRect.y = ((SCREEN_HEIGHT / 2) - (winTextRect.h / 2)) + 40;
			SDL_RenderCopy(mRenderer, winText, NULL, &winTextRect);
		}

		if (characterMario->GetAlive() == false && characterLuigi->GetAlive() == false && !characterPeach->isRescued)
		{
			mBlackBackground->Render(Vector2D(), SDL_FLIP_NONE);

			gameOverTextRect.x = (SCREEN_WIDTH / 2) - (gameOverTextRect.w / 2);
			gameOverTextRect.y = (SCREEN_HEIGHT / 2) - (gameOverTextRect.h / 2);
			SDL_RenderCopy(mRenderer, gameOverText, NULL, &gameOverTextRect);

		}
	}
}

bool GameScreenLevel2::SetUpLevel()
{
	SetLevelMap();


	LoadAudio();
	LoadTextFont();


	Mix_PlayChannel(-1, startSound, 0);

	mBlackBackground = new Texture2D(mRenderer);
	if (!mBlackBackground->LoadFromFile("Images/Levels/Level2/Black_Bg_Small.png"))
	{
		cout << "Failed to load black background texture!";
		return false;
	}


	mLevelTexture = new Texture2D(mRenderer);
	if (!mLevelTexture->LoadFromFile("Images/Levels/Level2/Level2.png"))
	{
		cout << "Failed to load level map texture!";
		return false;
	}

	mTransparency = new Texture2D(mRenderer);
	if (!mTransparency->LoadFromFile("Images/Transparency.png"))
	{
		cout << "Failed to load transparency image!";
		return false;
	}

	characterMario = new CharacterMario(mRenderer, "Images/Characters/MarioWalk.png", Vector2D(64, 342), mLevelMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Images//Characters/LuigiWalk.png", Vector2D(414, 342), mLevelMap);

	mScreenshake = false;
	mBackgroundYPos = 0.0f;

	CreatePeach(Vector2D(32, 20), FACING_RIGHT);
	CreateKoopa(Vector2D(64, 34), FACING_RIGHT);
	CreateGoomba(Vector2D(414, 34), FACING_LEFT);

	//Coin creation timee
	{

		//Middle-left
		CreateCoin(Vector2D(25, 165));
		CreateCoin(Vector2D(85, 195));

		//Middle
		CreateCoin(Vector2D(215, 245));
		CreateCoin(Vector2D(280, 245));

		//Middle-right
		CreateCoin(Vector2D(470, 165));
		CreateCoin(Vector2D(410, 195));

		//Middle
		CreateCoin(Vector2D(215, 85));
		CreateCoin(Vector2D(280, 85));
		CreateCoin(Vector2D(175, 85));
		CreateCoin(Vector2D(320, 85));
	}

}

void GameScreenLevel2::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1 },
									   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0 },
									   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
									   { 0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0 },
									   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0 },
									   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									   { 1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1 } };
	//clear up any old map
	if (mLevelMap != nullptr)
	{
		delete mLevelMap;
	}

	//set the new map
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel2::WallWrap(Character* character)
{
	// Right wall collision - wrap left
	if (character->GetPosition().x >= SCREEN_WIDTH - character->GetCollisionBox().Width / 3)
	{
		character->SetPosition(Vector2D(0, character->GetPosition().y));
	}

	// Left wall collision - wrap right
	if (character->GetPosition().x < 0)
	{
		character->SetPosition(Vector2D(SCREEN_WIDTH - character->GetCollisionBox().Width / 3, character->GetPosition().y));
	}
}

void GameScreenLevel2::UpdatePeach(float deltaTime, SDL_Event e)
{
	characterPeach->Update(deltaTime, e);

	if (!characterPeach->isRescued)
	{
		if (Collisions::Instance()->Circle(characterPeach, characterMario))
		{
			Mix_HaltMusic();
			Mix_PlayChannel(-1, winSound, 0);

			characterPeach->isRescued = true;


			cout << "The Mario won the map" << endl;
		}
		else if (Collisions::Instance()->Circle(characterPeach, characterLuigi))
		{
			Mix_HaltMusic();
			Mix_PlayChannel(-1, winSound, 0);

			characterPeach->isRescued = true;

			cout << endl << "The Mario Brother won the map";
		}
	}
}

void GameScreenLevel2::CreatePeach(Vector2D position, FACING direction)
{
	characterPeach = new CharacterPeach(mRenderer, "Images/Characters/Peach.png", mLevelMap, position, direction);
}

void GameScreenLevel2::UpdateCoin(float deltaTime, SDL_Event e)
{
	if (!mCoins.empty())
	{
		int coinToDelete = -1;
		for (unsigned int i = 0; i < mCoins.size(); i++)
		{
			mCoins[i]->Update(deltaTime, e);

			if (Collisions::Instance()->Circle(mCoins[i], characterMario))
			{
				Mix_PlayChannel(-1, coinPickup, 0);
				mCoins[i]->SetAlive(false);

				cout << "Mario got dolla" << endl;

				marioScore += 10;
				UpdateScores();
			}
			if (Collisions::Instance()->Circle(mCoins[i], characterLuigi))
			{
				Mix_PlayChannel(-1, coinPickup, 0);
				mCoins[i]->SetAlive(false);

				cout << "Alternate Waluigi got bank" << endl;

				luigiScore += 1;
				UpdateScores();
			}

			if (!mCoins[i]->GetAlive())
			{
				coinToDelete = i;
			}
		}

		if (coinToDelete != -1)
		{
			mCoins.erase(mCoins.begin() + coinToDelete);
		}
	}
}

void GameScreenLevel2::CreateCoin(Vector2D position)
{
	CharacterCoin* characterCoin = new CharacterCoin(mRenderer, "Images/Objects/Coin.png", mLevelMap, position);
	mCoins.push_back(characterCoin);
}


void GameScreenLevel2::DoScreenshake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
	{
		mEnemyKoopa[i]->TakeDamage();
	}

	for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
	{
		mEnemyGoomba[i]->TakeDamage();
	}
}

void GameScreenLevel2::UpdateKoopas(float deltaTime, SDL_Event e)
{
	if (!mEnemyKoopa.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
		{
			// Check if enemy is on the bottom row of tiles.
			if (mEnemyKoopa[i]->GetPosition().y > 300.0f)
			{
				// Is the enemy off screen to the left / right?
				if (mEnemyKoopa[i]->GetPosition().x < (float)(-mEnemyKoopa[i]->GetCollisionBox().Width * 0.5f) || mEnemyKoopa[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemyKoopa[i]->GetCollisionBox().Width * 0.5f))
				{
					mEnemyKoopa[i]->SetAlive(false);

				}
			}

			mEnemyKoopa[i]->Update(deltaTime, e);

			// Check to see if the enemy collides with the player.
			if (mEnemyKoopa[i]->GetPosition().y > 300.0f || mEnemyKoopa[i]->GetPosition().y <= 64.0f && (mEnemyKoopa[i]->GetPosition().x < 64.0f || mEnemyKoopa[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
			}
			else
			{
				if (Collisions::Instance()->Circle(mEnemyKoopa[i], characterMario))
				{
					if (mEnemyKoopa[i]->GetPosition().y > characterMario->GetPosition().y)
					{
						mEnemyKoopa[i]->GetInjured();
						cout << "Mario got that koopa" << endl;
						marioScore += 20;
						UpdateScores();
						mEnemyKoopa[i]->SetPosition(Vector2D(500, 1000));
					}
					else
					{
						Mix_PauseMusic();
						startTimer = true;
						Mix_PlayChannel(-1, deathSFX, 0);

						characterMario->SetAlive(false);
						characterMario->SetPosition(Vector2D(500, 1000));

						cout << "Mario got shelled" << endl;
					}
				}
				else if (Collisions::Instance()->Circle(mEnemyKoopa[i], characterLuigi))
				{
					if (mEnemyKoopa[i]->GetPosition().y > characterLuigi->GetPosition().y)
					{
						mEnemyKoopa[i]->GetInjured();
						cout << "The Brother got that koopa" << endl;
						luigiScore += 2;
						UpdateScores();
						mEnemyKoopa[i]->SetPosition(Vector2D(500, 1000));
					}
					else
					{
						Mix_PauseMusic();
						startTimer = true;
						Mix_PlayChannel(-1, deathSFX, 0);

						characterLuigi->SetAlive(false);
						characterLuigi->SetPosition(Vector2D(500, 1000));

						cout << "Mario's Brother got shelled" << endl;
					}
				}
			}

			if (!mEnemyKoopa[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		if (enemyIndexToDelete != -1)
		{
			mEnemyKoopa.erase(mEnemyKoopa.begin() + enemyIndexToDelete);
		}
	}

	if (startTimer == true)
	{
		pauseMusic -= 1.0f;
	}

	if (pauseMusic == 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel2::CreateKoopa(Vector2D position, FACING direction)
{
	CharacterKoopa* characterKoopa = new CharacterKoopa(mRenderer, "Images/Enemies/Koopa.png", mLevelMap, position, direction);
	mEnemyKoopa.push_back(characterKoopa);
}

void GameScreenLevel2::UpdateGoombas(float deltaTime, SDL_Event e)
{
	if (!mEnemyGoomba.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
		{
			//at bottom?
			if (mEnemyGoomba[i]->GetPosition().y > 300.0f)
			{
				//off screen?
				if (mEnemyGoomba[i]->GetPosition().x < (float)(-mEnemyGoomba[i]->GetCollisionBox().Width * 0.5f) || mEnemyGoomba[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemyGoomba[i]->GetCollisionBox().Width * 0.5f))
				{
					mEnemyGoomba[i]->SetAlive(false);;
				}
			}

			mEnemyGoomba[i]->Update(deltaTime, e);

			//Check to see if the goomba hit player
			if (mEnemyGoomba[i]->GetPosition().y > 300.0f || mEnemyGoomba[i]->GetPosition().y <= 64.0f && (mEnemyGoomba[i]->GetPosition().x < 64.0f || mEnemyGoomba[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{

			}
			else
			{
				if (Collisions::Instance()->Circle(mEnemyGoomba[i], characterMario))
				{
					if (mEnemyGoomba[i]->GetPosition().y > characterMario->GetPosition().y)
					{
						mEnemyGoomba[i]->GetInjured();
						cout << "Mario got that koopa" << endl;
						marioScore += 50;
						UpdateScores();
						mEnemyGoomba[i]->SetPosition(Vector2D(500, 1000));
					}
					else
					{
						Mix_PauseMusic();
						startTimer = true;
						Mix_PlayChannel(-1, deathSFX, 0);

						characterMario->SetAlive(false);
						characterMario->SetPosition(Vector2D(500, 1000));

						cout << endl << "A Goomba killed Mario";

					}
				}
				else if (Collisions::Instance()->Circle(mEnemyGoomba[i], characterLuigi))
				{
					if (mEnemyGoomba[i]->GetPosition().y > characterLuigi->GetPosition().y)
					{
						mEnemyGoomba[i]->GetInjured();
						cout << "Green Mario is a Murderer" << endl;
						luigiScore += 5;
						UpdateScores();
						mEnemyGoomba[i]->SetPosition(Vector2D(500, 1000));
					}
					else
					{
						Mix_PauseMusic();
						startTimer = true;
						Mix_PlayChannel(-1, deathSFX, 0);

						characterLuigi->SetAlive(false);
						characterLuigi->SetPosition(Vector2D(500, 1000));

						cout << endl << "A Goomba killed other Mario";
					}
				}
			}

			if (!mEnemyGoomba[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		if (enemyIndexToDelete != -1)
		{
			mEnemyGoomba.erase(mEnemyGoomba.begin() + enemyIndexToDelete);
		}
	}

	if (startTimer == true)
	{
		pauseMusic -= 1.0f;
	}

	if (pauseMusic == 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel2::CreateGoomba(Vector2D position, FACING direction)
{
	CharacterGoomba* characterGoomba = new CharacterGoomba(mRenderer, "Images/Enemies/GoombaNPC.png", mLevelMap, position, direction);
	mEnemyGoomba.push_back(characterGoomba);
}

//void GameScreenLevel2::UpdateEnemies(float deltaTime, SDL_Event e)
//{
//	//Update the enemies
//	if (!mEnemies.empty())
//	{
//		int enemyIndexToDelete = -1;
//		for (unsigned int i = 0; i < mEnemies.size(); i++)
//		{
//			//Check if enemy is on the bottom row of tiles
//			//if (mEnemies[i]->GetPosition().y > 300.0f)
//			//{
//				//Is the enemy off the screen?
//				if (mEnemies[i]->GetPosition().x < (float)(-mEnemies[i]->GetCollisionBox().Width * 0.5f) || mEnemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemies[i]->GetCollisionBox().Width * 0.55f))
//				{
//					mEnemies[i]->setAlive(false);
//				}
//			/*}*/
//			//Do the update
//			mEnemies[i]->Update(deltaTime, e);
//
//			//Check if the enemy collides
//			if ((mEnemies[i]->GetPosition().y > 300.0f || mEnemies[i]->GetPosition().y <= 64.0f) && (mEnemies[i]->GetPosition().x < 64.0f || mEnemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
//			{
//				//Ignore the collisions if enemy behind a pipe
//			}
//			else
//			{
//				if (Collisions::Instance()->Circle(mEnemies[i], mMarioCharacter))
//				{
//					//This is where mario dies if he needs to
//					cout << "OH NO ";
//				}
//			}
//
//			if (!mEnemies[i]->getAlive())
//			{
//				enemyIndexToDelete = i;
//			}
//		}
//		//remove dead enemies
//		if (enemyIndexToDelete != -1)
//		{
//			mEnemies.erase(mEnemies.begin() + enemyIndexToDelete);
//		}
//	}
//}
