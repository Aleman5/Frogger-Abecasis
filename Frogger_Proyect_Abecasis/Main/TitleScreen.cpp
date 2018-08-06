#include"TitleScreen.h"

TitleScreen::TitleScreen(const int windowWidht, const int windowHeight)
	: _windowWidht(windowWidht), _windowHeight(windowHeight)
{
	init();
}
TitleScreen::~TitleScreen()
{

}
void TitleScreen::init()
{
	actualElement = 0;

	font.loadFromFile("ARCADECLASSIC.TTF");

	displayTexts[0].setFont(font);
	displayTexts[0].setCharacterSize(40);
	displayTexts[0].setFillColor(sf::Color::Green);

	for (int i = 1; i < MOUNT_OF_TEXTS; i++)
	{
		displayTexts[i].setFont(font);
		displayTexts[i].setCharacterSize(30);
		displayTexts[i].setFillColor(sf::Color::White);
	}

	displayTexts[0].setString("Play Game");
	displayTexts[1].setString("Highscores");
	displayTexts[2].setString("Exit Game");

	displayTexts[0].setPosition(_windowWidht / 2 - 100, _windowHeight / 2 - 40);
	displayTexts[1].setPosition(_windowWidht / 2 - 95, _windowHeight / 2 + 10);
	displayTexts[2].setPosition(_windowWidht / 2 - 80, _windowHeight / 2 + 50);
}
void TitleScreen::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MOUNT_OF_TEXTS; i++)
		window.draw(displayTexts[i]);
}
void TitleScreen::goUp()
{
	displayTexts[actualElement].setFillColor(sf::Color::White);
	actualElement--;
	if (actualElement == -1)
		actualElement = 2;
	displayTexts[actualElement].setFillColor(sf::Color::Green);
}
void TitleScreen::goDown()
{
	displayTexts[actualElement].setFillColor(sf::Color::White);
	actualElement++;
	if (actualElement == 3)
		actualElement = 0;
	displayTexts[actualElement].setFillColor(sf::Color::Green);
}
int TitleScreen::getActual()
{
	return actualElement;
}