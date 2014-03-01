#pragma once

#include "sv_internal.h"

struct cl_context_t
{
	client_t	basic_info;
	int			delay_time;

	cl_context_t();
	cl_context_t(const client_t & _basic_info, int _delay_time);
};

typedef smap<ID, cl_context_t> client_map_t;

struct place_t
{
	cl_context_t *	cl;
	wstring			script_name;
	string			script_buf;
};

struct sv_global_t
{
	wstring				room_name;
	client_map_t		client_map;
	place_t				place[2];
	int					present_time;
	int					delta_time;

	sv_global_t(void)
	{
		place[0].cl = nullptr;
		place[1].cl = nullptr;
		delta_time = 0;
	}
};

extern sv_global_t svG;

void SendToAll(const Packet & packet);