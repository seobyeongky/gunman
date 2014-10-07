#pragma once

#include <v8.h>
#include <SFML\Graphics.hpp>
#include <assert.h>

using namespace sf;

template <class TypeName>
inline v8::Local<TypeName> StrongPersistentToLocal(
    const v8::Persistent<TypeName>& persistent) {
  return *reinterpret_cast<v8::Local<TypeName>*>(
      const_cast<v8::Persistent<TypeName>*>(&persistent));
}

template <class TypeName>
inline v8::Local<TypeName> WeakPersistentToLocal(
    v8::Isolate* isolate,
    const v8::Persistent<TypeName>& persistent) {
  return v8::Local<TypeName>::New(isolate, persistent);
}
template <class TypeName>
inline v8::Local<TypeName> PersistentToLocal(
    v8::Isolate* isolate,
    const v8::Persistent<TypeName>& persistent) {
  if (persistent.IsWeak()) {
    return WeakPersistentToLocal(isolate, persistent);
  } else {
    return StrongPersistentToLocal(persistent);
  }
}

template <class T, class V>
inline T * GetSelf(const v8::PropertyCallbackInfo<V>& info)
{
	v8::Local<v8::Object> self = info.This();
    assert(!self.IsEmpty());
    assert(self->InternalFieldCount() > 0);
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
	return static_cast<T*>(ptr);
}

v8::Handle<v8::Value> JS_MakeColorObject(v8::Isolate * isolate, const Color & c);