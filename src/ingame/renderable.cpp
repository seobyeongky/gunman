#include "renderable.h"

using namespace v8;

JSRenderable::JSRenderable(Isolate * isolate, Local<Object> handle)
	: ObjectWrap(handle)
{
	// Ref걸고+++++++
	//할것도읎다.
}

void JSRenderable::draw(RenderTarget& target, RenderStates states) const
{
	Local<Object> self = handle();
	
	if (self->InternalFieldCount() < 2) return;

	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	Drawable * drawable = static_cast<Drawable*>(ptr);

	target.draw(*drawable, states);
}

int JSRenderable::GetHeight() const 
{
	return 1;
}

int JSRenderable::GetPriority() const
{
	return 0;
}