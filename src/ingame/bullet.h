#pragma once

#include "../internal.h"

enum
{
	BULLET_PASS_THROUGHT = 1
};

class Bullet
{
public:
	Bullet(function<void()> callback, int flags);
//	virtual ~Bullet() {};

	function<void()>	callback;
	int					flags;
	Vector2i			pos;
	Vector2i			vel;
};