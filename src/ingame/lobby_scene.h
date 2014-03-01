#pragma once

#include "../scene.h"
#include "player.h"
#include "chat_box.h"
#include "pop_msg.h"

#include <queue>

class LobbyScene : public IScene
{
public:
				 LobbyScene(const wstring & room_name,
							bool is_host,
							ID my_id);
	virtual		~LobbyScene();

	bool		 HandleWindowEvent(const Event & event);
	bool		 HandleClientMsg(client_msg_t & msg);
	void		 FrameMove();

private:
	typedef smap<ID, player_t> player_map_t;
	struct unode_t
	{
		Sprite	rect;
		Text	label;
		bool	ready;
	};
	typedef vector<unode_t *> unode_vec_t;

private:
	bool			_is_host;
	ID				_my_id;
	player_t *		_me;
	player_map_t	_player_map;

	ChatBox			_chat_box;
	PopMsg			_pop;

	Sprite			_bg;
	Sprite			_chat_frame;

	unode_vec_t		_unodes;

	Menu			_menu;

	Cleaner			_cleaner;

	void	AddPlayer(const client_t & basic_info);
	void	UpdateUnodes();
};