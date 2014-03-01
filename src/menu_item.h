//
// Prebuilt item templates
//

#pragma once

#include "internal.h"

class MyItem : public IItem
{
public:
						MyItem(const wstring & string, function<void()> next);
	virtual				~MyItem();
	virtual void		selected();
	virtual void		unselected();
	virtual void		activated();
	virtual FloatRect	getLocalBounds() const;
	virtual FloatRect	getGlobalBounds() const;

	virtual bool		HandleWindowEvent(const Event & e);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text				_text;
	function<void()>	_next;
};

class EditTextItem : public IItem
{
public:
						 EditTextItem(const Font & font, unsigned int size, function<void()> next);
	virtual				~EditTextItem();
	virtual void		 selected();
	virtual void		 unselected();
	virtual void		 activated();
	virtual FloatRect	 getLocalBounds() const;
	virtual FloatRect	 getGlobalBounds() const;
	
	virtual bool		 HandleWindowEvent(const Event & e);

	wstring	 			 GetString() {return _edit_text.getString(); }

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	EditText			_edit_text;
	Sprite				_rect;
	bool				_focused;
	function<void()>	_next;
};