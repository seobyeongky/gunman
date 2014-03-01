#include "star_unit.h"
#include "../global.h"

#include <grpapi.h>

StarUnit::StarUnit()
	: _grpunit(nullptr)
	, _sprite()
	, _state(IDLE)
	, _anim_count(0)
{
}

StarUnit::StarUnit(const wstring & unitname, int player_index)
	: _grpunit(nullptr)
	, _sprite()
	, _state(IDLE)
{
	Init(unitname, player_index);
}

StarUnit::~StarUnit()
{
}

void StarUnit::Idle()
{
	if (_state != IDLE)
		_anim_count = 0;
	_state = IDLE;
}

void StarUnit::Walk()
{
	if (_state != WALK)
		_anim_count = 0;
	_state = WALK;
}

void StarUnit::Shot()
{
	if (_state != SHOT)
		_anim_count = 0;
	_state = SHOT;
}

void StarUnit::Die()
{
	if (_state != DEATH)
		_anim_count = 0;
	_state = DEATH;
}

void StarUnit::Init(const wstring & unitname, int player_index)
{
	smap<wstring, grp::unit_t*>::ConstIter it;
	if (!grp::unitmap.find(unitname, &it))
	{
		G.logger->Error(L"StarUnit : No %s at grp::unitmap)", unitname.c_str());
		return;
	}
	_grpunit = (*it).element();

	_sprite.setTexture(_grpunit->textures[player_index]);
	_sprite.setOrigin(_grpunit->frame_size.x/2.f, _grpunit->frame_size.y/2.f);
	_sprite.setScale(1.5f,1.5f);
	SetFrame(L"walk",0);
}

void StarUnit::Update()
{
	_sprite.setPosition(getPosition());

	switch (_state)
	{
	case IDLE:
		{
			SetRotatedFrame(L"walk", 0);
		}
		break;
	case WALK:
		{
			smap<wstring, vector<int>>::ConstIter it;
			if (_grpunit->frame_map.find(L"walk", &it))
			{
				const auto & frames = (*it).element();
				SetRotatedFrame(frames, _anim_count / ANIM_WEIGHT);
				_anim_count++;
				if (_anim_count >= (int)frames.size())
					_anim_count = 0;
			}
		}
		break;
	case SHOT:
		break;
	case DEATH:
		break;
	default:
		G.logger->Error(L"StarUnit invalid state : %d", _state);
	}
}

void StarUnit::draw(RenderTarget& target, RenderStates states) const
{
//	states.transform *= getTransform();
	target.draw(_sprite, states);
}

void StarUnit::SetFrame(const wstring & name, int index)
{
	smap<wstring, vector<int>>::ConstIter it;
	if (_grpunit->frame_map.find(name, &it))
	{
		_sprite.setTextureRect(grp::GetRectOf(_grpunit->frame_size, (*it).element()[index]));
	}
	else
	{
		G.logger->Warning(L"StarUnit SetFrame 실패 : 못찾음 (name:%s, index:%d)", name.c_str(), index);
	}
}

void StarUnit::GetRotationContext(int * rotation_index_ptr, bool * flip_ptr)
{
	float ratio = getRotation() / 360.f;
	int rotation_index = (int)(ratio * 32.f);
				
	bool flip = false;
	if (rotation_index >= 17)
	{
		rotation_index = 17-(rotation_index-17);
		flip = true;
	}

	*rotation_index_ptr = rotation_index;
	*flip_ptr = flip;
}

void StarUnit::SetRotatedFrame(const vector<int> & frames, int frameset_index)
{
	int rotation_index;
	bool flip;
	GetRotationContext(&rotation_index, &flip);

	if (flip)
		_sprite.setScale(-1.5f,1.5f);
	else
		_sprite.setScale(1.5f,1.5f);
	_sprite.setTextureRect(grp::GetRectOf(_grpunit->frame_size, frames[frameset_index] + rotation_index));
}

void StarUnit::SetRotatedFrame(const wstring & name, int frameset_index)
{
	smap<wstring, vector<int>>::ConstIter it;
	if (_grpunit->frame_map.find(L"walk", &it))
	{
		const auto & frames = (*it).element();
		SetRotatedFrame(frames, frameset_index);
	}
}