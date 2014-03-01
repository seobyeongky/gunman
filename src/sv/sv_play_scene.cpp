#include "sv_play_scene.h"
#include "../logic.h"
#include "sv_director.h"
#include "sv_global.h"
#include "sv_wait_scene.h"
#include "../global.h"

#include <sstream>


SvPlayScene::SvPlayScene()
	: _ready_map()
	, _begin(false)
	, _commands()
	, _ack_map()
{

}

SvPlayScene::~SvPlayScene()
{
}

void SvPlayScene::Init()
{
}

void SvPlayScene::HandleNewCl(const client_t & new_cl)
{
	DisconnectClient(new_cl.id);
}

void SvPlayScene::HandleGoneCl(const client_t & gone_cl)
{
	_ready_map.erase(gone_cl.id);
	_ack_map.erase(gone_cl.id);
	CheckClReady();
}

void SvPlayScene::HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet)
{
	switch (header)
	{
	case CL_TO_SV_READY_TO_RECV:
		{
			if (_begin) return;
			_ready_map[cl.id] = true;
			CheckClReady();
		}
		break;
	case CL_TO_SV_COMMANDS:
		{
			if (_ack_map[cl.id])
			{
				G.logger->Warning(L"Server : (%d)%s 중복 ack 전송", cl.id, cl.name);
				return;
			}
			
			_ack_map[cl.id] = true;

			PushCommands(cl.id, recv_packet);
		}
		break;
	default:
		SvScene::HandleRecvPacket(cl, header, recv_packet);
	}
}

void SvPlayScene::Go()
{
	if (_begin)
	{
		UpdateLogic();
	}
}

void SvPlayScene::CheckClReady()
{
	bool ok = true;

	for (auto & cl : svG.client_map)
	{
		if (_ready_map[cl.key()] == false)
		{
			ok = false;
			break;
		}
	}

	if (ok)
	{
		_begin = true;
	}
}

bool SvPlayScene::IsGotAllCommands()
{
	bool ok = true;

	for (auto & cl : svG.client_map)
	{
		if (_ack_map[cl.key()] == false)
		{
			ok = false;
			break;
		}
	}

	return ok;
}

void SvPlayScene::DelegateCommands()
{
	for (auto & it : svG.client_map)
	{
		_ack_map[it.key()] = false;
		Packet sendpacket;
		sendpacket << TO_UINT16(SV_TO_CL_COMMANDS)
			<< _commands.size();
		string outbuf;

		for (auto & c : _commands)
		{
			c.AppendToString(&outbuf);
			sendpacket << outbuf;
			outbuf.clear();
		}

		for (auto & cl : svG.client_map)
		{
			SafeSend(cl.key(), sendpacket);
		}

		_commands.clear();
	}
}

void SvPlayScene::UpdateLogic()
{
	if (IsGotAllCommands()) DelegateCommands();
}

void SvPlayScene::PushCommands(ID cid, Packet & recv_packet)
{
	size_t nr_commands;
	recv_packet >> nr_commands;
	string inbuf;
	for (size_t i = 0U; i < nr_commands; ++i)
	{
		Command c;
		recv_packet >> inbuf;
		c.ParseFromString(inbuf);
		c.set_pid(cid);
		_commands.push_back(c);
		inbuf.clear();
	}
}