#ifndef TITLE_SCREEN
#define TITLE_SCREEN

#include"SFML/Graphics.hpp"

#define MOUNT_OF_TEXTS 3

class TitleScreen
{
private:
	int _windowWidht;
	int _windowHeight;
	int actualElement;
	sf::Texture textIcon;
	sf::Sprite icon;
	sf::Font font;
	sf::Text title;
	sf::Text displayTexts[MOUNT_OF_TEXTS]; // Play - Highscores - Exit
public:
	TitleScreen(const int windowWidht, const int windowHeight);
	~TitleScreen();
	void init(); // To initialize the private variables
	void draw(sf::RenderWindow& window);
	void goUp();
	void goDown();
	int getActual();
};
#endif // !TITLE_SCREEN