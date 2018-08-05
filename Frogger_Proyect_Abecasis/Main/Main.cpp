#include<iostream>
#include"SFML/Window.hpp"
#include"SFML/Audio.hpp"
#include"SFML/Graphics.hpp"

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

void main()
{
	// The Frog has to move 12 times to reach the goal, plus the place where appears, we get 13 possible positions
	//  in the 'y' axis where the frog can stay. So 13 x 32 = 416 + 64 = 480. 'y' axis.
	// Where 40 is maxium height of the sprites. Those 60 extra pixels are for GUI staff.
	// The times that the Frog can move horizontally is also 13. So 13 x 32 = 416. 'x' axis.
	int windowWidht = 416;
	int windowHeight = 480;
	sf::RenderWindow window(sf::VideoMode(windowWidht, windowHeight), "SFML works!");

	// Textures
	sf::Texture tBackground;
	loadTexture(tBackground, "Background.png");
	sf::Texture tFrog;
	loadTexture(tFrog, "Frog.png");

	// Sprites
	sf::Sprite background;
	background.setTexture(tBackground);
	sf::Sprite frog;
	frog.setTexture(tFrog);
	frog.setTextureRect(sf::IntRect(0,0,24,24));

	// Variables
	int frogX = 196;
	int frogY = 388;
	int frogMovDistance = 32;
	//int posX = 0; // '0-12'
	//int posY = 0; // '0-12'
	bool isMoving = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::W)
				{
					if (frogY - frogMovDistance > 0)
						frogY -= frogMovDistance;
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					if (frogY + frogMovDistance < windowHeight - 66)
						frogY += frogMovDistance;
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					if (frogX - frogMovDistance > 0)
						frogX -= frogMovDistance;
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					if(frogX + frogMovDistance < windowWidht)
						frogX += frogMovDistance;
				}
				break;

			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		
		frog.setPosition(frogX, frogY);

		cout << frogX << "  " << frogY<<endl; // Just for testing purpoces

		window.clear();
		window.draw(background);
		window.draw(frog);
		window.display();
	}
}

/*

The objectives I didnt finish will have a (-) before 
	the text, in the other case will have (+)

Objectives to complete the game:
	- Frog dies if collides with the wall from 68 to 196 on the 'y' axis	
	- Frog wins is reaches the goal 6 times (1 for each hole)
	- Frog loses if his 3 LIFES are lost	
	- Make 4 obstacules (1 car, 1 bus, 1 faster car and 1 trunk)			
	- Obstacules are teleported to the other part of the scene when they reach the limit of the map
	- Make the GUI for the game (Lifes left and *if I want* the score)	
	- In case I use scores, save it in scores.dat	
	- Make a Title Screen	

*/



// ---------------- --------------- ----------------

// I let this here in case of using it in the future

// ---------------- --------------- ----------------


// This moves the Frog if 'isMoving' is true
/*if (isMoving)
{
if (dirX != 0)
{
frog.setPosition(frogX + frogMovVel * dirX, frogY); // Moving in 'x' axis

if (dirX > 0) // Going Right
{
if (destination >= frogX)
{
frogX = destination;
isMoving = false;
}

}
else // Going Left
{
if (destination <= frogX)
{
frogX = destination;
isMoving = false;
}
}
}
else if (dirY != 0)
{
frog.setPosition(frogX, frogY + frogMovVel * dirY); // Moving in 'y' axis

if (dirY > 0) // Going Down
{
if (destination >= frogY)
{
frogY = destination;
isMoving = false;
}
}
else // Going Up
{
if (destination <= frogY)
{
frogY = destination;
isMoving = false;
}
}
}
else // I'm putting this else in case of dirX and dirY are both 0.
isMoving = false;
}*/