#pragma once

#include "../internal.h"
#include "cooldown_shadow.h"
#include "ability.h"
#include "champion.h"

class SkillBox : public Drawable, public Transformable
{
public:
	SkillBox();

	void Reset(Champion * me, const wstring & q, const wstring & w, const wstring & e);
	void Update();

	void UseQSkill();
	void UseWSkill();
	void UseESkill();

private:
	enum { MAX_INDEX = 3 };

	class Content : public Drawable, public Transformable
	{
	public:
		Content(Keyboard::Key key);
		void Reset(const wstring & sname, Champion * me);
		void Use(); // force use... by absoulte logic sv
		void Update();
	private:
		virtual void draw(RenderTarget& target, RenderStates states) const;
	private:
		bool			_valid;
		Sprite			_skill;
		CooldownShadow	_shadow;
		Text			_text;
		int				_cooldown;
		Champion *		_me;
		Keyboard::Key	_key;

		int				GetRestTime() const;
	};
	
private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Sprite		_bg;
	
	Content		_q_item;
	Content		_w_item;
	Content		_e_item;
};