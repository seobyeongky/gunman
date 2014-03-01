#pragma once

#include "../internal.h"

enum
{
	PR_GROUND_UNIT = 0,
	PR_ANI00 = 1,
	PR_ARROW = 2
};

class IRenderable : public Drawable, public Transformable
{
public:
	virtual ~IRenderable(){}

	virtual int GetHeight() const = 0;
	virtual int GetPriority() const = 0;
};