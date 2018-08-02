#include"SFML/Window.hpp"
#include"SFML/Audio.hpp"
#include"SFML/Graphics.hpp"

void main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//window.draw(/* Aca iria el elemento a dibujar */);
		window.display();
	}
}