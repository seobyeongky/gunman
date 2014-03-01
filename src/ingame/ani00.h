// 클릭할 때 화면에 나타나는 애니메이션

#pragma once

#include "../internal.h"
#include "finite.h"
#include "renderable.h"

class Ani00 : public IFinite, public IRenderable
{
public:
	Ani00();
	virtual ~Ani00();

	virtual void Consume(int amount);
	virtual bool IsDead() const;

	virtual int GetHeight() const {return 0;}
	virtual int GetPriority() const {return PR_ANI00;}

protected:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	enum
	{
		LIFETIME = 30,
		WIDTH = 55,
		HEIGHT = 55,
		NR_FRAMES = 4
	};


private:
	Sprite	_sprite;
	int		_lifetime;
	int		_renderable_id;

	void	Update();
};