#include "champion.h"
#include "coord.h"
#include "renderer.h"

Champion::Champion(int player_index)
	: skills()
	, _ghost(L"ghost", player_index)
	, _pos()
	, _dest()
	, _renderable_id(0)
{
	_renderable_id  = Renderer::AddRenderable(this);
}

Champion::~Champion()
{
	Renderer::RemoveRenderable(_renderable_id);
}

void Champion::SetPos(const Vector2i & pos)
{
	_pos = pos;
	_dest = _pos;

	Vector2f fpos((float)_pos.x, (float)_pos.y);
	setPosition(fpos);
}

void Champion::RunCommandGo(const Vector2i & pos)
{
	_dest = pos;
}

void Champion::Update()
{
	int rest_dist = DISTPF;

	Vector2i prev_pos = _pos;

	int dx = _dest.x - _pos.x;
	int dy = _dest.y - _pos.y;

	if (dx != 0 || dy != 0)
	{
		// ratio = (abs(dx) + abs(dy)) / DISTPF;
		int ratio_factor = abs(dx) + abs(dy);
		int ratio_divider = ratio_factor / DISTPF;
		if (ratio_divider != 0)
		{
			dx /= ratio_divider;
			dy /= ratio_divider;
		}

		_pos.x += dx;
		_pos.y += dy;

		Vector2f fdpos((float)dx, (float)dy);
		fdpos = GetUnitVector(fdpos);
		_ghost.setRotation(GetDir(fdpos));
		_ghost.Walk();
	}
	else
	{
		_ghost.Idle();
	}

	_ghost.Update();

	Vector2f fpos((float)_pos.x, (float)_pos.y);
	setPosition(fpos);
}

void Champion::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_ghost, states);
}