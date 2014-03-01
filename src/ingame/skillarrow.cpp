#include "skillarrow.h"
#include "../global.h"
#include "renderer.h"

SkillArrow::SkillArrow ()
	: _renderable_id(0)
	, _visible(true)
	, _inf_arrow(*G.sprite_map[L"arrow-inf"])
	, _body_arrow(*G.sprite_map[L"arrow-body"])
	, _head_arrow(*G.sprite_map[L"arrow-head"])
{
	_inf_arrow.setOrigin(30.f, 186.f);

	_renderable_id = Renderer::AddRenderable(this);
}

SkillArrow::~SkillArrow ()
{
	Renderer::RemoveRenderable(_renderable_id);
}

void SkillArrow::SetVisible(bool b)
{
	_visible = b;
}

void SkillArrow::Update(int length)
{
	if (!_visible) return;

	_length = length;

	if (_length >= 0)
	{
	}
}

void SkillArrow::draw(RenderTarget& target, RenderStates states) const
{
	if (!_visible) return;

	states.transform *= getTransform();

	if (_length == -1)
	{
		target.draw(_inf_arrow, states);
	}
}