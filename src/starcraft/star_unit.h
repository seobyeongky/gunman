#pragma once

#include "../internal.h"
#include "grp_loader.h"

// fixme : Dangling pointer에 취약함...
class StarUnit : public Drawable, public Transformable
{
public:
			 StarUnit();
			 StarUnit(const wstring & unitname, int player_index);
	virtual	~StarUnit();

	void	 Init(const wstring & unitname, int player_index);

	void	 Idle();
	void	 Walk();
	void	 Shot();
	void	 Die();

	void	 Update();

protected:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	enum state_t
	{
		IDLE,
		WALK,
		SHOT,
		DEATH
	};

private:
	const grp::unit_t * _grpunit;
	Sprite				_sprite;
	state_t				_state;

	enum
	{
		ANIM_WEIGHT = 1
	};

	int					_anim_count;

	void SetFrame(const wstring & name, int index);
	void GetRotationContext(int * rotation_index_ptr, bool * flip_ptr);
	void SetRotatedFrame(const vector<int> & frames, int frameset_index);
	void SetRotatedFrame(const wstring & name, int frameset_index);
};