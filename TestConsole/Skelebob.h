#pragma once
#include "SFML/Graphics.hpp"

class Skelebob
{
	enum Direction
	{
		UP = 1 << 0, 
		LEFT = 1 << 1, 
		DOWN = 1 << 2, 
		RIGHT = 1 << 3
	};
	
	uint8_t dir = 0;

	sf::IntRect m_RectSourceSprite;
	sf::Texture texture;
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;
	sf::Clock m_Clock;
public:
	Skelebob();

	const sf::Sprite& getSprite() const { return m_Sprite; }

	void moveUp(bool moving);
	void moveLeft(bool moving);
	void moveDown(bool moving);
	void moveRight(bool moving);

	void update(float elapsedTime);
};