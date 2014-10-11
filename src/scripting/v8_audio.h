#include <v8.h>
#include <string>

namespace js_audio
{
	void Init(v8::Isolate * isolate, v8::Handle<v8::ObjectTemplate> exports, const std::wstring & map_name);
	void Instantiate(v8::Isolate * isolate);
	void Finalize();
};