#include "roomname_widget.h"
#include "../global.h"
#include "../util.h"
#include "../sv/sv_service.h"
#include "../director.h"
#include "../ingame/lobby_scene.h"
#include "../net_interface.h"
#include "../userprofile.h"

RoomNameWidget::RoomNameWidget(function<void()> next)
	: _label(L":: �� �̸� ::", G.title_font, 50U),
	_roomname(new EditTextItem(G.title_font, 50U, [](){})),
	_menu(Menu::delta_t(Vector2f(0.f, 75.f), 0.f, 1.f), Keyboard::Return)
{
	_label.setPosition(0.f, 0.f);
	_label.setColor(Color(63, 161, 105));
	SetMiddleOfLocal(&_label);

	_menu.PushItem(_roomname);
	_menu.PushItem(new MyItem(L"�����", [this](){
		const wstring & roomname = _roomname->GetString();
		if (roomname.empty())
		{
			ErrorMsg(L"�� �̸��� �Է��Ͻʽÿ�.");
			return;
		}
		if (!BeginSvService(_roomname->GetString()))
		{
			ErrorMsg(L"���� ���� ����!");
			return;		
		}
		if (!NetInterface::Connect(L"localhost", profile::name))
		{
			ErrorMsg(L"���� ���� ����!");
			return;
		}
		
		_cleaner.Register(NetInterface::RegisterConnectedCallback([roomname](const server_info_t & sv_info, ID my_id){
			Director::SwitchScene(new LobbyScene(roomname, true, 0u));
		}));

		_cleaner.Register(NetInterface::RegisterConnectFailedCallback([this, roomname](){
			_cleaner.Activate();
			ErrorMsg(L"%s������ ���� ����", roomname.c_str());
		}));
	}));
	_menu.PushItem(new MyItem(L"���ư���", next));
	_menu.setPosition(0.f, 100.f);
}

RoomNameWidget::~RoomNameWidget()
{
}

bool RoomNameWidget::HandleWindowEvent(const Event & e)
{	
	Event ne;
	TransferCoord(getTransform(), e, &ne);
	return _menu.HandleWindowEvent(ne);
}

void RoomNameWidget::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_label, states);
	target.draw(_menu, states);
}