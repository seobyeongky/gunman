#include "v8_texture.h"
#include "v8_utils.h"
#include "../util.h"

using namespace v8;

#define DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXTURE(that) DEFINE_HANDLE_SCOPE_AND_GET_SELF(Texture,that)

Persistent<FunctionTemplate> JsTexture::constructor;
wstring working_dir;


JsTexture::JsTexture(Isolate * isolate, Local<Object> handle)
	: ObjectWrap(handle), _texture()
{
    assert(handle->InternalFieldCount() > 1);
	handle->SetInternalField(0, External::New(isolate, &_texture));
	handle->SetInternalField(1, External::New(isolate, this));
}

void JsTexture::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(args.GetIsolate());
	Local<Object> thiz = args.This();
	JsTexture * wrapped = new JsTexture(args.GetIsolate(), thiz);
	
	args.GetReturnValue().Set(thiz);
}

void JsTexture::Init(Isolate * isolate, Handle<ObjectTemplate> exports, const wstring & map_name)
{
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Texture"));
	tpl->InstanceTemplate()->SetInternalFieldCount(2);

	// Prototype
	Local<ObjectTemplate> proto = tpl->PrototypeTemplate();
//	proto->SetInternalFieldCount(1);
	Texture t;
	proto->Set(v8::String::NewFromUtf8(isolate, "loadFromFile"), FunctionTemplate::New(isolate, JS_LoadFromFile));
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "width"), JS_GetWidth);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "height"), JS_GetHeight);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "smooth"), JS_GetSmooth, JS_SetSmooth);

	constructor.Reset(isolate, tpl);

	exports->Set(v8::String::NewFromUtf8(isolate, "Texture"), PersistentToLocal(isolate, constructor));

	working_dir = L"data/";
	working_dir += map_name;
}

void JsTexture::Finalize()
{
	constructor.Reset();
}

void JsTexture::JS_LoadFromFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXTURE(args);

	bool result = false;
	{
		DirChanger change(working_dir.c_str());
		v8::String::Value path(args[0]);
		const wchar_t * wchar_list = reinterpret_cast<wchar_t*>(*path);
		string multi;
		uni2multi(wchar_list, &multi);
		
		result = self->loadFromFile(multi);
	}
	args.GetReturnValue().Set(Boolean::New(args.GetIsolate(), result));
}

void JsTexture::JS_GetWidth(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXTURE(info);
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), self->getSize().x));
}

void JsTexture::JS_GetHeight(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXTURE(info);
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), self->getSize().y));
}

void JsTexture::JS_GetSmooth(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXTURE(info);
	info.GetReturnValue().Set(Boolean::New(info.GetIsolate(), self->isSmooth()));
}

void JsTexture::JS_SetSmooth(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXTURE(info);
	self->setSmooth(value->BooleanValue());
}
