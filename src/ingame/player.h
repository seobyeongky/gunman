#pragma once

#include "../internal.h"

struct player_t
{
	wstring		name;
	Color		color;

	player_t();
	player_t(const wstring & _name, const Color & _color);
};