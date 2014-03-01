/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	intro_scene.h

	IntroScene 클래스의 헤더 파일입니다.
=======================================================*/

#pragma once

#include "scene.h"

class IntroScene : public IScene {
public:
				 IntroScene();
	virtual		~IntroScene();

	bool		 HandleWindowEvent(const Event & event);
	bool		 HandleClientMsg(client_msg_t & msg) {return false;}
	void		 FrameMove();

private:
	Sprite		_ooparts_logo;
	int			_anim_count;

	bool		HandleKeyPressed(Event::KeyEvent key);
};