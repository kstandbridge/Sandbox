#include "SFML/Graphics.hpp"

int main(int argc, char** argv)
{
	sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Skelebob");

	sf::Event event{};
	sf::Texture texture;
	texture.loadFromFile("GameData/BODY_skeleton.png");

	sf::IntRect rectSourceSprite(64, 0, 64, 64);
	sf::Sprite sprite(texture, rectSourceSprite);
	sf::Clock clock;
	sf::Clock deltaClock;

	sf::Vector2f position;
	enum Direction {STAND, UP, LEFT, DOWN, RIGHT};
	auto direction = STAND;
	while(renderWindow.isOpen())
	{
		sf::Time delta = deltaClock.restart();
		float elapsedTime = delta.asSeconds();
		float m_Speed = 200;

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
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				direction = UP;
				rectSourceSprite.top = 0;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				direction = LEFT;
				rectSourceSprite.top = 64 * 1;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				direction = DOWN;
				rectSourceSprite.top = 64 * 2;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				direction = RIGHT;
				rectSourceSprite.top = 64 * 3;
			} 
			else
			{
				direction = STAND;
			}
		}

		sprite.setPosition(position);

		switch(direction) 
		{
			case UP: 
				position.y -= m_Speed * elapsedTime;
				break;
			case LEFT: 
				position.x -= m_Speed * elapsedTime;
				break;
			case DOWN:
				position.y += m_Speed * elapsedTime;
				break;
			case RIGHT: 
				position.x += m_Speed * elapsedTime;
				break;
			case STAND: 
			default: 
				break;
		}

		if(direction != STAND)
		{
			if(clock.getElapsedTime().asSeconds() > 0.1f)
			{
				if(rectSourceSprite.left == 64 * 8)
				{
					rectSourceSprite.left = 64;
				}
				else
				{
					rectSourceSprite.left += 64;
				}
				clock.restart();
			}
		}
		else
		{
			rectSourceSprite.left = 0;
		}

		sprite.setTextureRect(rectSourceSprite);

		renderWindow.clear();
		renderWindow.draw(sprite);
		renderWindow.display();
	}
}
