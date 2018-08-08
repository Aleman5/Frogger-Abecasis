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

	textIcon.loadFromFile("Frog.png");

	icon.setTexture(textIcon);
	icon.setPosition(_windowWidht / 2 - 78, 32);
	icon.scale(6, 6);

	texControls.loadFromFile("Controls.png");

	controls.setTexture(texControls);
	controls.setPosition(_windowWidht - 110, _windowHeight - 110);

	font.loadFromFile("ARCADECLASSIC.TTF");

	title.setFont(font);
	title.setCharacterSize(60);
	title.setFillColor(sf::Color::Red);
	title.setString("FROGGER");
	title.setPosition(86, 160);

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

	displayTexts[0].setPosition(_windowWidht / 2 - 97, _windowHeight / 2);
	displayTexts[1].setPosition(_windowWidht / 2 - 92, _windowHeight / 2 + 50);
	displayTexts[2].setPosition(_windowWidht / 2 - 77, _windowHeight / 2 + 90);
}
void TitleScreen::draw(sf::RenderWindow& window)
{
	window.draw(icon);
	window.draw(controls);
	window.draw(title);
	

	for (int i = 0; i < MOUNT_OF_TEXTS; i++)
		window.draw(displayTexts[i]);
}
void TitleScreen::goUp()
{
	displayTexts[actualElement].setFillColor(sf::Color::White);
	actualElement--;
	if (actualElement == -1)
		actualElement = MOUNT_OF_TEXTS - 1;
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