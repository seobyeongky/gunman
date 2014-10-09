#include "v8_sfml.h"
#include "v8_utils.h"
#include "v8_transformable.h"

#include "../global.h"

#include <codecvt>

using namespace v8;

#define DEFINE_HANDLE_SCOPE_AND_GET_SELF(T) 	HandleScope handle_scope(info.GetIsolate()); \
	T * self = GetSelf<T>(info);

#define DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT DEFINE_HANDLE_SCOPE_AND_GET_SELF(Text)

Persistent<FunctionTemplate> JsText::constructor;


JsText::JsText(Isolate * isolate, Local<Object> handle)
	: ObjectWrap(handle), _text("", G.default_font, 12)
{
    assert(handle->InternalFieldCount() > 0);
	handle->SetInternalField(0, External::New(isolate, &_text));
}

void JsText::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(args.GetIsolate());
	Local<Object> thiz = args.This();
	JsText * wrapped = new JsText(args.GetIsolate(), thiz);
	
	args.GetReturnValue().Set(thiz);
}

void JsText::Init(Isolate * isolate, Handle<ObjectTemplate> exports)
{
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Text"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->Inherit(v8_transformable::GetFunctionTemplate(isolate));

	// Prototype
	Local<ObjectTemplate> proto = tpl->PrototypeTemplate();
//	proto->SetInternalFieldCount(1);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "string"), JS_GetString, JS_SetString);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "color"), JS_GetColor, JS_SetColor);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "characterSize"), JS_GetCharacterSize, JS_SetCharacterSize);

	constructor.Reset(isolate, tpl);

	exports->Set(v8::String::NewFromUtf8(isolate, "Text"), PersistentToLocal(isolate, constructor));
}

void JsText::Finalize()
{
	constructor.Reset();
}

void JsText::JS_GetString(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT;
	wstring what = self->getString().toWideString();
	vector<uint16_t> utf16(what.begin(), what.end());
	info.GetReturnValue().Set(v8::String::NewFromTwoByte(info.GetIsolate(), &utf16[0], v8::String::kNormalString, utf16.size()));
}

void JsText::JS_SetString(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT;
	v8::String::Value str(value);
	const wchar_t * wchar_list = reinterpret_cast<wchar_t*>(*str);
	self->setString(wchar_list);
}

void JsText::JS_GetColor(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT;
	info.GetReturnValue().Set(JS_MakeColorObject(info.GetIsolate(), self->getColor()));
}

void JsText::JS_SetColor(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT;
	auto obj = Local<Object>::Cast(value);
#define EXTRACT(key) obj->Get(v8::String::NewFromUtf8(info.GetIsolate(), key))
	auto r = EXTRACT("r");
	auto g = EXTRACT("g");
	auto b = EXTRACT("b");
	auto a = EXTRACT("a");
	self->setColor(Color(r->Int32Value(), g->Int32Value(), b->Int32Value(), a->Int32Value()));
}

void JsText::JS_GetCharacterSize(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), self->getCharacterSize()));
}

void JsText::JS_SetCharacterSize(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_TEXT;
	auto num = Local<Number>::Cast(value);
	self->setCharacterSize(value->Int32Value());
}
