#pragma once

#include <v8.h>

namespace v8_transformable
{
	void Init(v8::Isolate * isolate);
	v8::Local<v8::FunctionTemplate> GetFunctionTemplate(v8::Isolate * isolate);
}