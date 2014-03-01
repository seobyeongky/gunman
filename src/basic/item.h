#pragma once

#include "widget.h"

class IItem : public IWidget
{
public:
	virtual void			selected() = 0;
	virtual void			unselected() = 0;
	virtual void			activated() = 0;
	virtual sf::FloatRect	getLocalBounds() const = 0;
	virtual sf::FloatRect	getGlobalBounds() const = 0;
};