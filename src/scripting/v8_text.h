#pragma once

#include "v8_object_wrap.h"

#include <SFML\Graphics.hpp>
using namespace sf;

class JsText : public ObjectWrap
{
public:
	static void Init(v8::Isolate * isolate, v8::Handle<v8::ObjectTemplate> exports);
	static void Finalize();

private:
	JsText(v8::Isolate * isolate, v8::Local<v8::Object> handle);

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void JS_GetString(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_SetString(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void JS_GetColor(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_SetColor(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void JS_GetCharacterSize(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_SetCharacterSize(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void JS_GetWidth(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_GetHeight(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

	static v8::Persistent<v8::FunctionTemplate> constructor;

	Text _text;
};