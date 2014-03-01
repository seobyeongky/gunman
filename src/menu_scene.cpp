#include "menu_scene.h"
#include "util.h"
#include "asset.h"
#include "director.h"
#include "global.h"
#include "ingame/play_scene.h"
#include "sv\sv_service.h"
#include "net_interface.h"
#include "menu_item.h"
#include "userprofile.h"

#include <sstream>

MenuScene::MenuScene()
	: _bg(*G.sprite_map[L"mountains-copy"])
	, _title(*G.sprite_map[L"title-logo"])
	, _widget(nullptr)
	, _first_profile_widget(nullptr)
	, _mainmenu(Menu::delta_t(Vector2f(0.f, 75.f), 0.f, 1.f), Keyboard::Return)
	, _roomname_widget([this](){
		_widget = &_mainmenu;
})
	, _roomlist_widget([this](){
		_widget = &_mainmenu;
	})
	, _profile_make_widget(true, [this](bool, const userprofile &){
		_widget = &_mainmenu;
	})
	, _profile_select_widget(true, [this](bool setted, const userprofile & me){
		if (setted)
			profile::Set(me);
		_widget = &_mainmenu;
	})
{
	G.window.setTitle(L"사격의 달인");

	FitScaleToScreen(&_bg);
	
	if (G.bg_music) G.bg_music->play();
	
	auto & winsize = G.window.getSize();

	_title.setPosition(winsize.x / 2.f, winsize.y / 3.f);
	SetMiddleOfLocal(&_title);

	_mainmenu.PushItem(new MyItem(L"방 만들기", [this](){
		_widget = &_roomname_widget;
	}));
	_mainmenu.PushItem(new MyItem(L"방 목록 보기", [this](){
		_widget = &_roomlist_widget;
	}));
	_mainmenu.PushItem(new MyItem(L"다른 프로필 선택", [this](){
		_profile_select_widget.Reload();
		_widget = &_profile_select_widget;
	}));
	_mainmenu.PushItem(new MyItem(L"새 프로필 만들기", [this](){
		_widget = &_profile_make_widget;
	}));
	_mainmenu.PushItem(new MyItem(L"게임 종료", [](){
		Director::End();
	}));

	_mainmenu.setPosition(winsize.x / 2.f, winsize.y / 2.f);
	_roomname_widget.setPosition(winsize.x / 2.f, winsize.y / 2.f);
	_roomlist_widget.setPosition(winsize.x / 2.f, winsize.y / 2.f);
	_profile_make_widget.setPosition(winsize.x / 2.f, winsize.y / 2.f);
	_profile_select_widget.setPosition(winsize.x / 2.f, winsize.y / 2.f);

	// Main Widget
	vector<userprofile> profiles;
	if (!profile::GetList(&profiles))
		G.logger->Error(L"프로필 목록 얻어오기 실패!");

	if (profiles.empty())
		_first_profile_widget = new ProfileMakeWidget(false, [this](bool setted, const userprofile & me){
			assert(setted);
			profile::Set(me);
			_widget = &_mainmenu;
		});
	else
		_first_profile_widget = new ProfileSelectWidget(false, [this](bool setted, const userprofile & me){
			assert(setted);
			profile::Set(me);
			_widget = &_mainmenu;
		});
	_first_profile_widget->setPosition(winsize.x / 2.f, winsize.y / 2.f);
	_widget = _first_profile_widget;

	for (int i = 0; i < 10; i++)
	{
		wstringstream ss;
		ss << L"cloud" << (i % 3);
		Sprite * new_cloud = new Sprite(*G.sprite_map[ss.str()]);
		new_cloud->setPosition(
			static_cast<float>(rand() % (5*winsize.x)) - 2.5f*winsize.x
			, static_cast<float>(rand() % (winsize.y/6))
			);
		_clouds.push_back(new_cloud);
	}
}

MenuScene::~MenuScene()
{
	if (_first_profile_widget) delete _first_profile_widget;
}

bool MenuScene::HandleWindowEvent(const Event & event)
{
	if (_widget)
		return  _widget->HandleWindowEvent(event);
	else
		return false;
}

bool MenuScene::HandleClientMsg(client_msg_t & msg)
{
	return false;
}

bool MenuScene::HandleKeyPressed(Event::KeyEvent key)
{
	return true;
}

void MenuScene::FrameMove()
{
	auto & winsize = G.window.getSize();

	G.window.draw(_bg);

	for (auto & cloud : _clouds)
	{
		auto & pos = cloud->getPosition();
		cloud->setPosition(pos.x+G.delta_time/10.f, pos.y);
		if (pos.x > G.window.getSize().x + winsize.x)
		{
			cloud->setPosition(-static_cast<float>(winsize.x), pos.y);
		}
		G.window.draw(*cloud);
	}

	G.window.draw(_title);
	
	if (_widget)
		G.window.draw(*_widget);
}