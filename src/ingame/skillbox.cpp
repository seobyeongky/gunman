#include "skillbox.h"
#include "../global.h"

SkillBox::Content::Content(Keyboard::Key key)
	: _valid(false)
	, _skill()
	, _shadow(Color(0, 20, 200, 150))
	, _text()
	, _cooldown(0)
	, _me(nullptr)
	, _key(key)
{	
}

void SkillBox::Content::Reset(const wstring & sname, Champion *	me)
{
	_me = me;

	if (sname.length() == 0)
	{
		_valid = false;
		return;
	}
	smap<wstring, Ability*>::Iter it;
	if (!ability_map.find(sname, &it))
	{
		G.logger->Error(L"ability not found : %s", sname.c_str());
		_valid = false;
		return;
	}
	Ability * abil = (*it).element();
	smap<wstring, Sprite*>::Iter it2;
	if (!G.sprite_map.find(abil->icon, &it2))
	{
		G.logger->Error(L"iconf %s missing from sprite map!", sname.c_str());
		_valid = false;
		return;
	}
	Sprite * icon = (*it2).element();

	_valid = true;
	_skill = *icon;
	_text.setFont(G.default_font);
	_text.setColor(Color::White);
	_text.setPosition(Vector2f(25.f, 25.f));
	_cooldown = abil->cooltime * 1000;
}

void SkillBox::Content::Use()
{
	if (!_valid) return;

	_shadow.SetRatio(0.f);
}

void SkillBox::Content::Update()
{
	if (!_valid) return;
	int resttime = GetRestTime();
	_shadow.SetRatio(1.f - static_cast<float>(resttime) / _cooldown);
	_text.setString(to_wstring(resttime/60));
	auto bounds = _text.getLocalBounds();
	_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void SkillBox::Content::draw(RenderTarget& target, RenderStates states) const
{
	if (!_valid) return;

	states.transform *= getTransform();
	
	target.draw(_skill, states.transform);
	if (GetRestTime()  > 0)
	{
		target.draw(_shadow, states.transform);
		target.draw(_text, states.transform);
	}
}

int SkillBox::Content::GetRestTime() const
{
	skillcontext * sc = _me->skills[_key];
	if (!sc) return 0;
	return sc->cooltime.get();
}

SkillBox::SkillBox()
	: _bg(*G.sprite_map[L"skillbox"])
	, _q_item(Keyboard::Q)
	, _w_item(Keyboard::W)
	, _e_item(Keyboard::E)
{
	float begin_x = 20.f;
	float begin_y = 15.f;
	float y_interval = 59.f;
	
	_q_item.setPosition(begin_x, begin_y);
	_w_item.setPosition(begin_x, begin_y + y_interval);
	_e_item.setPosition(begin_x, begin_y + 2.f * y_interval);
}

void SkillBox::Reset(Champion * me, const wstring & q, const wstring & w, const wstring & e)
{
	_q_item.Reset(q, me);
	_w_item.Reset(w, me);
	_e_item.Reset(e, me);
}

void SkillBox::UseQSkill()
{
	_q_item.Use();
}

void SkillBox::UseWSkill()
{
	_w_item.Use();
}

void SkillBox::UseESkill()
{
	_e_item.Use();
}

void SkillBox::Update()
{
	_q_item.Update();
	_w_item.Update();
	_e_item.Update();
}

void SkillBox::draw(RenderTarget &target, RenderStates states) const
{
	target.draw(_bg, getTransform());

	target.draw(_q_item, getTransform());
	target.draw(_w_item, getTransform());
	target.draw(_e_item, getTransform());
}