#include "intro_scene.h"
#include "util.h"
#include "asset.h"
#include "director.h"
#include "global.h"
#include "menu_scene.h"

#include <sstream>

IntroScene::IntroScene()
	: _ooparts_logo(*G.sprite_map[L"ooparts-logo"]),
	_anim_count(0){
	auto winsize = G.window.getSize();

	SetMiddleOfLocal(&_ooparts_logo);
	SetMiddleOfScreen(&_ooparts_logo);
	SetScaleByWidth(&_ooparts_logo, winsize.x / 3.f);

	G.sfx_mgr.Play(L"data\\system\\audio\\intro.wav");
}

IntroScene::~IntroScene()
{
}

bool IntroScene::HandleWindowEvent(const Event & event)
{
	if (event.type == Event::KeyPressed)
		return HandleKeyPressed(event.key);
	else
		return false;
}

bool IntroScene::HandleKeyPressed(Event::KeyEvent key)
{
	if (_anim_count < 200) _anim_count = 200;
	return true;
}

void IntroScene::FrameMove()
{
	if (_anim_count < 30)
	{
		int opacity = _anim_count * 255 / 30;
		Color nextcolor(255, 255, 255, opacity);
		_ooparts_logo.setColor(nextcolor);
	}
	else if (_anim_count >= 200)
	{
		int opacity = (230 - _anim_count) * 255 / 30;
		Color nextcolor(255, 255, 255, opacity);
		_ooparts_logo.setColor(nextcolor);		
	}

	if (_anim_count == 230)
		Director::SwitchScene(new MenuScene());

	++_anim_count;

	G.window.clear(Color::White);
	G.window.draw(_ooparts_logo);
}