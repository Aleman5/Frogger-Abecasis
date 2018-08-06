/*



Font used: Arcade Classic. Download link -> https://www.dafont.com/arcade-classic-pizz.font

*/


#include<iostream>
#include<vector>
#include"SFML/Window.hpp"
#include"SFML/Audio.hpp"
#include"SFML/Graphics.hpp"
#include"TitleScreen.h"
#include"HighScores.h"

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

	// Textures
			// Background
	sf::Texture tBackground;
	loadTexture(tBackground, "Background.png");
			// Frog
	sf::Texture tFrog;
	loadTexture(tFrog, "Frog.png");


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

	// Variables
			// Frog
	int frogOriginalPosX = 196;
	int frogOriginalPosY = 388;
	int frogX = frogOriginalPosX;
	int frogY = frogOriginalPosY;
	int frogMovDistance = 32;
	int frogLifes = 3;
	int frogScore = 0;

			// Frog copies
	int mountOfGoals = 6;
	int scorePerGoal = 100;

	// Text position
	scoreTxt.setPosition(4, windowHeight - frogMovDistance + 4);
	changeScore(frogScore, 0, scoreTxt);

	// Initialization
	for (int i = 0; i < mountOfGoals; i++)
	{
		vGoalFrogs.push_back(make_pair(sf::Sprite(tFrog), false));
		vGoalFrogs[i].first.setPosition((i) * frogMovDistance * 2 + frogMovDistance + 4, 36);
	}

	for (int i = 0; i < frogLifes; i++)
	{
		vLifeFrogs.push_back(sf::Sprite(tFrog));
		vLifeFrogs[i].setPosition(i * frogMovDistance + 4, windowHeight - 60);
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (actualScreen)
			{
			case 0: // Title Screen
				switch (event.type)
				{
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
					case sf::Keyboard::W:
						titleScreen.goUp();
						break;
					case sf::Keyboard::S:
						titleScreen.goDown();
						break;
					case sf::Keyboard::Space:
						switch (titleScreen.getActual())
						{
						case 0: // Start Game
							actualScreen = 1;

							// Reseting values to default
							frogX = frogOriginalPosX;
							frogY = frogOriginalPosY;
							frogLifes = 3;
							frogScore = 0;
							changeScore(frogScore, 0, scoreTxt);
							for (int i = 0; i < mountOfGoals; i++)
								vGoalFrogs[i].second = false;
							break;
						case 1: // Open Highscore Screen
							actualScreen = 2;
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
			case 1: // In Game
				switch (event.type)
				{
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
					case sf::Keyboard::W:
						frogY -= frogMovDistance;
						break;
					case sf::Keyboard::S:
						if (frogY + frogMovDistance < windowHeight - 66)
							frogY += frogMovDistance;
						break;
					case sf::Keyboard::A:
						if (frogX - frogMovDistance > 0)
						{
							frogX -= frogMovDistance;
						}
						else
						{
							if (frogY > 67 && frogY < 197) // Collision with the water wall
							{
								if (frogLifes > 0)
								{
									frogX = frogOriginalPosX;
									frogY = frogOriginalPosY;
								}
								else
								{
									actualScreen = 0;
									highscores.saveScore(frogScore);
								}
								frogLifes--;
							}
						}
						break;
					case sf::Keyboard::D:
						if (frogX + frogMovDistance < windowWidht)
						{
							frogX += frogMovDistance;
						}
						else
						{
							if (frogY > 67 && frogY < 197) // Collision with the water wall
							{
								if (frogLifes > 0)
								{
									frogX = frogOriginalPosX;
									frogY = frogOriginalPosY;
								}
								else
								{
									actualScreen = 0;
									highscores.saveScore(frogScore);
								}
								frogLifes--;
							}
						}
						break;
					case sf::Keyboard::Space:
						actualScreen = 0;
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
						actualScreen = 0;
					}
				}
				break;
			}
			
		}
		
		switch (actualScreen)
		{
		case 0:
			window.clear();
			titleScreen.draw(window);
			window.display();
			break;
		case 1:
			// Checking goal
			if (frogY < frogMovDistance * 2)
			{
				int pos = (int)(frogX / frogMovDistance);

				if (pos % 2 == 1 && !vGoalFrogs[(int)(pos / 2)].second)
				{
					vGoalFrogs[(int)(pos / 2)].second = true;

					changeScore(frogScore, scorePerGoal, scoreTxt);

					if (frogScore % (scorePerGoal * 6) == 0)
						for (int i = 0; i < mountOfGoals; i++)
							vGoalFrogs[i].second = false;

					frogX = frogOriginalPosX;
					frogY = frogOriginalPosY;
				}
				else // In this case, the frog didnt reach de goal correctly
				{
					if (frogLifes > 0)
					{
						frogX = frogOriginalPosX;
						frogY = frogOriginalPosY;
					}
					else
					{
						actualScreen = 0;
						highscores.saveScore(frogScore);
					}
					frogLifes--;
				}
			}

			// Makes the Frog's translation
			if(frogLifes >= 0)
				frog.setPosition(frogX, frogY);

			// Drawing
			window.clear();
			window.draw(background);
			window.draw(scoreTxt);
			for (int i = 0; i < mountOfGoals; i++)
				if (vGoalFrogs[i].second)
					window.draw(vGoalFrogs[i].first);
			for (int i = 0; i < frogLifes; i++)
				window.draw(vLifeFrogs[i]);
			window.draw(frog);
			window.display();
			break;
		case 2:
			window.clear();
			highscores.draw(window);
			window.display();
			break;
		}
		
	}
}

/*

The objectives I didnt finish will have a (-) before
	the text, in the other case will have (+)

Objectives to complete the game:
	+ Frog dies if collides with the wall from 68 to 196 on the 'y' axis
	+ Frog wins if reaches all the goals (6)
	+ Frog loses if his LIFES are lost
	- Make 3 obstacules (1 car, 1 bus and 1 trunk)
	- Obstacules are teleported to the other part of the scene when they reach the limit of the map
	+ Make the GUI for the game (Lifes left and *if I want* the score)
	+ In case I use scores, save it in scores.dat
	+ Make a Title Screen
	- Include sounds

*/