#pragma once

#include "internal.h"
#include "sfx_mgr.h"

struct global_t
{
	RenderWindow				window;

	Logger *					logger;

	Font						default_font;
	Font						title_font;

	smap<wstring, Texture *>	texture_map;
	smap<wstring, Sprite *>		sprite_map;

	audiere::AudioDevicePtr		audio_device;
	audiere::OutputStreamPtr	bg_music;
	opzSFXMgr					sfx_mgr;


	int							present_time;
	int							delta_time;

	wstring						my_name;
};

extern global_t G;