#pragma once

#include "../internal.h"

class IFinite
{
public:
	virtual ~IFinite(){}

	virtual void Consume(int amount) = 0;
	virtual bool IsDead() const = 0;
};

class SfFinite : public IFinite, public Drawable, public Transformable
{
public:
	virtual ~SfFinite(){}
};