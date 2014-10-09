#pragma once

#include "v8_object_wrap.h"

#include <SFML\Graphics.hpp>
using namespace sf;

class JsTexture;

class JsSprite: public ObjectWrap
{
public:
	static void Init(v8::Isolate * isolate, v8::Handle<v8::ObjectTemplate> exports);
	static void Finalize();

private:
	JsSprite(v8::Isolate * isolate, v8::Local<v8::Object> handle);
	virtual ~JsSprite();

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static void JS_GetTexture(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_SetTexture(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

	static v8::Persistent<v8::FunctionTemplate> constructor;

	JsTexture * _texture;
	Sprite _sprite;
};