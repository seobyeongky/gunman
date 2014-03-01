#include "roomlist_widget.h"
#include "../global.h"
#include "../util.h"
#include "../net_interface.h"
#include "../director.h"
#include "../ingame/lobby_scene.h"
#include "../userprofile.h"

#include <sstream>

RoomListWidget::RoomListWidget(function<void()> next)
	: _label(L"::방 목록::", G.title_font, 50U)
	, _menu(Menu::delta_t(Vector2f(0, 70), 0.f, 1.f), Keyboard::Return)
{
	_label.setPosition(0.f, 0.f);
	_label.setColor(Color(63, 161, 105));
	SetMiddleOfLocal(&_label);
	
	_menu.PushItem(new MyItem(L"돌아가기", next));

	_menu.setPosition(0.f, 75.f);

	_cleaner.Register(NetInterface::RegisterServerListUpdate([this, next](const vector<server_info_t> & servers){
		_menu.Clear();

		for (auto & server : servers)
		{
			wstringstream ss;
			ss << server.name
				<< L" - " << server.address
				<< L" (" << server.num_of_clients << L"/" << server.max_num_of_clients << L")";
			_menu.PushItem(new MyItem(ss.str(), [this, server, next](){
				if (!NetInterface::Connect(server.address, profile::name))
					return;

				_cleaner.Register(NetInterface::RegisterConnectedCallback([server](const server_info_t & sv_info, ID my_id){
					Director::SwitchScene(new LobbyScene(server.name, false, my_id));
				}));

				_cleaner.Register(NetInterface::RegisterConnectFailedCallback([this, server](){
					_cleaner.Activate();
					ErrorMsg(L"%s서버로 접속 실패", server.address);
				}));
			}));
		}

		_menu.PushItem(new MyItem(L"돌아가기", next));
	}));
}

RoomListWidget::~RoomListWidget()
{
}

bool RoomListWidget::HandleWindowEvent(const Event & e)
{
	Event ne;
	TransferCoord(getTransform(), e, &ne);
	return _menu.HandleWindowEvent(ne);
}

void RoomListWidget::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_label, states);
	target.draw(_menu, states);
}