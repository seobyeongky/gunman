#include "v8_utils.h"
#include "../internal.h"

using namespace v8;

Handle<v8::Value> JS_MakeColorObject(v8::Isolate * isolate, const Color & c)
{
	EscapableHandleScope handle_scope(isolate);
	Handle<Object> obj = Object::New(isolate);
#define SETT(key,value) obj->Set(v8::String::NewFromUtf8(isolate, key), value)
	SETT("r", Integer::New(isolate, c.r));
	SETT("g", Integer::New(isolate, c.g));
	SETT("b", Integer::New(isolate, c.b));
	SETT("a", Integer::New(isolate, c.a));
	return handle_scope.Escape<Object>(obj);
}