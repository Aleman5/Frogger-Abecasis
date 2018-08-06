#include"Highscores.h"

Highscores::Highscores(const int windowWidht, const int windowHeight)
	: _windowWidht(windowWidht), _windowHeight(windowHeight)
{
	init();
}
Highscores::~Highscores()
{

}
void Highscores::init()
{
	actualElement = 0;

	font.loadFromFile("ARCADECLASSIC.TTF");

	displayTexts[0].setFont(font);
	displayTexts[0].setCharacterSize(40);
	displayTexts[0].setString("Scores");
	displayTexts[0].setFillColor(sf::Color::Blue);
	displayTexts[0].setPosition(_windowWidht / 2 - 78, 60);

	displayTexts[1].setFont(font);
	displayTexts[1].setCharacterSize(30);
	displayTexts[1].setString("Main Menu");
	displayTexts[1].setFillColor(sf::Color::Green);
	displayTexts[1].setPosition(132, _windowHeight - 60);
}
void Highscores::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MOUNT_OF_TEXTS; i++)
		window.draw(displayTexts[i]);
}
void Highscores::goUp()
{
	displayTexts[actualElement].setFillColor(sf::Color::White);
	actualElement--;
	if (actualElement == -1)
		actualElement = MOUNT_OF_TEXTS;
	displayTexts[actualElement].setFillColor(sf::Color::Green);
}
void Highscores::goDown()
{
	displayTexts[actualElement].setFillColor(sf::Color::White);
	actualElement++;
	if (actualElement == 3)
		actualElement = 0;
	displayTexts[actualElement].setFillColor(sf::Color::Green);
}
int Highscores::getActual()
{
	return actualElement;
}
bool Highscores::saveScore(const int score)
{
	ofstream archivo;
	try
	{
		archivo.open("scores.dat", ios::app);
		if (!archivo.good())
			throw ios::failure("Couldnt open: scores.dat");
		archivo << score << endl;
		archivo.close();
	}
	catch (ios::failure f)
	{
		cerr << f.what();
	}
}