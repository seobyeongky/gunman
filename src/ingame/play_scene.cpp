#include "play_scene.h"
#include "../global.h"
#include "../net_interface.h"
#include "../sv/sv_service.h"
#include "../util.h"
#include "../logic.h"
#include "../userprofile.h"
#include "../director.h"
#include "../colors.h"
#include "../starcraft/grp_loader.h"
#include "../scripting/v8_utils.h"
#include "../scripting/v8_sfml.h"
#include "../scripting/v8_transformable.h"
#include "../scripting/v8_audio.h"
#include "ani00.h"
#include "ability.h"
#include "renderer.h"
#include "coord.h"
#include "../menu_scene.h"
#include "../sfkey_conv.h"

#include <v8.h>
#include <codecvt>

using namespace v8;


#define JS_STR(arg) v8::String::NewFromUtf8(_js_isolate, arg)
#define JS_FUNC(func) FunctionTemplate::New(_js_isolate, func)
#define JS_PARAM_ASSERTION(exp) if (!(exp)) \
	{ \
		args.GetIsolate()->ThrowException( \
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters " #exp)); \
		return; \
	} \


// Reads a file into a v8 string.
bool GetByteFromFile(const char* path, std::vector<char> * ptr) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) return false;

  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);

  ptr->clear();
  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (int i = 0; i < size;) {
    int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
    i += read;
  }
  fclose(file);
  ptr->assign(chars, chars + size);

  return true;
}



void CheckCursorScreenCollision(bool * up, bool * down, bool * left, bool * right)
{
	auto mousepos = Mouse::getPosition(G.window);
	auto winsize = G.window.getSize();
	if (mousepos.x < 0.07*winsize.x) *left = true;
	else *left = false;
	if (0.93*winsize.x < mousepos.x) *right = true;
	else *right = false;
	if (mousepos.y < 0.07*winsize.y) *up = true;
	else *up = false;
	if (0.93*winsize.y < mousepos.y) *down = true;
	else *down = false;
}


PlayScene * PlayScene::_instance = nullptr;

PlayScene::PlayScene(const wstring & room_name,
					 bool is_host, ID my_id, smap<ID, player_t> & player_map,
					 const wstring & map_name)
	: _cleaner()
	, _is_host(is_host)
	, _my_id(my_id)
	, _me(nullptr)
	, _player_map()
	, _map_name(map_name)
	, _send_ok(true)
	, _ui_flag()
	, _key_press_dict()
	, _backbuf()
	, _chat_box()
	, _pop()
	, _map()
	, _upside()
	, _downside()
	, _context_map()
	, _champions()
	, _mychamp(nullptr)
	, _bullets()
	, _frame_count(0)
	, _input_sync_term(10)
	, _ui_finites()
	, _skillbox()
	, _ui_skill_flag()
	, _ui_skill_name()
	, _ui_skill_key()
	, _ui_skill_arrow(nullptr)
{
	_instance = this;

	if (G.bg_music && G.bg_music->isPlaying()) G.bg_music->stop();

	using namespace placeholders;

	_window_event_dict.insert(Event::KeyPressed, bind(&PlayScene::HandleKeyPressedEvent, this, _1));
	_window_event_dict.insert(Event::KeyReleased, bind(&PlayScene::HandleKeyReleasedEvent, this, _1));
	_window_event_dict.insert(Event::TextEntered, bind(&PlayScene::HandleTextEnteredEvent, this, _1));
	_window_event_dict.insert(Event::MouseButtonPressed, bind(&PlayScene::HandleMouseButtonPressedEvent, this, _1));
	_window_event_dict.insert(Event::MouseButtonReleased, bind(&PlayScene::HandleMouseButtonReleasedEvent, this, _1));
	_window_event_dict.insert(Event::IMEUpdated, bind(&PlayScene::HandleIMEUpdatedEvent, this, _1));

	Renderer::Init();

//	_ui_skill_arrow = new SkillArrow();
/*
	{
		Ability * abil = new Ability();
		abil->name = L"missile";
		abil->flag = ACTIVE_ABILITY;
		abil->invoke = [this](Champion * champ, invoke_arg_t arg){
//			champ->RunCommandSkill(arg.key, [this](){
					
//			});
		};
		abil->cooltime = 5;
		abil->range = -1;
		abil->targeting = false;
		abil->casttime = 500;
		abil->icon = L"icon-missile";
		abil->uiflag.add(UI_SKILL_ARROW);
		ability_map[L"missile"] = abil;
	}
	*/
	// Fixme : player_map을 const로 인자 받게...
	// Fixme : smap 복사생성자만들어야함..복사대입연산자도
	for (auto it : player_map)
		_player_map[it.key()] = it.element();

	auto winsize = G.window.getSize();

	std::wstring buf;
	buf += L"사격의 달인 - ";
	buf += room_name;
	buf += L"방";
	G.window.setTitle(buf);

	_me = &_player_map[_my_id];

	_cleaner.Register(NetInterface::RegisterClientGoneCallback([this](const client_t & cl_info){
		_player_map.erase(cl_info.id);
	}));

	_cleaner.Register(NetInterface::RegisterPacketCallback(SV_TO_CL_CHAT, [this](Packet & packet)
	{
		ID cl_id;
		wstring msg;
		if (!(packet >> cl_id >> msg)) return;
		auto & player = _player_map[cl_id];
		_chat_box.AddChatMsg(player.color, player.name,
			Color(235, 235, 255), msg);
		G.sfx_mgr.Play(L"data\\system\\chatmsg.mp3");
	}));

	_cleaner.Register(NetInterface::RegisterPacketCallback(SV_TO_CL_BROADCAST, [this](Packet & packet){
		size_t nr_inputs;
		packet >> nr_inputs;
		static string inbuf;
		for (size_t i = 0U; i < nr_inputs; ++i)
		{
			Input input;
			packet >> inbuf;
			input.ParseFromString(inbuf);
			HandleInputFromRemote(input);
			inbuf.clear();
		}
		uint16_t nr_void_input_players;
		packet >> nr_void_input_players;
		for (uint16_t i = 0U; i < nr_void_input_players; ++i)
		{
			ID cid;
			packet >> cid;
			player_t * player = SafeGetPlayer(cid);
			if (!player) return;
			player->input_received = true;
		}
	}));

	_backbuf.create(G.window.getSize().x, G.window.getSize().y, false);
	_backsprite.setTexture(_backbuf.getTexture());
	_backsprite.setOrigin(0.f, static_cast<float>(_backbuf.getSize().y));
	_backsprite.setScale(1.f,-1.f);

	_chat_box.setPosition(.05f*winsize.x, .7f*winsize.y);
	_chat_box.Reset(*_me);
	_chat_box.the_text.on([this](const wstring & msg){
		if (msg.empty()) return;
		Packet send_packet;
		send_packet << TO_UINT16(CL_TO_SV_CHAT);
		send_packet << msg;
		SafeSend(send_packet);
		Input input;
		input.set_pid(_my_id);
		input.set_type(INPUT_CHAT_MESSAGE);
		wstring_convert<codecvt_utf8<wchar_t>> myconv;
		auto bytes = myconv.to_bytes(msg);
		input.set_msg(&bytes[0], bytes.size());
		_inputs.push_back(input);
	});

//	InitContext();
	
//	_map.ParseFile("./data/missile_dodge/tiled/departed.tmx");
/*	if (_map.HasError())
	{
		ErrorMsg(L"error code: %d\n", _map.GetErrorCode());
		ErrorMsg(L"error text: %hs\n", _map.GetErrorText().c_str());
		return;
	}
	
	for (auto & obj_group : _map.GetObjectGroups())
	{
		for (auto & tmxobj : obj_group->GetObjects())
		{
			if (tmxobj->GetName() == "upside")
			{
				_upside = *tmxobj;
			}
			else if (tmxobj->GetName() == "downside")
			{
				_downside = *tmxobj;
			}
		}
	}
	*/

	

//	{
//		DirChanger changed(L"data\\missile_dodge\\tiled");
//		g_tilemap.LoadFromTmx(_map);
//	}

//	grp::AddUnit(L"ghost", L"data\\missile_dodge\\grp\\ghost.grp", L"data\\missile_dodge\\grp\\ghost.info", _player_map.size());

//	CreatePlayerUnits();


	_skillbox.setPosition((float)winsize.x, .5f*winsize.y);
	_skillbox.setOrigin(89.f, .5f*191.f);

	JS_Init();

	SendReadyToRecv();
}

PlayScene::~PlayScene()
{
	Renderer::Uninit();
	JS_Finalize();

	for (auto it : _champions) delete it.element();
	for (auto it : _ui_finites) delete it;

	if (_is_host) EndSvService();

	delete _ui_skill_arrow;
//	ReleaseAbilities();

	if (G.bg_music) G.bg_music->play();
}

bool PlayScene::HandleWindowEvent(const Event & e)
{
	window_event_dict_t::Iter it;
	if (_window_event_dict.find(e.type, &it))
	{
		return (*it).element()(e);
	}

	return false;
}

bool PlayScene::HandleKeyPressedEvent(const Event & e)
{
	if (_ui_flag.has(UI_FLAG_CHAT))
	{
		if (e.key.code == Keyboard::Return)
		{
			_ui_flag.drop(UI_FLAG_CHAT);
			_chat_box.DrawEditText(false);
			_chat_box.HandleWindowEvent(e);
			return true;
		}
		return _chat_box.HandleWindowEvent(e);
	}
	else
	{
		/*
		static Keyboard::Key keys[] = {Keyboard::Q, Keyboard::W, Keyboard::E, Keyboard::R};
		for (auto key : keys)
		{
			if (e.key.code != key) continue;
			skillcontext * sc = _mychamp->skills[key];
			if (!sc) break;
			if (sc->cooltime.get() > 0) continue;
			if (_ui_flag.has(UI_FLAG_AIM) && _ui_skill_name == sc->ability->name) continue;
			_ui_flag.add(UI_FLAG_AIM);
			_ui_skill_flag = sc->ability->uiflag;
			_ui_skill_name = sc->ability->name;
			_ui_skill_key = key;
		}
		*/
#ifdef _DEBUG
		if (e.key.code == Keyboard::F5)
		{
			JS_Finalize();
			JS_Init();
		}
		else
#endif
		if (e.key.code == Keyboard::Return)
		{
			_ui_flag.add(UI_FLAG_CHAT);
			_chat_box.DrawEditText(true);
			return true;
		}
		else
		{
			Input input;
			input.set_pid(_my_id);
			input.set_type(INPUT_KEY_DOWN);
			input.set_key(e.key.code);
			
			_key_press_dict[input.key()] = true;
			_inputs.push_back(input);
			return true;
		}
	}
	return false;
}

bool PlayScene::HandleKeyReleasedEvent(const Event & e)
{
	smap<int, bool>::Iter it;
	if (_key_press_dict.find(e.key.code, &it) && (*it).element() == true)
	{
		Input input;
		input.set_pid(_my_id);
		input.set_type(INPUT_KEY_UP);
		input.set_key(e.key.code);
		(*it).element() = false;
		_inputs.push_back(input);
		return true;
	}

	return false;
}

bool PlayScene::HandleTextEnteredEvent(const Event & e)
{
	if (_ui_flag.has(UI_FLAG_CHAT))
	{
		if(e.key.code == UNICODE_ESC)
		{
			_chat_box.ClearEditText();
			return true;
		}
		return _chat_box.HandleWindowEvent(e);
	}
	else
	{
		if (e.key.code == UNICODE_ESC)
		{
			DisconnectToServer();
			Director::SwitchScene(new MenuScene());
			return true;
		}
		return false;
	}
}

bool PlayScene::HandleMouseButtonPressedEvent(const Event & e)
{
	Input input;
	input.set_pid(_my_id);

	if (e.mouseButton.button == Mouse::Left)
		input.set_type(INPUT_LEFT_MOUSE_DOWN);
	else if (e.mouseButton.button == Mouse::Right)
		input.set_type(INPUT_RIGHT_MOUSE_DOWN);
	else
		assert(!"Unknown mouse button!");
	
	input.set_allocated_pos(GetClickedPos(e));
	_inputs.push_back(input);

	return true;
}

bool PlayScene::HandleMouseButtonReleasedEvent(const Event & e)
{
	Input input;
	input.set_pid(_my_id);

	if (e.mouseButton.button == Mouse::Left)
		input.set_type(INPUT_LEFT_MOUSE_UP);
	else if (e.mouseButton.button == Mouse::Right)
		input.set_type(INPUT_RIGHT_MOUSE_UP);
	else
		assert(!"Unknown mouse button!");

	input.set_allocated_pos(GetClickedPos(e));
	_inputs.push_back(input);

	return true;
}	

bool PlayScene::HandleIMEUpdatedEvent(const Event & event)
{
	if (_ui_flag.has(UI_FLAG_CHAT))
		return _chat_box.HandleWindowEvent(event);
	else
		return false;
}

void PlayScene::ResetSkillUI()
{
	_ui_flag.drop(UI_FLAG_AIM);
	_ui_skill_arrow->SetVisible(false);
}

void PlayScene::SendPendingInputs()
{
	// inputs buffer가 비어있지 않다면 서버로 flush시킵니다.
	Packet sendpacket;
	sendpacket	<< TO_UINT16(CL_TO_SV_INPUTS)
		<< _inputs.size();
	if (_inputs.size() > 0)
	{
		static string outbuf;
		for (auto it : _inputs)
		{
			it.AppendToString(&outbuf);
			sendpacket << outbuf;
			outbuf.clear();
		}
		_inputs.clear();
	}
	opznet::SafeSend(sendpacket);
	_send_ok = false;
}

void PlayScene::FrameMove()
{
	if (_send_ok)
	{
		SendPendingInputs();
	}

	if (_frame_count % _input_sync_term == 0)
	{
		if (CheckAllSended())
		{
			// Continue
			MoveGameFrame();
			for (auto it : _player_map)
				it.element().input_received = false;
			SendPendingInputs();
		}
		else
		{
			// Wait for pending input
		}
	}
	else
	{
		MoveGameFrame();
	}

	UpdateUI();

	Render();
}

void PlayScene::AddPlayer(const client_t & basic_info)
{
	ID id = basic_info.id;
	_player_map.insert(id, player_t(basic_info.name, colors::GetPlayerColor(id)));
}

// 서버로부터 온 클라이언트 input이니, 순서는 보장이 되어있음.
void PlayScene::HandleInputFromRemote(Input & input)
{
	ID clid = input.pid();
	player_t * player = SafeGetPlayer(clid);
	if (!player) return;
	player->input_received = true;
	
	if (!_js_player_input_callback_ref.IsEmpty())
	{
		Isolate::Scope isolate_scope(_js_isolate);
		HandleScope handle_scope(_js_isolate);
		v8::TryCatch try_catch;
		try_catch.SetVerbose(true);
		Local<v8::Context> context = PersistentToLocal(_js_isolate, _js_context_ref);
		v8::Context::Scope context_scope(context );
		Local<Function> cb = PersistentToLocal(_js_isolate, _js_player_input_callback_ref);
		Handle<Number> pid = Number::New(_js_isolate, static_cast<double>(clid));
		Handle<Number> type = Number::New(_js_isolate, static_cast<double>(input.type()));

		/*
			INPUT_LEFT_MOUSE_DOWN = 0;
			INPUT_LEFT_MOUSE_UP = 1;
			INPUT_RIGHT_MOUSE_DOWN = 2;
			INPUT_RIGHT_MOUSE_UP = 3;
			INPUT_MOUSE_MOVE = 4;
			INPUT_KEY_DOWN = 5;
			INPUT_KEY_UP = 6;
			INPUT_CHAT_MESSAGE = 7;
		*/
		switch (input.type())
		{
		case INPUT_LEFT_MOUSE_DOWN:
		case INPUT_LEFT_MOUSE_UP:
		case INPUT_RIGHT_MOUSE_DOWN:
		case INPUT_RIGHT_MOUSE_UP:
		case INPUT_MOUSE_MOVE:
			{
				Handle<Object> pos = Object::New(_js_isolate);
				pos->Set(JS_STR("x"), Number::New(_js_isolate, static_cast<int>(input.pos().x())));
				pos->Set(JS_STR("y"), Number::New(_js_isolate, static_cast<int>(input.pos().y())));
				Handle<Value> args[] = {pid, type, pos};
				Handle<Value> result = cb->Call(context->Global(), 3, args);
			}
			break;
		case INPUT_KEY_DOWN:
		case INPUT_KEY_UP:
			{
//				char key = input.key();
//				char buf[] = {key, '\0'};
				Handle<Value> args[] = {pid, type, JS_STR(get_sfconv(static_cast<Keyboard::Key>(input.key())))};
				Handle<Value> result = cb->Call(context->Global(), 3, args);
			}
			break;
		case INPUT_CHAT_MESSAGE:
			{
				wstring_convert<codecvt_utf8<wchar_t>> myconv;
				auto msg = myconv.from_bytes(input.msg());
				vector<uint16_t> haha(msg.begin(), msg.end());
				if (haha.empty()) break;
				Handle<Value> args[] = {pid, type, v8::String::NewFromTwoByte(_js_isolate, &haha[0], v8::String::kNormalString, haha.size())};
				Handle<Value> result = cb->Call(context->Global(), 3, args);
			}
			break;
		}


		if (try_catch.HasCaught())
		{
			ReportException(&try_catch);
		}
	}

	/*
	switch (input.type())
	{
	case COMMAND_GO:
		{
			smap<ID, Champion*>::Iter champ_it;
			if (!_champions.find(clid, &champ_it))
			{
				G.logger->Warning(L"HandleCommand : COMMAND_GO : ¾ø´Â Ã¨ÇÇ¿Â(%d)", clid);
				return;
			}
			Champion * champion = (*champ_it).element();
			champion->RunCommandGo(Vector2i(c.pos().x(),c.pos().y()));
		}
		break;

	case COMMAND_SKILL:
		{
			smap<ID, Champion*>::Iter champ_it;
			if (!_champions.find(clid, &champ_it))
			{
				G.logger->Warning(L"HandleCommand : COMMAND_GO : ¾ø´Â Ã¨ÇÇ¿Â(%d)", clid);
				return;
			}
			Champion * champ = (*champ_it).element();
			Vector2i pos(c.pos().x(), c.pos().y());
			auto key = (Keyboard::Key)c.key();
			skillcontext * sc = champ->skills[key];
			if (!sc) return;
			if (sc->cooltime.get() > 0) return;
//			champ->RunCommandSkill(key, [](){});
			invoke_arg_t arg;
			arg.key = (Keyboard::Key)c.key();
			arg.pos = pos;
			sc->ability->invoke(champ, arg);
		}
	}
	*/
}

void PlayScene::HandleInput()
{
	if (GetFocus() != G.window.getSystemHandle()) return;
	

/*	{
		bool up, down, left, right;
		CheckCursorScreenCollision(&up, &down, &left, &right);

		// Handle View
		if (left) Renderer::Move(-15.f,0);
		if (right) Renderer::Move(15.f,0);
		if (up) Renderer::Move(0,-15.f);
		if (down) Renderer::Move(0,15.f);
	}*/
}

void PlayScene::UpdateLogic()
{
	for (auto & it : _champions)
	{
		it.element()->Update();
	}

	for (list<Bullet*>::iterator it = _bullets.begin(); it != _bullets.end();)
	{
		Bullet * bullet = *it;
		bool remove = false;

		bullet->pos += bullet->vel;

		for (auto & champit : _champions)
		{
			Champion * champ = champit.element();
			auto & champpos = champ->GetPos();
			if (abs(champpos.x-bullet->pos.x) <= champ->GetSize()/2
				&& abs(champpos.y-bullet->pos.y) <= champ->GetSize()/2)
			{
				bullet->callback();
				if (!(bullet->flags & BULLET_PASS_THROUGHT))
				{
					remove = true;
					break;
				}
			}
		}

		if (remove)
		{
			delete bullet;
			it = _bullets.erase(it);
		}
		else
			it++;
	}
}

void PlayScene::UpdateUI()
{
	_chat_box.Update();
//	_skillbox.Update();

	bool up, down, left, right;
	CheckCursorScreenCollision(&up, &down, &left, &right);

	// Handle Cursor
	if (left || right || down || up)
	{
		Sprite cursor;
		if (left && up) cursor = *G.sprite_map[L"cursor_left_up"];
		else if (right && up) cursor = *G.sprite_map[L"cursor_right_up"];
		else if (right && down) cursor = *G.sprite_map[L"cursor_right_down"];
		else if (left && down) cursor = *G.sprite_map[L"cursor_left_down"];
		else if (up) cursor = *G.sprite_map[L"cursor_up"];
		else if (right) cursor = *G.sprite_map[L"cursor_right"];
		else if (down) cursor = *G.sprite_map[L"cursor_down"];
		else if (left) cursor = *G.sprite_map[L"cursor_left"];
		cursor.setOrigin(22.f,22.f);
		Director::SetCursor(cursor);
	}
	else
	{
		Director::SetDefaultCursor();
	}

	/*
	for (vector<IFinite*>::iterator it = _ui_finites.begin(); it != _ui_finites.end();)
	{
		(*it)->Consume(1);
		if ((*it)->IsDead())
		{
			delete *it;
			it = _ui_finites.erase(it);
		}
		else
			it++;
	}

	if (_ui_flag.has(UI_FLAG_AIM))
	{
		if (_ui_skill_flag.has(UI_SKILL_ARROW))
		{
			_ui_skill_arrow->SetVisible(true);
			Vector2f champpos((float)_mychamp->GetPos().x, (float)_mychamp->GetPos().y);
			_ui_skill_arrow->setPosition(champpos);
			_ui_skill_arrow->Update(ability_map[_ui_skill_name]->range);
			Vector2f cursorpos((float)Mouse::getPosition(G.window).x, (float)Mouse::getPosition(G.window).y);
			cursorpos = Renderer::TransformPoint(cursorpos);
			_ui_skill_arrow->setRotation(GetDir(cursorpos-champpos));
		}
	}
	*/
}

void PlayScene::Render()
{
	Renderer::Render();
	G.window.draw(_backsprite);
	G.window.draw(_chat_box);
//	G.window.draw(_skillbox);
	G.window.draw(_pop);
}

void PlayScene::InitContext()
{
	unsigned int i = 0;
	int team = 0;
	for (auto & player : _player_map)
	{
		if (i >= _player_map.size()/2) team = 1;
		auto & context = _context_map[player.key()];
		context.team = team;
		context.index = i;
		i++;
	}
}

void PlayScene::CreatePlayerUnits()
{
	vector<ID> upsides;
	vector<ID> downsides;

	for (auto & it : _player_map)
	{
		auto & context = _context_map[it.key()];
		if (context.team == 0)
		{
			upsides.push_back(it.key());
		}
		else if (context.team == 1)
		{
			downsides.push_back(it.key());
		}
		else
		{
			G.logger->Warning(L"Invalid context(%d) : team is %d", it.key(), context.team);
		}
	}

	CreatePlayerUnits2(upsides, _upside);
	CreatePlayerUnits2(downsides, _downside);
}

void PlayScene::CreatePlayerUnits2(const vector<ID> ids, const Tmx::Object & loc)
{
	for (ID id : ids)
	{
		auto & context = _context_map[id];
		auto * new_champ = new Champion(context.index);
		new_champ->SetPos(Vector2i(loc.GetX()+loc.GetWidth()/2,loc.GetY()+loc.GetHeight()/2));
		new_champ->skills[Keyboard::Q] = new skillcontext(ability_map[L"missile"]);
		_champions[id] = new_champ;
		if (id == _my_id)
		{
			_mychamp = new_champ;
			_skillbox.Reset(_mychamp, L"missile", L"", L"");
		}
	}
}

void PlayScene::SendReadyToRecv()
{
	Packet sendpacket;
	sendpacket << TO_UINT16(CL_TO_SV_READY_TO_RECV);
	SafeSend(sendpacket);
}


void PlayScene::JS_Print(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
	    HandleScope handle_scope(args.GetIsolate());
	    if (first) {
		  first = false;
		} else {
		  printf(" ");
		}
		v8::String::Value msg(args[i]);
		const wchar_t * wchar_list = reinterpret_cast<wchar_t*>(*msg);

		_instance->_chat_box.AddInfoMsg(wchar_list);
		G.sfx_mgr.Play(L"data\\system\\bell.wav");
	}
}

void PlayScene::JS_OnPlayerInput(const FunctionCallbackInfo<Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	JS_PARAM_ASSERTION(args.Length() == 1);

	_js_player_input_callback_ref.Reset(_js_isolate, Handle<Function>::Cast(args[0]));
}

void PlayScene::JS_OnPrivateInput(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	JS_PARAM_ASSERTION(args.Length() == 1);

	_js_private_input_callback_ref.Reset(_js_isolate, Handle<Function>::Cast(args[0]));
}

void PlayScene::JS_OnFrameMove(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	JS_PARAM_ASSERTION(args.Length() == 1);

	_js_frame_move_callback_ref.Reset(_js_isolate, Handle<Function>::Cast(args[0]));
}

void PlayScene::JS_RendererGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	HandleScope handle_scope(_js_isolate);

	info.GetReturnValue().Set(PersistentToLocal(_js_isolate, _js_renderer_ref));
}

void PlayScene::JS_PlayerGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	HandleScope handle_scope(_js_isolate);

	info.GetReturnValue().Set(PersistentToLocal(_js_isolate, _js_player_api_ref));
}

void PlayScene::JS_PlayerApiMe(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	assert(_me && "me should exists");
	args.GetReturnValue().Set(JS_MakePlayerObject(_js_isolate, _my_id, *_me));
}

void PlayScene::JS_PlayerApiAll(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	Handle<Array> ret = Array::New(_js_isolate, _player_map.size());
	int32_t index = 0;
	for (auto it : _player_map)
	{
		ret->Set(index, JS_MakePlayerObject(_js_isolate, it.key(), it.element()));
		index++;
	}
	args.GetReturnValue().Set(ret);
}

void PlayScene::JS_PlayerApiTheOthers(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	Handle<Array> ret = Array::New(_js_isolate, _player_map.size() - 1);
	int32_t index = 0;
	for (auto it : _player_map)
	{
		if (it.key() == _my_id) continue;
		ret->Set(index, JS_MakePlayerObject(_js_isolate, it.key(), it.element()));
		index++;
	}
	args.GetReturnValue().Set(ret);
}

void PlayScene::JS_UIGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	HandleScope handle_scope(_js_isolate);

	info.GetReturnValue().Set(PersistentToLocal(_js_isolate, _js_ui_ref));
}

void PlayScene::JS_UIWidthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	HandleScope handle_scope(_js_isolate);
	
	info.GetReturnValue().Set(Number::New(_js_isolate, G.window.getSize().x));
}

void PlayScene::JS_UIHeightGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	HandleScope handle_scope(_js_isolate);
	
	info.GetReturnValue().Set(Number::New(_js_isolate, G.window.getSize().y));
}

void PlayScene::JS_UIDraw(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	JS_PARAM_ASSERTION(args.Length() == 1);

	HandleScope handle_scope(_js_isolate);

	Local<Object> the = Local<Object>::Cast(args[0]);
	JS_PARAM_ASSERTION(the->InternalFieldCount() > 0);

	Local<External> wrapped = Local<External>::Cast(the->GetInternalField(0));
	void* ptr = wrapped->Value();
	Drawable * drawable = static_cast<CommonDrwableStyle *>(ptr);

	_backbuf.draw(*drawable);
//	G.window.draw(*drawable);
}

void PlayScene::JS_RendererMove(const FunctionCallbackInfo<Value>& args)
{
	JS_PARAM_ASSERTION(args.Length() == 2);
	
	HandleScope handle_scope(args.GetIsolate());

	auto dx = Handle<Number>::Cast(args[0]);
	auto dy = Handle<Number>::Cast(args[1]);

	Renderer::Move(static_cast<float>(dx->NumberValue()), static_cast<float>(dy->NumberValue()));
}

// a = new Sprite
// id = Renderer.addRenderable(a)
// Renderer.removeRenderable(id)

void PlayScene::JS_RendererAddRenderable(const FunctionCallbackInfo<Value>& args)
{
	JS_PARAM_ASSERTION(args.Length() == 1);

	HandleScope handle_scope(args.GetIsolate());

	auto obj = Handle<Object>::Cast(args[0]);

	auto js_renderable = new JSRenderable(args.GetIsolate(), obj);

	int id = Renderer::AddRenderable(js_renderable);
	_js_renderables[id] = js_renderable;

	args.GetReturnValue().Set(id);
}

void PlayScene::JS_RendererRemoveRenderable(const FunctionCallbackInfo<Value>& args)
{
	JS_PARAM_ASSERTION(args.Length() == 1);

	HandleScope handle_scope(args.GetIsolate());

	auto num = Handle<Number>::Cast(args[0]);

	Renderer::RemoveRenderable(num->Int32Value());
}


void PlayScene::JS_Init()
{
	_js_isolate = v8::Isolate::New();
	{
		Isolate::Scope isolate_scope(_js_isolate);
		
		HandleScope handle_scope(_js_isolate);
		
		Local<ObjectTemplate> js_global;
		js_global = ObjectTemplate::New(_js_isolate);

		js_global->Set(JS_STR("print"), JS_FUNC(S_JS_Print));
		js_global->Set(JS_STR("onPlayerInput"), JS_FUNC(S_JS_OnPlayerInput));
		js_global->Set(JS_STR("onPrivateInput"), JS_FUNC(S_JS_OnPrivateInput));
		js_global->Set(JS_STR("onFrameMove"), JS_FUNC(S_JS_OnFrameMove));
		js_global->SetAccessor(JS_STR("Renderer"), S_JS_RendererGetter);
		js_global->SetAccessor(JS_STR("Player"), S_JS_PlayerGetter);
		js_global->SetAccessor(JS_STR("UI"), S_JS_UIGetter);

		v8_transformable::Init(_js_isolate);
		JsText::Init(_js_isolate, js_global);
		JsTexture::Init(_js_isolate, js_global, _map_name);
		JsSprite::Init(_js_isolate, js_global);
		js_audio::Init(_js_isolate, js_global, _map_name);
/*
		Local<ObjectTemplate> player_templ = ObjectTemplate::New(_js_isolate);
		player_templ->SetInternalFieldCount(1);
		player_templ->SetAccessor(JS_STR("id"), js_player_t::JS_GetId);
		player_templ->SetAccessor(JS_STR("name"), js_player_t::JS_GetName);
		player_templ->SetAccessor(JS_STR("color"), js_player_t::JS_GetColor);
		_js_player_templ_ref.Reset(_js_isolate, player_templ);
		*/
		Local<v8::Context> context = v8::Context::New(_js_isolate, nullptr, js_global);
		_js_context_ref.Reset(_js_isolate, context);

		v8::Context::Scope context_scope(context);

		Local<ObjectTemplate> renderer_templ = ObjectTemplate::New(_js_isolate);
		renderer_templ->Set(JS_STR("move"), JS_FUNC(S_JS_RendererMove));
		renderer_templ->Set(JS_STR("addRenderable"), JS_FUNC(S_JS_RendererAddRenderable));
		renderer_templ->Set(JS_STR("removeRenderable"), JS_FUNC(S_JS_RendererRemoveRenderable));
		Local<Object> renderer = renderer_templ->NewInstance();
		_js_renderer_ref.Reset(_js_isolate, renderer);

		Local<ObjectTemplate> player_api_templ = ObjectTemplate::New(_js_isolate);
		player_api_templ->Set(JS_STR("me"), JS_FUNC(S_JS_PlayerApiMe));
		player_api_templ->Set(JS_STR("all"), JS_FUNC(S_JS_PlayerApiAll));
		player_api_templ->Set(JS_STR("theOthers"), JS_FUNC(S_JS_PlayerApiTheOthers));
		Local<Object> player_api = player_api_templ->NewInstance();
		_js_player_api_ref.Reset(_js_isolate, player_api);

		Local<ObjectTemplate> ui_templ = ObjectTemplate::New(_js_isolate);
		ui_templ->SetAccessor(JS_STR("width"), S_JS_UIWidthGetter);
		ui_templ->SetAccessor(JS_STR("height"), S_JS_UIHeightGetter);
		ui_templ->Set(JS_STR("draw"), JS_FUNC(S_JS_UIDraw));
		Local<Object> ui = ui_templ->NewInstance();
		_js_ui_ref.Reset(_js_isolate, ui);

		js_audio::Instantiate(_js_isolate);

		v8::TryCatch try_catch;
		v8::Handle<v8::Value> script_path;
		v8::Handle<v8::String> source;
		{
			std::string path;
			std::wstring wpath(L"data/");
			wpath += _map_name;
			wpath += L"/script/index.js";
			uni2multi(wpath, &path);
			script_path = JS_STR(path.c_str());
			std::vector<char> buf;
			if (!GetByteFromFile(path.c_str(), &buf))
			{
				ErrorMsg(L"%s 파일을 읽을 수 없습니다.", wpath.c_str());
				return;
			}
			source = v8::String::NewFromUtf8(_js_isolate, &buf[0], v8::String::kNormalString, buf.size());
		}
		v8::ScriptOrigin origin(script_path);
		v8::Handle<v8::Script> script = v8::Script::Compile(source, &origin);
		if (script.IsEmpty()) {
			// Print errors that happened during compilation.
			ReportException(&try_catch);
		} else {
			v8::Handle<v8::Value> result = script->Run();

			if (result.IsEmpty()) {
				assert(try_catch.HasCaught());
				// Print errors that happened during execution.
				ReportException(&try_catch);
			}
		}
	}
}

void PlayScene::JS_Finalize()
{
	_js_ui_ref.Reset();
	_js_player_api_ref.Reset();
	_js_renderer_ref.Reset();
	_js_frame_move_callback_ref.Reset();
	_js_private_input_callback_ref.Reset();
	_js_player_input_callback_ref.Reset();
	_js_context_ref.Reset();
	js_audio::Finalize();
	JsSprite::Finalize();
	JsTexture::Finalize();
	JsText::Finalize();
	v8_transformable::Finalize();
	_js_isolate->Dispose();
}

void PlayScene::ReportException(TryCatch* try_catch)
{
	v8::HandleScope handle_scope(_js_isolate);
	v8::Handle<v8::Message> message = try_catch->Message();
	v8::String::Value exception(try_catch->Exception());
	const wchar_t* exception_string = reinterpret_cast<const wchar_t *>(*exception);
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		_chat_box.AddInfoMsg(exception_string, Color(255, 255, 0, 255));
	} else {
		// Print (filename):(line number): (message).
		
		v8::String::Value filename(message->GetScriptResourceName());
		const wchar_t* filename_string  = reinterpret_cast<const wchar_t *>(*filename);
		int linenum = message->GetLineNumber();
		wchar_t buf[1024];
		wsprintf(buf, L"%s:%i: %s\n", filename_string, linenum, exception_string);
		// Print line of source code.
		v8::String::Utf8Value sourceline(message->GetSourceLine());
		const wchar_t * sourceline_string = reinterpret_cast<const wchar_t *>(*sourceline);
		wsprintf(buf, L"%s\n", sourceline_string);
		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++) {
			wsprintf(buf, L" ");
		}
		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) {
			wsprintf(buf, L"^");
		}
		wsprintf(buf, L"\n");
		v8::String::Value stack_trace(try_catch->StackTrace());
		if (stack_trace.length() > 0) {
			const wchar_t * stack_trace_string = reinterpret_cast<const wchar_t *>(*stack_trace);
			wsprintf(buf, L"%s\n", stack_trace_string);
		}
		_chat_box.AddInfoMsg(buf, Color(255, 255, 0, 255));
		G.sfx_mgr.Play(L"data\\system\\audio\\Error.wav");
	}
}

bool PlayScene::CheckAllSended() const
{
	bool all_sended = true;
	for (player_map_t::ConstIter it = _player_map.cbegin(); it != _player_map.cend(); ++it)
	{
		if (!(*it).element().input_received)
			return false;
	}

	return true;
}

void PlayScene::MoveGameFrame()
{
	if (!_js_frame_move_callback_ref.IsEmpty())
	{
		_backbuf.clear();

		Isolate::Scope isolate_scope(_js_isolate);
		HandleScope handle_scope(_js_isolate);
		v8::TryCatch try_catch;
		Local<v8::Context> context = PersistentToLocal(_js_isolate, _js_context_ref);
		v8::Context::Scope context_scope(context);
		Local<Function> cb = PersistentToLocal(_js_isolate, _js_frame_move_callback_ref);
		v8::Handle<v8::Value> result = cb->Call(context->Global(), 0, nullptr);

		if (try_catch.HasCaught())
		{
			ReportException(&try_catch);
		}
	}

	_frame_count++;
}

Vector2d * PlayScene::GetClickedPos(const Event & event)
{
	Vector2f p((float)event.mouseButton.x, (float)event.mouseButton.y);
	p = Renderer::TransformPoint(p);
	Vector2d *f = new Vector2d;
	f->set_x((int)p.x);
	f->set_y((int)p.y);
	return f;
}

player_t * PlayScene::SafeGetPlayer(ID pid)
{
	player_map_t::Iter it;
	if (!_player_map.find(pid, &it))
	{
		G.logger->Warning(L"HandleInputFromRemote : 없는 클라이언트 %d", pid);
		return nullptr;
	}

	return &(*it).element();
}