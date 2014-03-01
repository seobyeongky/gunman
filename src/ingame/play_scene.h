#pragma once

#include "../scene.h"
#include "player.h"
#include "chat_box.h"
#include "pop_msg.h"
#include "../starcraft/star_unit.h"
#include "finite.h"
#include "../proto/command.pb.h"
#include "champion.h"
#include "renderable.h"
#include "bullet.h"
#include "skillbox.h"
#include "skillarrow.h"

#include <queue>

// test tmx parsing
#include <Tmx.h>
#include "tilemap.h"

class PlayScene : public IScene
{
public:
				 PlayScene(	const wstring & room_name,
							bool is_host,
							ID my_id,
							smap<ID, player_t> & player_map);
	virtual		~PlayScene();

	bool		 HandleWindowEvent(const Event & event);
	bool		 HandleClientMsg(client_msg_t & msg);
	void		 FrameMove();

private:
	typedef smap<ID, player_t> player_map_t;

	enum
	{
		UI_FLAG_CHAT = 1
		, UI_FLAG_AIM = 2
	};

	enum
	{
		UI_SKILL_ARROW = 1
		, UI_SKILL_INF_ARROW = 2
	};

	struct context_t
	{
		int team;
		int index;	// player index
	};
	typedef smap<ID, context_t> context_map_t;

private:
	Cleaner					_cleaner;

	bool					_is_host;
	ID						_my_id;
	player_t *				_me;
	player_map_t			_player_map;

	Flag					_ui_flag;

	ChatBox					_chat_box;
	PopMsg					_pop;

	Tmx::Map				_map;

	Tmx::Object				_upside;
	Tmx::Object				_downside;

	context_map_t			_context_map;

	smap<ID, Champion*>		_champions;
	Champion*				_mychamp;
	list<Bullet*>			_bullets;

	int						_rest_update_count;
	bool					_command_send_ok;

	vector<Command>			_commands;

	// non-logical UI finites
	vector<IFinite*>		_ui_finites;

	SkillBox				_skillbox;

	Flag					_ui_skill_flag;
	wstring					_ui_skill_name;
	Keyboard::Key			_ui_skill_key;
	SkillArrow *			_ui_skill_arrow;

	// Event
	void	AddPlayer(const client_t & basic_info);
	void	HandleCommand(Command & c);

	bool	HandleKeyPressedEvent(const Event & e);
	bool	HandleTextEnteredEvent(const Event & e);
	bool	HandleMouseButtonPressedEvent(const Event & e);

	void	ResetSkillUI();

	// Frame Moves
	void	HandleInput();
	void	UpdateLogic();
	void	UpdateUI();
	void	Render();

	// Init Functions
	void	InitContext();
	void	CreatePlayerUnits();
	void	CreatePlayerUnits2(const vector<ID> ids, const Tmx::Object & loc);
	void	SendReadyToRecv();
};

extern PlayScene * g_playscene;