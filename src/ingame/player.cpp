#include "player.h"

player_t::player_t() : name(), color()
{
}
player_t::player_t(const wstring & _name, const Color & _color)
	: name(_name), color(_color)
{
}