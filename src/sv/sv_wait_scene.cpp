#include "sv_wait_scene.h"
#include "sv_global.h"
#include "sv_util.h"
#include "sv_director.h"
#include "sv_play_scene.h"

SvWaitScene::SvWaitScene()
	: begin_flag(false)
{
}

SvWaitScene::~SvWaitScene()
{
}

void SvWaitScene::Init()
{
}

void SvWaitScene::HandleNewCl(const client_t & new_cl)
{
}

void SvWaitScene::HandleGoneCl(const client_t & gone_cl)
{
}

void SvWaitScene::HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet)
{
	switch (header)
	{
	case CL_TO_SV_START:
		{
			if (begin_flag) return;
			if (cl.id != 0u)
				return;
			wstring map;
			recv_packet >> map;
			Packet sendpacket;
			sendpacket << TO_UINT16(SV_TO_CL_WILL_START) << map;
			for (auto & cl : svG.client_map)
				SafeSend(cl.key(), sendpacket);
			begin_flag = true;
			SvDirector::SwitchScene(new SvPlayScene());
		}
	default:
		SvScene::HandleRecvPacket(cl, header, recv_packet);
	}
}

void SvWaitScene::Go()
{
}