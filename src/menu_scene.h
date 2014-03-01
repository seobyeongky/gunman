/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	menu_scene.h

	MenuScene 클래스의 헤더 파일입니다.
=======================================================*/

#pragma once

#include "scene.h"
#include "widget/roomname_widget.h"
#include "widget/roomlist_widget.h"
#include "widget/profile_widget.h"

class MenuScene : public IScene {
public:
			 MenuScene();
	virtual	~MenuScene();

	bool	 HandleWindowEvent(const Event & event);
	bool	 HandleClientMsg(client_msg_t & msg);
	void	 FrameMove();

private:
	Sprite				_bg;
	Sprite				_title;
	
	IWidget	*			_widget;

	IWidget *			_first_profile_widget;

	Menu				_mainmenu;
	RoomNameWidget		_roomname_widget;
	RoomListWidget		_roomlist_widget;
	ProfileMakeWidget	_profile_make_widget;
	ProfileSelectWidget	_profile_select_widget;

	vector<Sprite *>	_clouds;

	bool HandleKeyPressed(Event::KeyEvent key);
};