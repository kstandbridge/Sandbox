#include "SFML/Graphics.hpp"

#include "Skelebob.h"


int main(int argc, char** argv)
{
	sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Skelebob");

	sf::Event event{};
	Skelebob skelebob;
	
	sf::Clock deltaClock;

	while(renderWindow.isOpen())
	{
		sf::Time delta = deltaClock.restart();
		float elapsedTime = delta.asSeconds();
		
		while(renderWindow.pollEvent(event))
		{
			if(event.type == sf::Event::EventType::Closed)
			{
				renderWindow.close();
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				renderWindow.close();
			}

			skelebob.moveUp(sf::Keyboard::isKeyPressed(sf::Keyboard::W));
			skelebob.moveLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::A));
			skelebob.moveDown(sf::Keyboard::isKeyPressed(sf::Keyboard::S));
			skelebob.moveRight(sf::Keyboard::isKeyPressed(sf::Keyboard::D));
		}

		skelebob.update(elapsedTime);

		renderWindow.clear();
		renderWindow.draw(skelebob.getSprite());
		renderWindow.display();
	}
}
