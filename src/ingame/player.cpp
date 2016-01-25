#include <v8.h>

#include "player.h"
#include "../scripting/v8_utils.h"

using namespace v8;

// player_t
player_t::player_t() : name(), color(),
	input_received(false)
{
}
player_t::player_t(const wstring & _name, const Color & _color)
	: name(_name), color(_color),
	input_received(false)
{
}

Handle<Value> JS_MakePlayerObject(Isolate * isolate, ID id, const player_t & p)
{
	EscapableHandleScope handle_scope(isolate);
	Handle<Object> obj = Object::New(isolate);
#define SETT(key,value) obj->Set(v8::String::NewFromUtf8(isolate, key), value)
	SETT("id", Integer::NewFromUnsigned(isolate, id));

	vector<uint16_t> buf(p.name.length() + 1);
	for (unsigned int i = 0; i < p.name.length(); i++)
	{
		buf[i] = p.name[i];
	}
	buf[p.name.length()] = 0;

	SETT("name", v8::String::NewFromTwoByte(isolate, &buf[0]));

//	string uname;
//	uni2multi(p.name, &uname);
//	SETT("name", v8::String::NewFromUtf8(isolate, uname.c_str()));
	SETT("color", JS_MakeColorObject(isolate, p.color));
	return handle_scope.Escape<Object>(obj);
}



// JsPlayer

/*
Persistent<Function> JsPlayer::constructor;

JsPlayer::JsPlayer(ID id_, const player_t & p)
	: id(id_), name(p.name), color(p.color)
{
}

void JsPlayer::Init(Isolate * isolate, Handle<Object> exports)
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Player"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Local<ObjectTemplate> proto = tpl->PrototypeTemplate();
  proto->SetAccessor(v8::String::NewFromUtf8(isolate, "id"), JS_GetId);
  proto->SetAccessor(v8::String::NewFromUtf8(isolate, "name"), JS_GetName);
  proto->SetAccessor(v8::String::NewFromUtf8(isolate, "color"), JS_GetColor);
  constructor = Persistent<Function>(isolate, tpl->GetFunction());

  exports->Set(v8::String::NewFromUtf8(isolate, "Player"), PersistentToLocal(isolate, constructor));
}

void JsPlayer::JS_GetId(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	JsPlayer * self = GetSelf<JsPlayer>(info);
	info.GetReturnValue().Set(Integer::New(info.GetIsolate(), self->id));
}

void JsPlayer::JS_GetName(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	JsPlayer * self = GetSelf<JsPlayer>(info);

	string uname;
	uni2multi(self->name, &uname);
	info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), uname.c_str()));
}

void JsPlayer::JS_GetColor(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{

}*/