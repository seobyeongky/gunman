#include "menu_item.h"
#include "util.h"

MyItem::MyItem(const wstring & string, function<void()> next)
	: _text(string, G.title_font, 52),
	_next(next)
{
	_text.setColor(Color::Black);
	SetMiddleOfLocal(&_text);
}

MyItem::~MyItem()
{
}

void MyItem::selected()
{
	_text.setColor(Color::Yellow);
	G.sfx_mgr.Play(L"data\\system\\audio\\Shield222.wav");
}

void MyItem::unselected()
{
	_text.setColor(Color::Black);
}

void MyItem::activated()
{
	G.sfx_mgr.Play(L"data\\system\\audio\\Shield2.wav");
	_next();
}

FloatRect MyItem::getLocalBounds() const
{
	return _text.getGlobalBounds();
}

FloatRect MyItem::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

bool MyItem::HandleWindowEvent(const Event & e)
{
	return false;
}

void MyItem::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_text, states);
}

EditTextItem::EditTextItem(const Font & font, unsigned int size, function<void()> next)
	: _edit_text(font, size, 8),
	_rect(*G.sprite_map[L"rect"]),
	_focused(false),
	_next(next)
{
	SetMiddleOfLocal(&_edit_text);
	_edit_text.setTextColor(Color(71, 22, 165));
	_edit_text.setCaretColor(Color(30, 12, 115));
	SetMiddleOfLocal(&_rect);
}

EditTextItem::~EditTextItem()
{
}

void EditTextItem::selected()
{
	G.sfx_mgr.Play(L"data\\system\\audio\\Shield222.wav");
	_edit_text.drawCaret(true);
}

void EditTextItem::unselected()
{
	_edit_text.drawCaret(false);
}

void EditTextItem::activated()
{
	G.sfx_mgr.Play(L"data\\system\\audio\\Shield2.wav");
	_next();
}

bool EditTextItem::HandleWindowEvent(const Event & e)
{
	SetMiddleOfLocal(&_edit_text);
	return _edit_text.HandleWindowEvent(e);
}

void EditTextItem::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_rect, states);
	target.draw(_edit_text, states);
}

FloatRect EditTextItem::getLocalBounds() const
{
	return _rect.getGlobalBounds();
}

FloatRect EditTextItem::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}