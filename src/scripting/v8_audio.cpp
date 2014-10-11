#include "v8_audio.h"
#include "v8_utils.h"
#include "../util.h"
#include "../sfx_mgr.h"
#include "../global.h"

#define JS_FUNC(x) FunctionTemplate::New(x)

namespace js_audio
{
	using namespace v8;

	Persistent<Object> instance;
	wstring working_dir;
	audiere::OutputStreamPtr the_music;

	void PlayEffect(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		DirChanger change(working_dir.c_str());
		v8::String::Value path(args[0]);
		const wchar_t * wchar_list = reinterpret_cast<wchar_t*>(*path);
		G.sfx_mgr.Play(wchar_list);
	}

	void PlayMusic(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		DirChanger change(working_dir.c_str());
		v8::String::Value path(args[0]);
		const wchar_t * wchar_list = reinterpret_cast<wchar_t*>(*path);
		string multi;
		uni2multi(wchar_list, &multi);
		if (the_music = audiere::OpenSound(G.audio_device, multi.c_str(), true))
		{
			the_music->setRepeat(true);
			the_music->play();
			args.GetReturnValue().Set(true);
		}
		else
		{
			args.GetReturnValue().Set(false);
		}
	}

	void StopMusic(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		if (the_music)
		{
			the_music->stop();
			args.GetReturnValue().Set(true);
		}
		else
		{
			args.GetReturnValue().Set(false);
		}
	}

	void AudioGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		HandleScope handle_scope(info.GetIsolate());
		info.GetReturnValue().Set(PersistentToLocal(info.GetIsolate(), instance));
	}

	void Init(Isolate * isolate, Handle<ObjectTemplate> exports, const wstring & map_name)
	{
		working_dir = L"data/";
		working_dir += map_name;

		exports->SetAccessor(v8::String::NewFromUtf8(isolate, "Audio"), AudioGetter);
	}

	void Instantiate(Isolate * isolate)
	{
		Local<ObjectTemplate> tpl = ObjectTemplate::New();
		tpl->Set(v8::String::NewFromUtf8(isolate, "playEffect"), JS_FUNC(PlayEffect));
		tpl->Set(v8::String::NewFromUtf8(isolate, "playMusic"), JS_FUNC(PlayMusic));
		tpl->Set(v8::String::NewFromUtf8(isolate, "stopMusic"), JS_FUNC(StopMusic));

		instance.Reset(isolate, tpl->NewInstance());
	}

	void Finalize()
	{
		if (the_music) the_music->stop();
		instance.Reset();
	}
};