#include "v8_sprite.h"
#include "v8_texture.h"
#include "v8_utils.h"
#include "v8_transformable.h"

#include "../global.h"

using namespace v8;

#define DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_SPRITE(that) DEFINE_HANDLE_SCOPE_AND_GET_SELF(Sprite, that)

Persistent<FunctionTemplate> JsSprite::constructor;


JsSprite::JsSprite(Isolate * isolate, Local<Object> handle)
	: ObjectWrap(handle), _texture(nullptr), _sprite()
{
    assert(handle->InternalFieldCount() > 1);
	handle->SetInternalField(0, External::New(isolate, &_sprite));
	handle->SetInternalField(1, External::New(isolate, this));
}

JsSprite::~JsSprite()
{
	if (_texture) _texture->Unref();
}

void JsSprite::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope handle_scope(args.GetIsolate());
	Local<Object> thiz = args.This();
	JsSprite* wrapped = new JsSprite(args.GetIsolate(), thiz);
	
	args.GetReturnValue().Set(thiz);
}

void JsSprite::Init(Isolate * isolate, Handle<ObjectTemplate> exports)
{
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Sprite"));
	tpl->InstanceTemplate()->SetInternalFieldCount(2);
	tpl->Inherit(v8_transformable::GetFunctionTemplate(isolate));

	// Prototype
	Local<ObjectTemplate> proto = tpl->PrototypeTemplate();
//	proto->SetInternalFieldCount(1);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "texture"), 0, JS_SetTexture);

	constructor.Reset(isolate, tpl);

	exports->Set(v8::String::NewFromUtf8(isolate, "Sprite"), PersistentToLocal(isolate, constructor));
}

void JsSprite::Finalize()
{
	constructor.Reset();
}

void JsSprite::JS_SetTexture(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF_FOR_SPRITE(info);
	auto obj = Local<Object>::Cast(value);
	if (obj->InternalFieldCount() < 2) return;

	Texture * texture = GetSelf<Texture>(obj);
	self->setTexture(*texture);

	JsSprite * that = GetWrappedObject<JsSprite>(info.This());
	JsTexture * wrapped = GetWrappedObject<JsTexture>(obj);
	wrapped->Ref();
	if (that->_texture) that->_texture->Unref();
	that->_texture = wrapped;
}