/*

Alejandro Abecasis:
	- LinkedIn ->			  https://www.linkedin.com/in/alejandroabecasis/
	- GitHub (Game Link) ->   https://github.com/Aleman5/Frogger-Abecasis
	- Email ->				  ale.bros@hotmail.com

Assets used:
	- Frog ->				  https://www.piskelapp.com/p/agxzfnBpc2tlbC1hcHByEwsSBlBpc2tlbBiAgMD_lZ3cCww/view
	- Obstacules ->			  https://www.piskelapp.com/p/agxzfnBpc2tlbC1hcHByEwsSBlBpc2tlbBiAgMD_lfunCww/view
	- Controls ->			  https://www.piskelapp.com/p/agxzfnBpc2tlbC1hcHByEwsSBlBpc2tlbBiAgKCAqcHLCAw/view

Font used: 
	- Arcade Classic ->		  https://www.dafont.com/arcade-classic-pizz.font

Sounds used:
	- Hitted by a car ->	  https://freesound.org/people/LittleRobotSoundFactory/sounds/270338/
	- Falling to the water -> https://freesound.org/people/LittleRobotSoundFactory/sounds/270316/
	- Reaching a hole ->	  https://freesound.org/people/LittleRobotSoundFactory/sounds/270341/
	- Reaching the 6 holes -> https://freesound.org/people/LittleRobotSoundFactory/sounds/270330/
	- Background Music ->	  https://freesound.org/people/DirtyJewbs/sounds/159392/

Extra data:
	- Library used ->		  https://www.sfml-dev.org/
	- In case of having problems opening openal32.dll use this link -> https://www.dropbox.com/s/2jrp6fvqzln3r5f/openal%20instalador.zip?dl=0

*/


#include<iostream>
#include<vector>
#include"SFML/Audio.hpp"
#include"SFML/Graphics.hpp"
#include"TitleScreen.h"
#include"HighScores.h"

#define SCREEN_TITLE 0
#define SCREEN_IN_GAME 1
#define SCREEN_HIGHSCORES 2
#define MOUNT_OBSTACULES_2 2
#define MOUNT_OBSTACULES_3 3
#define MOUNT_GOALS 6

using namespace std;

bool loadTexture(sf::Texture& tex, const string file)
{
	if (!tex.loadFromFile(file))
	{
		cerr << "couldn't load the specified image: " << file.c_str();
		return false;
	}
	return true;
}

void changeScore(int& actualScore, const int mount)
{
	actualScore += mount;
}
void changeScore(int& actualScore, const int mount, sf::Text& scoreText)
{
	actualScore += mount;
	scoreText.setString("Score " + to_string(actualScore));
}


void main()
{
	int actualScreen = 0; // 0 = Title Screen / 1 = In game / 2 = Highscores

	// The Frog has to move 12 times to reach the goal, plus the place where appears, we get 13 possible positions
	//  in the 'y' axis where the frog can stay. So 13 x 32 = 416 + 64 = 480. 'y' axis.
	// Where 32 is maxium height of the sprites. Those 64 extra pixels are for GUI staff.
	// The times that the Frog can move horizontally is also 13. So 13 x 32 = 416. 'x' axis.
	int windowWidht = 416;
	int windowHeight = 480;
	sf::RenderWindow window(sf::VideoMode(windowWidht, windowHeight), "Frogger Alejandro Abecasis");
	
	TitleScreen titleScreen(windowWidht, windowHeight);
	Highscores highscores(windowWidht, windowHeight);

	// Font
	sf::Font font;
	font.loadFromFile("ARCADECLASSIC.TTF");


	// Texts
	sf::Text scoreTxt;
	scoreTxt.setFont(font);
	scoreTxt.setCharacterSize(24);
	scoreTxt.setFillColor(sf::Color::White);
	sf::Text levelTxt;
	levelTxt.setFont(font);
	levelTxt.setCharacterSize(24);
	levelTxt.setFillColor(sf::Color::White);


	// Sounds
	sf::Sound sound;
	sound.setVolume(5);
	sf::Sound winSound;
	winSound.setVolume(7);
	sf::SoundBuffer sbCarHit;
	sbCarHit.loadFromFile("CarHit.wav");
	sf::SoundBuffer sbFallingWater;
	sbFallingWater.loadFromFile("FallingWater.wav");
	sf::SoundBuffer sbGoalReached;
	sbGoalReached.loadFromFile("GoalReached.wav");
	sf::SoundBuffer sbWinLevel;
	sbWinLevel.loadFromFile("WinningALevel.wav");
	sf::Music mBackgroundMusic;
	mBackgroundMusic.openFromFile("BackgroundMusic.wav");
	mBackgroundMusic.setVolume(3);
	mBackgroundMusic.setLoop(true);

	// Textures
			// Background
	sf::Texture tBackground;
	loadTexture(tBackground, "Background.png");
			// Frog
	sf::Texture tFrog;
	loadTexture(tFrog, "Frog.png");
			// Obstacules
	sf::Texture tObstacules;
	loadTexture(tObstacules, "Obstacules.png");


	// Sprites
			// Background
	sf::Sprite background;
	background.setTexture(tBackground);
			// Frog
	sf::Sprite frog;
	frog.setTexture(tFrog);
			// Frog copies in case of reaching the goal
	vector<pair<sf::Sprite, bool>> vGoalFrogs;
			// Frog copies for the lifes
	vector<sf::Sprite> vLifeFrogs;
			// Obstacules
	vector<sf::Sprite> vLine1; // Car
	vector<sf::Sprite> vLine2; // Car
	vector<sf::Sprite> vLine3; // Bus
	vector<sf::Sprite> vLine4; // Car
	vector<sf::Sprite> vLine5; // SmallLunks
	vector<sf::Sprite> vLine6; // SmallLunks
	vector<sf::Sprite> vLine7; // LargeLunks
	vector<sf::Sprite> vLine8; // SmallLunks
	vector<sf::Sprite> vLine9; // LargeLunks


	// Variables
			// General purposes
	int offset = 4;
	int actualLine = 0;
	int pos = 0;
	int goalsReached = 0;
	int actualLevel = 1;
	bool hit = false;
			// Frog
	int frogOriginalPosX = 196;
	int frogOriginalPosY = 388;
	frog.setPosition(frogOriginalPosX, frogOriginalPosY);
	int frogMovDistance = 32;
	int frogLifes = 3;
	int frogScore = 0;
	float frogSpeed = 0;
			// Frog copies
	int scorePerGoal = 50;
			// Obstacules
	int positionToReturn = 128;
	float velocityLine1 =  1.8f;
	float velocityLine2 = -2.3f;
	float velocityLine3 =  2.7f;
	float velocityLine4 = -2.0f;
	float velocityLine5 = -1.8f;
	float velocityLine6 =  2.6f;
	float velocityLine7 = -2.2f;
	float velocityLine8 = -2.8f;
	float velocityLine9 =  2.3f;
	float velocityExtraPerWin = 0.35f;
	sf::Time time = sf::seconds(0.01f);


	// Text position
	scoreTxt.setPosition(offset, windowHeight - frogMovDistance + offset);
	changeScore(frogScore, 0, scoreTxt);
	levelTxt.setPosition(windowWidht - 96, windowHeight - frogMovDistance + offset);
	levelTxt.setString("Level " + to_string(actualLevel));


	// Initialization
	for (int i = 0; i < MOUNT_GOALS; i++)
	{
		vGoalFrogs.push_back(make_pair(sf::Sprite(tFrog), false));
		vGoalFrogs[i].first.setPosition((i) * frogMovDistance * 2 + frogMovDistance + offset, 36);
	}

	for (int i = 0; i < frogLifes; i++)
	{
		vLifeFrogs.push_back(sf::Sprite(tFrog));
		vLifeFrogs[i].setPosition(i * frogMovDistance + offset, windowHeight - 60);
	}

	for (int i = 0; i < MOUNT_OBSTACULES_3; i++) 
	{
		// Line 1 Car
		vLine1.push_back(sf::Sprite(tObstacules));
		vLine1[i].setTextureRect(sf::IntRect(0, 0, 32, 24));
		vLine1[i].setPosition(80 * i + 32, frogMovDistance * 11 + offset);

		// Line 2 Car
		vLine2.push_back(sf::Sprite(tObstacules));
		vLine2[i].setTextureRect(sf::IntRect(96, 0, 32, 24));
		vLine2[i].setPosition(96 * i + 32, frogMovDistance * 10 + offset);

		// Line 4 Car
		vLine4.push_back(sf::Sprite(tObstacules));
		vLine4[i].setTextureRect(sf::IntRect(96, 0, 32, 24));
		vLine4[i].setPosition(96 * i + 32, frogMovDistance * 8 + offset);

		// Line 5 Small Lunks
		vLine5.push_back(sf::Sprite(tObstacules));
		vLine5[i].setTextureRect(sf::IntRect(0, 48, 96, 24));
		vLine5[i].setPosition(160 * i + 32, frogMovDistance * 6 + offset);

		// Line 6 Small Lunks
		vLine6.push_back(sf::Sprite(tObstacules));
		vLine6[i].setTextureRect(sf::IntRect(0, 48, 96, 24));
		vLine6[i].setPosition(160 * i + 32, frogMovDistance * 5 + offset);

		// Line 8 Small Lunks
		vLine8.push_back(sf::Sprite(tObstacules));
		vLine8[i].setTextureRect(sf::IntRect(0, 48, 96, 24));
		vLine8[i].setPosition(192 * i + 32, frogMovDistance * 3 + offset);
	}
	for (int i = 0; i < MOUNT_OBSTACULES_2; i++) 
	{
		// Line 3 Bus
		vLine3.push_back(sf::Sprite(tObstacules));
		vLine3[i].setTextureRect(sf::IntRect(32, 0, 64, 24));
		vLine3[i].setPosition(160 * i + 32, frogMovDistance * 9 + offset);

		// Line 7 Large Lunks
		vLine7.push_back(sf::Sprite(tObstacules));
		vLine7[i].setTextureRect(sf::IntRect(0, 24, 128, 24));
		vLine7[i].setPosition(224 * i + 32, frogMovDistance * 4 + offset);

		// Line 9 Large Lunks
		vLine9.push_back(sf::Sprite(tObstacules));
		vLine9[i].setTextureRect(sf::IntRect(0, 24, 128, 24));
		vLine9[i].setPosition(224 * i + 32, frogMovDistance * 2 + offset);
	}


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (actualScreen)
			{
			case SCREEN_TITLE:
				switch (event.type)
				{
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
					case sf::Keyboard::Up:
						titleScreen.goUp();
						break;
					case sf::Keyboard::Down:
						titleScreen.goDown();
						break;
					case sf::Keyboard::Space:
						switch (titleScreen.getActual())
						{
						case 0: // Start Game
							actualScreen = SCREEN_IN_GAME;
							mBackgroundMusic.play();

							// Reseting values to default
							goalsReached = 0;
							actualLevel = 1;
							frog.setPosition(frogOriginalPosX, frogOriginalPosY);
							frogLifes = 3;
							frogScore = 0;
							changeScore(frogScore, 0, scoreTxt);
							for (int i = 0; i < MOUNT_GOALS; i++)
								vGoalFrogs[i].second = false;
							break;
						case 1: // Open Highscore Screen
							actualScreen = SCREEN_HIGHSCORES;
							break;
						case 2: // Exit Game
							window.close();
							break;
						}
						break;
					}
					break;
				case sf::Event::Closed:
					window.close();
					break;
				}
				break;
			case SCREEN_IN_GAME: // In Game
				switch (event.type)
				{
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
					case sf::Keyboard::Up:
						frog.move(0, -frogMovDistance);
						break;
					case sf::Keyboard::Down:
						if (frog.getPosition().y + frogMovDistance < windowHeight - 66)
						{
							frog.move(0, frogMovDistance);
						}
						break;
					case sf::Keyboard::Left:
						if (frog.getPosition().x - frogMovDistance > 0)
						{
							frog.move(-frogMovDistance, 0);
						}
						else
						{
							if (frog.getPosition().y > 67 && frog.getPosition().y < 197) // Collision with the water wall
							{
								if (frogLifes > 0)
								{
									frog.setPosition(frogOriginalPosX, frogOriginalPosY);
									sound.setBuffer(sbFallingWater);
									sound.play();
								}
								else
								{
									actualScreen = SCREEN_HIGHSCORES;
									highscores.saveScore(frogScore);
									winSound.stop();
									mBackgroundMusic.stop();
								}
								frogLifes--;
							}
						}
						break;
					case sf::Keyboard::Right:
						if (frog.getPosition().x + frogMovDistance < windowWidht)
						{
							frog.move(frogMovDistance, 0);
						}
						else
						{
							if (frog.getPosition().y > 67 && frog.getPosition().y < 197) // Collision with the water wall
							{
								if (frogLifes > 0)
								{
									frog.setPosition(frogOriginalPosX, frogOriginalPosY);
									sound.setBuffer(sbFallingWater);
									sound.play();
								}
								else
								{
									actualScreen = SCREEN_HIGHSCORES;
									highscores.saveScore(frogScore);
									winSound.stop();
									mBackgroundMusic.stop();
								}
								frogLifes--;
							}
						}
						break;
					case sf::Keyboard::Space:
						actualScreen = SCREEN_HIGHSCORES;
						winSound.stop();
						mBackgroundMusic.stop();
						break;
					}
					break;

				case sf::Event::Closed:
					window.close();
					break;
				}
				break;
			case 2: // Highscores
				if (event.type == sf::Event::KeyPressed)
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Space:
						actualScreen = SCREEN_TITLE;
					}
				}
				break;
			}

		}

		switch (actualScreen)
		{
		case SCREEN_TITLE:
			window.clear();
			titleScreen.draw(window);
			window.display();
			break;
		case SCREEN_IN_GAME:
			// Checking Collisions and obstacules movement
			actualLine = (int)((frog.getPosition().y - offset) / frogMovDistance);
			frogSpeed = 0;
			hit = true;
			switch (actualLine)
			{
			case 1: // Checking goal
				pos = (int)(frog.getPosition().x / frogMovDistance);

				if (pos % 2 == 1 && !vGoalFrogs[(int)(pos / 2)].second)
				{
					vGoalFrogs[(int)(pos / 2)].second = true;

					changeScore(frogScore, actualLevel * 50 + scorePerGoal, scoreTxt);

					goalsReached++;

					if (goalsReached == MOUNT_GOALS)
					{
						for (int i = 0; i < MOUNT_GOALS; i++)
							vGoalFrogs[i].second = false;
						changeScore(frogScore, scorePerGoal * 4, scoreTxt);

						velocityLine1 += velocityExtraPerWin;
						velocityLine2 += velocityExtraPerWin;
						velocityLine3 += velocityExtraPerWin;
						velocityLine4 += velocityExtraPerWin;
						velocityLine5 += velocityExtraPerWin;
						velocityLine6 += velocityExtraPerWin;
						velocityLine7 += velocityExtraPerWin;
						velocityLine8 += velocityExtraPerWin;
						velocityLine9 += velocityExtraPerWin;

						goalsReached = 0;

						actualLevel++;
						levelTxt.setString("Level " + to_string(actualLevel));

						winSound.setBuffer(sbWinLevel);
						winSound.play();
					}
					else
					{
						winSound.setBuffer(sbGoalReached);
						winSound.play();
					}

					frog.setPosition(frogOriginalPosX, frogOriginalPosY);
				}
				else // In this case, the frog didnt reach de goal correctly
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbCarHit);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 2: // Line 9
				for (int i = 0; i < MOUNT_OBSTACULES_2; i++)
				{
					if (frog.getPosition().x > vLine9[i].getPosition().x && 
						frog.getPosition().x + frog.getTextureRect().width < vLine9[i].getPosition().x + vLine9[i].getTextureRect().width)
					{
						hit = false;
						frog.move(velocityLine9 * time.asSeconds(), 0);
					}

				}
				if (hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbFallingWater);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 3: // Line 8
				for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
				{
					if (frog.getPosition().x > vLine8[i].getPosition().x && 
						frog.getPosition().x + frog.getTextureRect().width < vLine8[i].getPosition().x + vLine8[i].getTextureRect().width)
					{
						frog.move(velocityLine8 * time.asSeconds(), 0);
						hit = false;
					}

				}
				if (hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbFallingWater);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 4: // Line 7
				for (int i = 0; i < MOUNT_OBSTACULES_2; i++)
				{
					if (frog.getPosition().x > vLine7[i].getPosition().x &&
						frog.getPosition().x + frog.getTextureRect().width < vLine7[i].getPosition().x + vLine7[i].getTextureRect().width)
					{
						frog.move(velocityLine7 * time.asSeconds(), 0);
						hit = false;
					}
				}
				if (hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbFallingWater);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 5: // Line 6
				for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
				{
					if (frog.getPosition().x > vLine6[i].getPosition().x &&
						frog.getPosition().x + frog.getTextureRect().width < vLine6[i].getPosition().x + vLine6[i].getTextureRect().width)
					{
						frog.move(velocityLine6 * time.asSeconds(), 0);
						hit = false;
						break;
					}
				}
				if (hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbFallingWater);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 6: // Line 5
				for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
				{
					if (frog.getPosition().x > vLine5[i].getPosition().x &&
						frog.getPosition().x + frog.getTextureRect().width < vLine5[i].getPosition().x + vLine5[i].getTextureRect().width)
					{
						frog.move(velocityLine5 * time.asSeconds(), 0);
						hit = false;
						break;
					}
				}
				if (hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbCarHit);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 8: // Line 4
				for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
				{
					if (frog.getPosition().x + frog.getTextureRect().width < vLine4[i].getPosition().x + vLine4[i].getTextureRect().width)
					{
						if (frog.getPosition().x + frog.getTextureRect().width > vLine4[i].getPosition().x)
						{
							hit = false;
							break;
						}

					}
					else if (hit && frog.getPosition().x > vLine4[i].getPosition().x)
					{
						if (frog.getPosition().x < vLine4[i].getPosition().x + vLine4[i].getTextureRect().width)
						{
							hit = false;
							break;
						}
					}
				}
				if (!hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbCarHit);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 9: // Line 3
				for (int i = 0; i < MOUNT_OBSTACULES_2; i++)
				{
					if (frog.getPosition().x + frog.getTextureRect().width < vLine3[i].getPosition().x + vLine3[i].getTextureRect().width)
					{
						if (frog.getPosition().x + frog.getTextureRect().width > vLine3[i].getPosition().x)
						{
							hit = false;
							break;
						}

					}
					else if (hit && frog.getPosition().x > vLine3[i].getPosition().x)
					{
						if (frog.getPosition().x < vLine3[i].getPosition().x + vLine3[i].getTextureRect().width)
						{
							hit = false;
							break;
						}
					}
				}
				if (!hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbCarHit);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 10: // Line 2
				for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
				{
					if (frog.getPosition().x + frog.getTextureRect().width < vLine2[i].getPosition().x + vLine2[i].getTextureRect().width)
					{
						if (frog.getPosition().x + frog.getTextureRect().width > vLine2[i].getPosition().x)
						{
							hit = false;
							break;
						}

					}
					else if (hit && frog.getPosition().x > vLine2[i].getPosition().x)
					{
						if (frog.getPosition().x < vLine2[i].getPosition().x + vLine2[i].getTextureRect().width)
						{
							hit = false;
							break;
						}
					}
				}
				if (!hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbCarHit);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			case 11: //Line 1
				for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
				{
					if (frog.getPosition().x + frog.getTextureRect().width < vLine1[i].getPosition().x + vLine1[i].getTextureRect().width)
					{
						if (frog.getPosition().x + frog.getTextureRect().width > vLine1[i].getPosition().x)
						{
							hit = false;
							break;
						}

					}
					else if (hit && frog.getPosition().x > vLine1[i].getPosition().x)
					{
						if (frog.getPosition().x < vLine1[i].getPosition().x + vLine1[i].getTextureRect().width)
						{
							hit = false;
							break;
						}
					}
				}
				if (!hit)
				{
					if (frogLifes > 0)
					{
						frog.setPosition(frogOriginalPosX, frogOriginalPosY);
						sound.setBuffer(sbFallingWater);
						sound.play();
					}
					else
					{
						actualScreen = SCREEN_HIGHSCORES;
						highscores.saveScore(frogScore);
						winSound.stop();
						mBackgroundMusic.stop();
					}
					frogLifes--;
				}
				break;
			}

			// Obstacules movement and limit
			for (int i = 0; i < MOUNT_OBSTACULES_2; i++)
			{
				vLine3[i].move(velocityLine3 * time.asSeconds(), 0);
				vLine7[i].move(velocityLine7 * time.asSeconds(), 0);
				vLine9[i].move(velocityLine9 * time.asSeconds(), 0);

				if (vLine3[i].getPosition().x > windowWidht + positionToReturn)
					vLine3[i].move(-windowWidht - positionToReturn * 3, 0);
				if (vLine7[i].getPosition().x < -positionToReturn)
					vLine7[i].move(windowWidht + positionToReturn * 2, 0);
				if (vLine9[i].getPosition().x > windowWidht + positionToReturn)
					vLine9[i].move(-windowWidht - positionToReturn * 3, 0);
			}
			for (int i = 0; i < MOUNT_OBSTACULES_3; i++)
			{
				vLine1[i].move(velocityLine1 * time.asSeconds(), 0);
				vLine2[i].move(velocityLine2 * time.asSeconds(), 0);
				vLine4[i].move(velocityLine4 * time.asSeconds(), 0);
				vLine5[i].move(velocityLine5 * time.asSeconds(), 0);
				vLine6[i].move(velocityLine6 * time.asSeconds(), 0);
				vLine8[i].move(velocityLine8 * time.asSeconds(), 0);

				if (vLine1[i].getPosition().x > windowWidht + positionToReturn)
					vLine1[i].move(-windowWidht - positionToReturn * 3, 0);
				if (vLine2[i].getPosition().x < -positionToReturn)
					vLine2[i].move(windowWidht + positionToReturn * 2, 0);
				if (vLine4[i].getPosition().x < -positionToReturn)
					vLine4[i].move(windowWidht + positionToReturn * 2, 0);
				if (vLine5[i].getPosition().x < -positionToReturn)
					vLine5[i].move(windowWidht + positionToReturn * 2, 0);
				if (vLine6[i].getPosition().x > windowWidht + positionToReturn)
					vLine6[i].move(-windowWidht - positionToReturn * 3, 0);
				if (vLine8[i].getPosition().x < -positionToReturn)
					vLine8[i].move(windowWidht + positionToReturn * 2, 0);
			}


			window.clear();
			window.draw(background);
			window.draw(scoreTxt);
			window.draw(levelTxt);
			for (int i = 0; i < MOUNT_GOALS; i++)
				if (vGoalFrogs[i].second)
					window.draw(vGoalFrogs[i].first);
			for (int i = 0; i < frogLifes; i++)
				window.draw(vLifeFrogs[i]);
			for (int i = 0; i < MOUNT_OBSTACULES_3; i++) // Lines 1, 2, 4, 5, 6, 8
			{
				window.draw(vLine1[i]);
				window.draw(vLine2[i]);
				window.draw(vLine4[i]);
				window.draw(vLine5[i]);
				window.draw(vLine6[i]);
				window.draw(vLine8[i]);
			}
			for (int i = 0; i < MOUNT_OBSTACULES_2; i++) // Lines 3, 7, 9
			{
				window.draw(vLine3[i]);
				window.draw(vLine7[i]);
				window.draw(vLine9[i]);
			}
			window.draw(frog);
			window.display();
			break;
		case SCREEN_HIGHSCORES:
			window.clear();
			highscores.draw(window);
			window.display();
			break;
		}
		
	}
}