#pragma once

#include "../internal.h"

class Champion;

enum
{
	ACTIVE_ABILITY = 1
	, PASSIVE_ABILITY = 2
	, TOGGLE_ABILITY = 4
};

struct Ability
{
	wstring						name;
	int							flag;
	function<void(Champion*)>	invoke;	// for ACTIVE ABILITY
	int							cooltime;
	int							range;
	bool						targeting;
	int							casttime;
	wstring						icon;
	Flag						uiflag;
};

extern smap<wstring, Ability*> ability_map;

//void LoadAbilities();
//void ReleaseAbilities();