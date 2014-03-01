#pragma once

#include "renderable.h"

class SkillArrow : public IRenderable
{
public:
	SkillArrow ();
	virtual ~SkillArrow ();

	virtual int GetHeight() const {return 0;}
	virtual int GetPriority() const {return PR_ARROW;}

	void SetVisible(bool b);

	void Update(int length);

protected:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	int			_renderable_id;
	bool		_visible;
	int			_length;
	Sprite		_inf_arrow;
	Sprite		_body_arrow;
	Sprite		_head_arrow;
};