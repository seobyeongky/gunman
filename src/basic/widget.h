#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class IWidget : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~IWidget() {}

	virtual bool HandleWindowEvent(const sf::Event & e) = 0;
};