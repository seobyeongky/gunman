#include "profile_widget.h"
#include "../global.h"
#include "../util.h"

ProfileMakeWidget::ProfileMakeWidget(bool back_button, function<void(bool, const userprofile &)> next)
	: _label(L":: 프로필 생성 (이름을 입력하세요) ::", G.title_font, 50U),
	_username(new EditTextItem(G.title_font, 50U, [](){})),
	_menu(Menu::delta_t(Vector2f(0.f, 75.f), 0.f, 1.f), Keyboard::Return)
{
	_label.setPosition(0.f, 0.f);
	_label.setColor(Color(63, 161, 105));
	SetMiddleOfLocal(&_label);

	_menu.PushItem(_username);
	_menu.PushItem(new MyItem(L"만들기", [this, next](){
		if (_username->GetString().empty())
		{
			ErrorMsg(L"이름을 입력하세요");
			return;
		}
		if (!profile::MakeNew(_username->GetString()))
		{
			G.logger->Error(L"프로필 생성 실패 (같은 이름의 유저가 있습니다.)");
			return;
		}

		userprofile theone;
		theone.name = _username->GetString();
		next(true, theone);
	}));
	if (back_button)
		_menu.PushItem(new MyItem(L"돌아가기", [next](){
			userprofile null;
			next(false, null);
	}));
	_menu.setPosition(0.f, 100.f);
}

ProfileMakeWidget::~ProfileMakeWidget()
{
}

bool ProfileMakeWidget::HandleWindowEvent(const Event & e)
{
	Event ne;
	TransferCoord(getTransform(), e, &ne);
	return _menu.HandleWindowEvent(ne);
}

void ProfileMakeWidget::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_label, states);
	target.draw(_menu, states);
}


ProfileSelectWidget::ProfileSelectWidget(bool back_button, function<void(bool, const userprofile &)> next)
	: _label(L":: 프로필을 고르십시오 ::", G.title_font, 50U)
	, _menu(Menu::delta_t(Vector2f(0.f, 75.f), 0.f, 1.f), Keyboard::Return)
	, _back_button(back_button)
	, _next(next)
{
	_label.setPosition(0.f, 0.f);
	_label.setColor(Color(63, 161, 105));
	SetMiddleOfLocal(&_label);

	Reload();

	_menu.setPosition(0.f, 75.f);
}

ProfileSelectWidget::~ProfileSelectWidget()
{
}

bool ProfileSelectWidget::HandleWindowEvent(const Event & e)
{
	Event ne;
	TransferCoord(getTransform(), e, &ne);
	return _menu.HandleWindowEvent(ne);
}

void ProfileSelectWidget::Reload()
{
	vector<userprofile> profiles;
	if (!profile::GetList(&profiles))
		G.logger->Error(L"프로필 목록 불러오기 실패");

	_menu.Clear();
	for (auto & profile : profiles)
		_menu.PushItem(new MyItem(profile.name, [this, profile](){
			_next(true, profile);
		}));

	if (_back_button)
		_menu.PushItem(new MyItem(L"돌아가기", [this](){
			userprofile null;
			_next(false, null);
		}));
}

void ProfileSelectWidget::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_label, states);
	target.draw(_menu, states);
}