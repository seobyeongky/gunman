#include "v8_sfml.h"
#include "v8_utils.h"

using namespace v8;


namespace v8_transformable
{

Persistent<FunctionTemplate> tpl_ref;

#define DEFINE_HANDLE_SCOPE_AND_GET_SELF 	HandleScope handle_scope(info.GetIsolate()); \
	Transformable * self = GetSelf<Transformable>(info);


void New(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	HandleScope handle_scope(info.GetIsolate());

	info.GetReturnValue().Set(info.This());
}

void GetX(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getPosition().x)));
}

void SetX(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	float x = static_cast<float>(num->NumberValue());
	float y = self->getPosition().y;
	self->setPosition(x, y);
}

void GetY(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getPosition().y)));
}

void SetY(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	float x = self->getPosition().x;
	float y = static_cast<float>(num->NumberValue());
	self->setPosition(x, y);
}

void GetRotation(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getRotation())));
}

void SetRotation(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	self->setRotation(static_cast<float>(num->NumberValue()));
}

void GetScaleX(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getScale().x)));
}

void SetScaleX(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	float factorX = static_cast<float>(num->NumberValue());
	float factorY = self->getScale().y;
	self->setScale(factorX, factorY);
}

void GetScaleY(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getScale().y)));
}

void SetScaleY(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	float factorX = self->getScale().x;
	float factorY = static_cast<float>(num->NumberValue());
	self->setScale(factorX ,factorY);
}

void GetOriginX(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getOrigin().x)));
}

void SetOriginX(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	float originX = static_cast<float>(num->NumberValue());
	float originY = self->getOrigin().y;
	self->setOrigin(originX, originY);
}

void GetOriginY(Local<v8::String> name, const PropertyCallbackInfo<Value>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), static_cast<double>(self->getOrigin().y)));
}

void SetOriginY(Local<v8::String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	DEFINE_HANDLE_SCOPE_AND_GET_SELF;
	auto num = Local<Number>::Cast(value);
	float originX = self->getOrigin().x;
	float originY = static_cast<float>(num->NumberValue());
	self->setOrigin(originX, originY);
}

void Init(Isolate * isolate)
{
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Transformable"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Local<ObjectTemplate> proto = tpl->PrototypeTemplate();
//	proto->SetInternalFieldCount(1);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "x"), GetX, SetX);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "y"), GetY, SetY);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "rotation"), GetRotation, SetRotation);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "scaleX"), GetScaleX, SetScaleX);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "scaleY"), GetScaleY, SetScaleY);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "originX"), GetOriginX, SetOriginX);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "originY"), GetOriginY, SetOriginY);
	tpl_ref.Reset(isolate, tpl);
}

Local<FunctionTemplate> GetFunctionTemplate(Isolate * isolate)
{
	return PersistentToLocal(isolate, tpl_ref);
}

}