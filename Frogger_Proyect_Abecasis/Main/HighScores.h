#ifndef HIGHSCORES
#define HIGHSCORES

#include<iostream>
#include<fstream>
#include"SFML/Graphics.hpp"

#define MOUNT_OF_TEXTS 2

using namespace std;

class Highscores
{
private:
	int _windowWidht;
	int _windowHeight;
	int actualElement;
	sf::Font font;
	sf::Text displayTexts[MOUNT_OF_TEXTS]; // Scores - Main Menu
public:
	Highscores(const int windowWidht, const int windowHeight);
	~Highscores();
	void init(); // To initialize the private variables
	void draw(sf::RenderWindow& window);
	void goUp();
	void goDown();
	int getActual();
	bool saveScore(const int score);
};
#endif // !HIGHSCORES