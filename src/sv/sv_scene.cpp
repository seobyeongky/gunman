#include "sv_scene.h"
#include "sv_global.h"

void SvScene::HandleRecvPacket(	const client_t & cl,
								cl_to_sv_t header,
								Packet & recv_packet)
{
	switch(header)
	{
	case CL_TO_SV_CHAT:
		{
			wstring msg;
			if (!(recv_packet >> msg)) return;

			Packet send_packet;
			send_packet	<< TO_UINT16(SV_TO_CL_CHAT)
						<< cl.id
						<< msg;

			for (auto it = svG.client_map.begin(); it != svG.client_map.end(); ++it)
			{
				if ((*it).key() != cl.id) SafeSend((*it).key(), send_packet);
			}
		}
		break;
	}
}