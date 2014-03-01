#pragma once

#include "../internal.h"
#include "../menu_item.h"

class RoomNameWidget : public IWidget
{
public:
			 RoomNameWidget(function<void()> next);
	virtual	~RoomNameWidget();

	bool	 HandleWindowEvent(const Event & e);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text				_label;
	EditTextItem *		_roomname;
	Menu				_menu;
	Cleaner				_cleaner;
};