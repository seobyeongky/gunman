#include "lobby_scene.h"
#include "../global.h"
#include "../net_interface.h"
#include "../sv/sv_service.h"
#include "../util.h"
#include "../logic.h"
#include "../userprofile.h"
#include "../menu_item.h"
#include "../director.h"
#include "../menu_scene.h"
#include "../colors.h"
#include "../map_list.h"
#include "play_scene.h"


#include <gl\GL.h>

LobbyScene::LobbyScene(const wstring & room_name,
					   bool is_host, ID my_id)
	: _is_host(is_host)
	, _my_id(my_id)
	, _chat_box()
	, _pop()
	, _bg(*G.sprite_map[L"dune21"])
	, _chat_frame(*G.sprite_map[L"frame01"])
	, _unodes()
	, _menu()
	, _map_list()
	, _cleaner()
{
	G.window.setTitle(L"사격의 달인 - " + room_name + L"방");

	_player_map[_my_id] = player_t(profile::name, colors::GetPlayerColor(_my_id));
	_me = &_player_map[_my_id];

	_cleaner.Register(NetInterface::RegisterClientIntroCallback([this](const client_t & cl_info)
	{
		G.sfx_mgr.Play(L"data\\system\\audio\\come.wav");
		AddPlayer(cl_info);
	}));

	_cleaner.Register(NetInterface::RegisterClientGoneCallback([this](const client_t & cl_info)
	{
		_player_map.erase(cl_info.id);
		UpdateUnodes();
	}));

	_cleaner.Register(NetInterface::RegisterPacketCallback(SV_TO_CL_CHAT, [this](Packet & packet)
	{
		ID cl_id;
		wstring msg;
		if (!(packet >> cl_id >> msg)) return;
		auto & player = _player_map[cl_id];
		_chat_box.AddChatMsg(player.color, player.name,
			Color(235, 235, 235), msg);
		G.sfx_mgr.Play(L"data\\system\\audio\\chatmsg.mp3");
	}));

	_cleaner.Register(NetInterface::RegisterPacketCallback(SV_TO_CL_WILL_START, [this, room_name](Packet & packet)
	{
		_pop.Show(L"곧 시작합니다");
		G.sfx_mgr.Play(L"data\\system\\audio\\영화+효과음_02.mp3");
		wstring map;
		packet >> map;
		Director::SetTimeout(2000, [this, room_name, map](){
			Director::SwitchScene(new PlayScene(room_name, _is_host, _my_id, _player_map, map));
		});
	}));

	auto winsize = G.window.getSize();

	_chat_box.Reset(*_me);
	_chat_box.setPosition(
		.5f*winsize.x-(350.f-60.f)/230.f*.4f*.5f*winsize.y,
		.88f*winsize.y);
	_chat_box.DrawEditText(true);

	dbgpoint.setPosition(
		.5f*winsize.x-(350.f-60.f)/230.f*.4f*.5f*winsize.y,
		.88f*winsize.y);

	SetScaleToSize(&_bg, (float)winsize.x, (float)winsize.y);

	_chat_frame.setOrigin(350.f/2,230.f/2);
	SetScaleByHeight(&_chat_frame, .4f*winsize.y);
	_chat_frame.setPosition(.5f*winsize.x, .75f*winsize.y);

	UpdateUnodes();

	_menu.setPosition(.9f*winsize.x, .9f*winsize.y);
	_menu.SetDelta(Menu::delta_t(Vector2f(0, -.15f*winsize.y), 0, 1));
	_menu.PushItem(new MyItem(L"나가기", [this](){
		DisconnectToServer();
		if (_is_host)
			EndSvService();
		Director::SwitchScene(new MenuScene());
	}));
	if (is_host)
	{
		_menu.PushItem(new MyItem(L"시작", [this](){
			if (_map.empty())
			{
				ErrorMsg(L"맵을 선택하십시오.");
				return;
			}
			Packet sendpacket;
			sendpacket << TO_UINT16(CL_TO_SV_START) << _map;
			SafeSend(sendpacket);
		}));
	}

	if (_is_host)
	{
		vector<wstring> map_list;
		MapList::GetList(&map_list);
		for (auto & map_name : map_list)
		{
			_map_list.PushItem(new MyItem(map_name, [this,map_name](){
				_map = map_name;
			}));
		}
	}
}

LobbyScene::~LobbyScene()
{
	for (auto unode : _unodes) delete unode;
}

bool LobbyScene::HandleWindowEvent(const Event & e)
{
	if (e.type == Event::TextEntered)
	{
		if (e.key.code == UNICODE_ESC)
		{
			_chat_box.ClearEditText();
			return true;
		}
	}
	if (_menu.HandleWindowEvent(e)) return true;
	if (_map_list.HandleWindowEvent(e)) return true;
	return _chat_box.HandleWindowEvent(e);
}

void LobbyScene::FrameMove()
{
	auto winsize = G.window.getSize();

	_chat_box.Update();
	_pop.Update();

	G.window.draw(_bg);

	for (auto & unode : _unodes)
	{
		G.window.draw(unode->rect);
		G.window.draw(unode->label);
	}

	G.window.draw(_chat_frame);

	glScissor((int)(.5f*winsize.x-(350.f-35.f)/230.f*.4f*.5f*winsize.y)
		, (int).5f*winsize.y
		, static_cast<GLint>(.87f*_chat_frame.getGlobalBounds().width),
		static_cast<GLint>(.95f*_chat_frame.getGlobalBounds().height));
	glEnable(GL_SCISSOR_TEST);
	G.window.draw(_chat_box);
	glDisable(GL_SCISSOR_TEST);

	if (_is_host)
		G.window.draw(_map_list);
	G.window.draw(_menu);
	G.window.draw(_pop);
}

void LobbyScene::AddPlayer(const client_t & basic_info)
{
	G.logger->Info(L"Add player : [%d] %s", basic_info.id, basic_info.name.c_str());

	_player_map.insert(basic_info.id, player_t(basic_info.name, colors::GetPlayerColor(basic_info.id)));
	
	UpdateUnodes();
}

void LobbyScene::UpdateUnodes()
{
	auto winsize = G.window.getSize();

	for (auto & unode : _unodes) delete unode;
	_unodes.clear();

	float x = 0.f;
	float y = 0.f;
	int i = 0;
	for (auto & player : _player_map)
	{
		if (i > 0 && i % 5 == 0)
		{
			x += .35f*winsize.x;
			y = 0.f;
		}

		unode_t * newone = new unode_t();
		newone->rect = *G.sprite_map[L"frame02"];
		newone->label.setFont(G.title_font);
		newone->label.setCharacterSize(35U);

		String text = player.element().name;
		newone->label.setString(text);
		newone->label.setPosition(x + .15f*winsize.x + .02f*winsize.x, .05f*winsize.y + y + .02f*winsize.y);

		SetScaleToSize(&newone->rect, .3f*winsize.x, .1f*winsize.y);
		newone->rect.setPosition(x + .15f*winsize.x, .05f*winsize.y + y);
		
		_unodes.push_back(newone);

		y += .12f*winsize.y;
		i++;
	}
}