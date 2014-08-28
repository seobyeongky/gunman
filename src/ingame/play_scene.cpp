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
#include "ani00.h"
#include "ability.h"
#include "renderer.h"
#include "coord.h"

#include <v8.h>
using namespace v8;



PlayScene * g_playscene = nullptr;


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

PlayScene::PlayScene(const wstring & room_name,
					 bool is_host, ID my_id, smap<ID, player_t> & player_map)
	: _cleaner()
	, _is_host(is_host)
	, _my_id(my_id)
	, _me(nullptr)
	, _player_map()
	, _ui_flag()
	, _chat_box()
	, _pop()
	, _map()
	, _upside()
	, _downside()
	, _context_map()
	, _champions()
	, _mychamp(nullptr)
	, _bullets()
	, _rest_update_count(0)
	, _command_send_ok(true)
	, _ui_finites()
	, _skillbox()
	, _ui_skill_flag()
	, _ui_skill_name()
	, _ui_skill_key()
	, _ui_skill_arrow(nullptr)
{
	g_playscene = this;

	Renderer::Init();

	_ui_skill_arrow = new SkillArrow();

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
	
	// Fixme : player_map을 const로 인자 받게...
	// Fixme : smap 복사생성자만들어야함..복사대입연산자도
	for (auto it : player_map)
		_player_map[it.key()] = it.element();

	auto winsize = G.window.getSize();

	G.window.setTitle(L"사격의 달인 - " + room_name + L"방");
	
	_me = &_player_map[_my_id];

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

	_cleaner.Register(NetInterface::RegisterPacketCallback(SV_TO_CL_COMMANDS, [this](Packet & packet){
		size_t nr_commands;
		packet >> nr_commands;
		string inbuf;
		for (size_t i = 0U; i < nr_commands; ++i)
		{
			Command c;
			packet >> inbuf;
			c.ParseFromString(inbuf);
			HandleCommand(c);
			inbuf.clear();
		}
		UpdateLogic();
		_rest_update_count = 3;
		_command_send_ok = true;
	}));

	_chat_box.setPosition(.05f*winsize.x, .7f*winsize.y);
	_chat_box.Reset(*_me);

	InitContext();
	
	_map.ParseFile("./data/missile_dodge/tiled/departed.tmx");
	if (_map.HasError())
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

	{
		DirChanger changed(L"data\\missile_dodge\\tiled");
		g_tilemap.LoadFromTmx(_map);
	}

	grp::AddUnit(L"ghost", L"data\\missile_dodge\\grp\\ghost.grp", L"data\\missile_dodge\\grp\\ghost.info", _player_map.size());

	CreatePlayerUnits();

	SendReadyToRecv();

	_skillbox.setPosition((float)winsize.x, .5f*winsize.y);
	_skillbox.setOrigin(89.f, .5f*191.f);

	V8::Initialize();
	_js_isolate = v8::Isolate::New();
	{
		Isolate::Scope isolate_scope(_js_isolate);
		CreateJSContext();
		v8::Context::Scope context_scope(_js_context);
		v8::HandleScope handle_scope(_js_isolate);
		v8::TryCatch try_catch;
		v8::Handle<v8::Value> name;
		v8::ScriptOrigin origin(name);
		v8::Handle<v8::String> source;
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

PlayScene::~PlayScene()
{
	V8::Dispose();

	for (auto it : _champions) delete it.element();
	for (auto it : _ui_finites) delete it;

	if (_is_host) EndSvService();

	delete _ui_skill_arrow;

	Renderer::Uninit();
//	ReleaseAbilities();
}

bool PlayScene::HandleWindowEvent(const Event & e)
{
	if (e.type == Event::KeyPressed)
		return HandleKeyPressedEvent(e);
	else if (e.type == Event::TextEntered)
		return HandleTextEnteredEvent(e);
	else if (e.type == Event::MouseButtonPressed)
		return HandleMouseButtonPressedEvent(e);
	else
		return false;
}

bool PlayScene::HandleKeyPressedEvent(const Event & e)
{
	if (_ui_flag.has(UI_FLAG_CHAT))
	{
		if (e.key.code == Keyboard::Return)
		{
			_ui_flag.drop(UI_FLAG_CHAT);
			_chat_box.HandleWindowEvent(e);
			return true;
		}
		return _chat_box.HandleWindowEvent(e);
	}
	else
	{
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
		
		if (e.key.code == Keyboard::Return)
		{
			_ui_flag.add(UI_FLAG_CHAT);
			return true;
		}
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
			G.window.close();
			return true;
		}
		return false;
	}
}

bool PlayScene::HandleMouseButtonPressedEvent(const Event & e)
{
	if (e.mouseButton.button == Mouse::Right)
	{
		if (_ui_flag.has(UI_FLAG_AIM))
		{
			ResetSkillUI();
		}

		Vector2f p((float)e.mouseButton.x, (float)e.mouseButton.y);
		p = Renderer::TransformPoint(p);

		Ani00 * newone = new Ani00();
		newone->setPosition(p.x, p.y);
		_ui_finites.push_back(newone);

		Command c;
		c.set_pid(_my_id);
		c.set_type(COMMAND_GO);
		Vector2d *f = new Vector2d;
		f->set_x((int)p.x);
		f->set_y((int)p.y);
		c.set_allocated_pos(f);
		_commands.push_back(c);

		return true;
	}

	return false;
}

void PlayScene::ResetSkillUI()
{
	_ui_flag.drop(UI_FLAG_AIM);
	_ui_skill_arrow->SetVisible(false);
}

bool PlayScene::HandleClientMsg(client_msg_t & msg)
{
	return false;
}

void PlayScene::FrameMove()
{
	HandleInput();

	if (_rest_update_count > 0)
	{
		_rest_update_count--;
		UpdateLogic();
	}

	if (_command_send_ok && _rest_update_count == 0)
	{
		_command_send_ok = false;
		Packet sendpacket;
		sendpacket	<< TO_UINT16(CL_TO_SV_COMMANDS)
			<< _commands.size();
		string outbuf;
		for (auto it : _commands)
		{
			it.AppendToString(&outbuf);
			sendpacket << outbuf;
			outbuf.clear();
		}
		opznet::SafeSend(sendpacket);
		_commands.clear();
	}

	UpdateUI();

	Render();
}

void PlayScene::AddPlayer(const client_t & basic_info)
{
	ID id = basic_info.id;
	_player_map.insert(id, player_t(basic_info.name, colors::GetPlayerColor(id)));
}

void PlayScene::HandleCommand(Command & c)
{
	ID clid = c.pid();
	player_map_t::Iter it;
	if (!_player_map.find(clid, &it))
	{
		G.logger->Warning(L"HandleCommand : 없는 클라이언트 %d", clid);
		return;
	}
	player_t & player = (*it).element();

	switch (c.type())
	{
	case COMMAND_GO:
		{
			smap<ID, Champion*>::Iter champ_it;
			if (!_champions.find(clid, &champ_it))
			{
				G.logger->Warning(L"HandleCommand : COMMAND_GO : 없는 챔피온(%d)", clid);
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
				G.logger->Warning(L"HandleCommand : COMMAND_GO : 없는 챔피온(%d)", clid);
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
}

void PlayScene::HandleInput()
{
	if (GetFocus() != G.window.getSystemHandle()) return;
	

	{
		bool up, down, left, right;
		CheckCursorScreenCollision(&up, &down, &left, &right);

		// Handle View
		if (left) Renderer::Move(-15.f,0);
		if (right) Renderer::Move(15.f,0);
		if (up) Renderer::Move(0,-15.f);
		if (down) Renderer::Move(0,15.f);
	}
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
	_skillbox.Update();

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
		if (_ui_flag.has(UI_FLAG_AIM))
			Director::SetCursor(Sprite());
		else
			Director::SetDefaultCursor();
	}

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
}

void PlayScene::Render()
{
	Renderer::Render();
	G.window.draw(_chat_box);
	G.window.draw(_skillbox);
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

		g_playscene->_chat_box.AddInfoMsg(wchar_list);
		G.sfx_mgr.Play(L"data\\system\\bell.wav");
	}
}

void PlayScene::JS_OnPlayerInput(const FunctionCallbackInfo<Value>& args)
{
	HandleScope handle_scope(_js_isolate);

	_js_player_input_callback = Handle<Function>::Cast(args[0]);
	_js_player_input_callback_ref.Reset(_js_isolate, _js_player_input_callback);
}


void PlayScene::CreateJSContext()
{
	HandleScope handle_scope(_js_isolate);

	_js_global = ObjectTemplate::New(_js_isolate);

	_js_global->Set(v8::String::NewFromUtf8(_js_isolate, "print"), FunctionTemplate::New(_js_isolate, S_JS_Print));

	_js_global->Set(v8::String::NewFromUtf8(_js_isolate, "_OnPlayerEvent"), FunctionTemplate::New(_js_isolate, S_JS_OnPlayerInput));

	_js_context = v8::Context::New(_js_isolate, nullptr, _js_global);

//	_js_context_ref.Reset(_js_isolate, _js_context);
}

void PlayScene::ReportException(TryCatch* try_catch)
{
	v8::HandleScope handle_scope(_js_isolate);
	v8::String::Value exception(try_catch->Exception());
	const wchar_t* exception_string = reinterpret_cast<const wchar_t *>(*exception);
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		_chat_box.AddInfoMsg(exception_string);
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
		_chat_box.AddInfoMsg(buf);
		G.sfx_mgr.Play(L"data\\system\\Error.wav");
	}
}