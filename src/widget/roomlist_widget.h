#pragma once

#include "../internal.h"
#include "../menu_item.h"

class RoomListWidget : public IWidget
{
public:
			 RoomListWidget(function<void()> next);
	virtual	~RoomListWidget();

	bool	 HandleWindowEvent(const Event & e);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text				_label;
	Menu				_menu;
	Cleaner				_cleaner;
};