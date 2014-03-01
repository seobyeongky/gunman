#include "sv_global.h"

cl_context_t::cl_context_t()
	: basic_info(), delay_time(0)
{
}
cl_context_t::cl_context_t(const client_t & _basic_info, int _delay_time)
	: basic_info(_basic_info), delay_time(_delay_time)
{
}

void SendToAll(const Packet & packet)
{
	for (auto it = svG.client_map.begin(); it != svG.client_map.end(); ++it)
		SafeSend((*it).key(), packet);
}