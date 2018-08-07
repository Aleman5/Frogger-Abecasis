#include"Highscores.h"

void Highscores::updateScoresFile()
{
	ofstream archivoScores;
	try
	{
		archivoScores.open("scores.dat", ios::out | ios::binary | ios::trunc);

		for (int i = 0; i < MOUNT_OF_SCORES; i++)
		{
			int actualScore = lisScores.front();
			archivoScores << actualScore << endl << endl;// Updating scores.dat
			scores[i].setString(to_string(actualScore)); // Updating texts
			lisScores.pop_front();
			lisScores.push_back(actualScore);
		}

		archivoScores.close();
	}
	catch (ios::failure f)
	{
		cerr << f.what();
	}
}

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
	displayTexts[0].setPosition(_windowWidht / 2 - 78, 50);

	displayTexts[1].setFont(font);
	displayTexts[1].setCharacterSize(30);
	displayTexts[1].setString("Main Menu");
	displayTexts[1].setFillColor(sf::Color::Green);
	displayTexts[1].setPosition(132, _windowHeight - 60);

	for (int i = 0; i < MOUNT_OF_SCORES; i++)
	{
		scores[i].setFont(font);
		scores[i].setCharacterSize(25);
		scores[i].setFillColor(sf::Color::White);
		scores[i].setPosition(140, i * 32 + 85);
	}

	//char* scoreA = new char();
	int scoreAux = 0;
	try
	{
		archivoScores.open("scores.dat", ios::in | ios::binary);
		if (!archivoScores.good()) throw ios::failure("Couldn't open scores.dat");

		for (int i = 0; i < MOUNT_OF_SCORES; i++)
		{
			//archivoScores.getline((char*)scoreA, sizeof(scoreAux));
			//scoreAux = *scoreA;
			archivoScores >> scoreAux;
			lisScores.push_back(scoreAux);
		}
		archivoScores.close();
	}
	catch (ios::failure f)
	{
		cerr << f.what();
	}
	//delete scoreA;

	for (int i = 0; i < MOUNT_OF_SCORES; i++)
	{
		int actualScore = lisScores.front();
		scores[i].setString(to_string(actualScore)); // Updating texts
		lisScores.pop_front();
		lisScores.push_back(actualScore);
	}
}
void Highscores::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MOUNT_OF_TEXTS; i++)
		window.draw(displayTexts[i]);

	for (int i = 0; i < MOUNT_OF_SCORES; i++)
	{
		window.draw(scores[i]);
	}
		
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
	lisScores.push_front(score);
	lisScores.sort();
	lisScores.reverse();
	lisScores.pop_back();

	updateScoresFile();
}