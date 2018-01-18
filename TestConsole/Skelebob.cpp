#include "Skelebob.h"

Skelebob::Skelebob()
{
	texture.loadFromFile("GameData/BODY_skeleton.png");

	m_RectSourceSprite = sf::IntRect(64, 0, 64, 64);
	m_Sprite = sf::Sprite(texture, m_RectSourceSprite);
}

void Skelebob::moveUp(const bool moving)
{
	moving ? dir |= UP : dir = dir & ~UP;
}

void Skelebob::moveLeft(const bool moving)
{
	moving ? dir |= LEFT : dir = dir & ~LEFT;
}

void Skelebob::moveDown(const bool moving)
{
	moving ? dir |= DOWN : dir = dir & ~DOWN;
}

void Skelebob::moveRight(const bool moving)
{
	moving ? dir |= RIGHT : dir = dir & ~RIGHT;
}

void Skelebob::update(float elapsedTime)
{
	float m_Speed = 200;

	if(dir & UP)
	{
		m_Position.y -= m_Speed * elapsedTime;
		m_RectSourceSprite.top = 0;
	}
	if(dir & DOWN)
	{
		m_Position.y += m_Speed * elapsedTime;
		m_RectSourceSprite.top = 64 * 2;
	}
	if(dir & RIGHT)
	{
		m_Position.x += m_Speed * elapsedTime;
		m_RectSourceSprite.top = 64 * 3;
	}
	if(dir & LEFT)
	{
		m_Position.x -= m_Speed * elapsedTime;
		m_RectSourceSprite.top = 64 * 1;
	}

	m_Sprite.setPosition(m_Position);

	if(dir != 0)
	{
		if(m_Clock.getElapsedTime().asSeconds() > 0.1f)
		{
			if(m_RectSourceSprite.left == 64 * 8)
			{
				m_RectSourceSprite.left = 64;
			}
			else
			{
				m_RectSourceSprite.left += 64;
			}
			m_Clock.restart();
		}
	}
	else
	{
		m_RectSourceSprite.left = 0;
	}

	m_Sprite.setTextureRect(m_RectSourceSprite);
}
