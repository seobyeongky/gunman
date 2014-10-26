/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	edit_text.h

	EditText class의 header file입니다.
=======================================================*/

#pragma once

#include "widget.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class EditText : public IWidget {
/*	입력용 text */

public:
							  EditText(const Font & font, size_t char_size, size_t maxchar);
	virtual					 ~EditText();

	void					  drawCaret(bool b);
	void					  setCharSize(size_t char_size);
	void					  setTextColor(const Color &color);
	void					  setCaretColor(const Color &color);
	void					  setPosition(float x, float y);

	bool					  HandleWindowEvent(const Event & e);

	const sf::String &		  getString() {return _string;}

	void					  clear();

	virtual FloatRect		  getLocalBounds() const;
	virtual FloatRect		  getGlobalBounds() const;

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text			_text;
	RectangleShape	_caret;

	sf::String		_string;
	size_t			_maxchar;
	size_t			_caret_pos;

	bool			_comp;

	bool			_ignore_next_char;

	bool			_draw_caret;

	void			 updateCaret();

	bool			 handleIMEUpdated(wchar_t unicode, bool done);
	bool			 handleEnteredKey(wchar_t unicode);
	bool			 handleKeyPress(Event::KeyEvent key);

	void			 Reset();
};