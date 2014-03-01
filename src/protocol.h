#pragma once

#define TO_UINT16 static_cast<unsigned short>

enum sv_state_t
{
	SV_WAIT,
	SV_PLAY
};

enum sv_to_cl_t
{
	SV_TO_CL_STATE,
	SV_TO_CL_CHAT,
	SV_TO_CL_INFO,
	SV_TO_CL_WILL_START,
	SV_TO_CL_COMMANDS
};

enum cl_to_sv_t
{
	CL_TO_SV_CHAT,
	CL_TO_SV_START,
	CL_TO_SV_READY_TO_RECV,
	CL_TO_SV_COMMANDS
};