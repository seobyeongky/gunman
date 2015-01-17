#pragma once

#include "../scene.h"
#include "player.h"
#include "chat_box.h"
#include "pop_msg.h"
#include "../starcraft/star_unit.h"
#include "finite.h"
#include "../proto/input.pb.h"
#include "champion.h"
#include "renderable.h"
#include "bullet.h"
#include "skillbox.h"
#include "skillarrow.h"

#include <queue>

#include <v8.h>

// test tmx parsing
#include <tmxparser/Tmx.h>
#include "tilemap.h"


#define DEFINE_JS_REF(type,that) v8::Persistent<type> that##_ref


#define DEFINE_STATIC_JS_FUNC(func) static void S_##func(const v8::FunctionCallbackInfo<v8::Value>& args) \
	{ \
		_instance->func(args); \
	}

#define DEFINE_STATIC_JS_GETTER(func) static void S_##func(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info) \
	{ \
		_instance->func(name, info); \
	}


class PlayScene : public IScene
{
public:
				 PlayScene(	const wstring & room_name,
							bool is_host,
							ID my_id,
							smap<ID, player_t> & player_map,
							const wstring & map_name);
	virtual		~PlayScene();

	bool		 HandleWindowEvent(const Event & event);
	void		 FrameMove();

private:
	static PlayScene * _instance;

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
	};

	struct context_t
	{
		int team;
		int index;	// player index
	};
	typedef smap<ID, context_t> context_map_t;
	typedef smap<Event::EventType,function<bool(const Event & e)>> window_event_dict_t;

private:
	Cleaner					_cleaner;

	window_event_dict_t		_window_event_dict;

	bool					_is_host;
	ID						_my_id;
	player_t *				_me;
	player_map_t			_player_map;
	wstring					_map_name;
	bool					_send_ok;

	Flag					_ui_flag;

	RenderTexture			_backbuf;
	Sprite					_backsprite;
	ChatBox					_chat_box;
	PopMsg					_pop;

	Tmx::Map				_map;

	Tmx::Object				_upside;
	Tmx::Object				_downside;

	context_map_t			_context_map;

	smap<ID, Champion*>		_champions;
	Champion*				_mychamp;
	list<Bullet*>			_bullets;

	// frame_count means the next frame index when the update logic not committed
	int						_frame_count;

	// x is a input sync frame s.t. x % _input_sync_term == 0
	int						_input_sync_term;
	vector<Input>			_inputs;

	// non-logical UI finites
	vector<IFinite*>		_ui_finites;

	SkillBox				_skillbox;

	Flag					_ui_skill_flag;
	wstring					_ui_skill_name;
	Keyboard::Key			_ui_skill_key;
	SkillArrow *			_ui_skill_arrow;

	// scripting
	v8::Isolate * _js_isolate;
	DEFINE_JS_REF(v8::Context, _js_context);
	DEFINE_JS_REF(v8::Function, _js_player_input_callback);
	DEFINE_JS_REF(v8::Function, _js_private_input_callback);
	DEFINE_JS_REF(v8::Function, _js_frame_move_callback);
	DEFINE_JS_REF(v8::Object, _js_renderer);
	DEFINE_JS_REF(v8::Object, _js_player_api);
	DEFINE_JS_REF(v8::Object, _js_ui);

	smap<int, JSRenderable*> _js_renderables;

	// Event
	void	AddPlayer(const client_t & basic_info);
	void	HandleInputFromRemote(Input & c);

	bool	HandleKeyPressedEvent(const Event & e);
	bool	HandleKeyReleasedEvent(const Event & e);
	bool	HandleTextEnteredEvent(const Event & e);
	bool	HandleMouseButtonPressedEvent(const Event & e);
	bool	HandleMouseButtonReleasedEvent(const Event & e);
	bool	HandleIMEUpdatedEvent(const Event & e);

	void	ResetSkillUI();

	void	SendPendingInputs();

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

	// Scripting

	// Global
	void JS_Print(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_Print);
	void JS_OnPlayerInput(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_OnPlayerInput);
	void JS_OnPrivateInput(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_OnPrivateInput);
	void JS_OnFrameMove(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_OnFrameMove);
	void JS_RendererGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	DEFINE_STATIC_JS_GETTER(JS_RendererGetter);
	void JS_PlayerGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	DEFINE_STATIC_JS_GETTER(JS_PlayerGetter);

	// Player
	void JS_PlayerApiMe(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_PlayerApiMe);
	void JS_PlayerApiAll(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_PlayerApiAll);
	void JS_PlayerApiTheOthers(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_PlayerApiTheOthers);

	// UI
	void JS_UIGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	DEFINE_STATIC_JS_GETTER(JS_UIGetter);
	void JS_UIWidthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	DEFINE_STATIC_JS_GETTER(JS_UIWidthGetter);
	void JS_UIHeightGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	DEFINE_STATIC_JS_GETTER(JS_UIHeightGetter);
	void JS_UIDraw(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_UIDraw);

	// Renderer
	void JS_RendererMove(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_RendererMove);
	void JS_RendererAddRenderable(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_RendererAddRenderable);
	void JS_RendererRemoveRenderable(const v8::FunctionCallbackInfo<v8::Value>& args);
	DEFINE_STATIC_JS_FUNC(JS_RendererRemoveRenderable);

	void JS_Init();
	void JS_Finalize();
	void ReportException(v8::TryCatch* try_catch);
	bool CheckAllSended() const;
	void MoveGameFrame();
	Vector2d * GetClickedPos(const Event & event);
	player_t * SafeGetPlayer(ID pid);
};
