#pragma once

#include "../internal.h"
#include "../scripting/v8_object_wrap.h"
#include <v8.h>

enum
{
	PR_GROUND_UNIT = 0,
	PR_ANI00 = 1,
	PR_ARROW = 2
};

class IRenderable : public Drawable, public Transformable
{
public:
	virtual ~IRenderable(){}

	virtual int GetHeight() const = 0;
	virtual int GetPriority() const = 0;
};


class JSRenderable : public IRenderable, public ObjectWrap
{
public:
	JSRenderable(v8::Isolate * isolate, v8::Local<v8::Object> handle);

	virtual int GetHeight() const;
	virtual int GetPriority() const;

protected:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
/*	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void JS_GetHeight(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_SetHeight(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void JS_GetPriority(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void JS_SetPriority(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

	static v8::Persistent<v8::FunctionTemplate> constructor
	
	int _height;
	int _priority;
	*/
};
