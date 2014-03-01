#pragma once

#include "../internal.h"
#include "../menu_item.h"
#include "../userprofile.h"

class ProfileMakeWidget : public IWidget
{
public:
			 ProfileMakeWidget(bool back_button, function<void(bool, const userprofile &)> next);
	virtual	~ProfileMakeWidget();

	bool	 HandleWindowEvent(const Event & e);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text			_label;
	EditTextItem *	_username;
	Menu			_menu;
};

class ProfileSelectWidget : public IWidget
{
public:
			 ProfileSelectWidget(bool back_button, function<void(bool, const userprofile &)> next);
	virtual	~ProfileSelectWidget();

	bool	 HandleWindowEvent(const Event & e);

	void	 Reload();

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text											_label;
	Menu											_menu;
	bool											_back_button;
	function<void(bool, const userprofile &)>		_next;
};