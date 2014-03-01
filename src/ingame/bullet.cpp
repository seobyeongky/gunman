#include "bullet.h"

Bullet::Bullet(function<void()> callback_, int flags_)
	: callback(callback_) 
	, flags(flags_)
	, pos()
	, vel()
{}