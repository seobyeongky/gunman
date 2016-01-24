#pragma once

#include "player.h"

class ChatBox : public IWidget
{
public:
			 ChatBox();
	virtual	~ChatBox();

	void	 Reset(const player_t & me);

	void	 Update();

	void	 AddChatMsg(const Color & color, const wstring & str, const Color & color2, const wstring & str2);
	void	 AddAlarmMsg(const wstring & str);
	void	 AddInfoMsg(const wstring & info_str, const Color & color = Color(190, 120, 100));

	bool	 HandleWindowEvent(const sf::Event & e);

	void	 DrawEditText(bool flag);
	void	 ClearEditText() { _chatting_text.clear(); }

	Observable<std::wstring> the_text;

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	struct msg_t
	{
		int						time;
		pair<Text *, Text *>	text_pair;
	};

private:
	EditText					_chatting_text;
	Color						_my_color;
	wstring						_my_name;
	list<msg_t>					_msg_list;
	bool						_draw_edit_text;

	bool	 HandleKeyPressed(Event::KeyEvent key);
};