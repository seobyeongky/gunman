#pragma once

#include "../internal.h"
#include "../starcraft/star_unit.h"
#include "renderable.h"
#include "ability.h"

struct skillcontext
{
	skillcontext(Ability * abil_) : ability(abil_), cooltime(0) {}
	Ability	*		ability;
	Observable<int>	cooltime;
};

class Champion : public IRenderable
{
public:
	Champion(int player_index);
	virtual ~Champion();

	virtual int GetHeight() const {return 0;}
	virtual int GetPriority() const {return PR_GROUND_UNIT;}

	const Vector2i & GetPos() const {return _pos;}
	void SetPos(const Vector2i & pos);

	int GetSize() const {return 20;}

	void RunCommandGo(const Vector2i & pos);
//	void RunCommandSkill(const );
	void Update();

	smap<Keyboard::Key, skillcontext*>	skills;

protected:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	enum {
		DISTPF = 3
	};

private:
	StarUnit	_ghost;
	Vector2i	_pos;
	Vector2i	_dest;
	int			_renderable_id;
};