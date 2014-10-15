#pragma once

#include "v8_object_wrap.h"

#include <SFML/Graphics.hpp>
using namespace sf;

class JsTexture : public ObjectWrap
{
public:
	static void Init(v8::Isolate * isolate, v8::Handle<v8::ObjectTemplate> exports, const std::wstring & map_name);
	static void Finalize();

private:
	JsTexture(v8::Isolate * isolate, v8::Local<v8::Object> handle);

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void JS_LoadFromFile(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void JS_GetWidth(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_GetHeight(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

	static v8::Persistent<v8::FunctionTemplate> constructor;

	Texture _texture;
};