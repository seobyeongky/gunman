#pragma once

#include "../internal.h"
#include "../scripting/v8_object_wrap.h"

struct player_t
{
	wstring		name;
	Color		color;
	bool		input_received;

	player_t();
	player_t(const wstring & _name, const Color & _color);
};

v8::Handle<v8::Value> JS_MakePlayerObject(v8::Isolate * isolate, ID id, const player_t & p);


/*
class JsPlayer : public ObjectWrap
{
public:
	static void Init(v8::Isolate * isolate, v8::Handle<v8::Object> exports);

	JsPlayer(ID id, const player_t & p);

private:
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void JS_GetId(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_GetName(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_GetColor(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

	static v8::Persistent<v8::Function> constructor;

	ID id;
	wstring name;
	Color color;
};
*/