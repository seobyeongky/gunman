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
	, _void_input_players()
	, _inputs()
//	, _ack_map()
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
//	_ack_map.erase(gone_cl.id);
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
	case CL_TO_SV_INPUTS:
		{
			SaveInputs(cl.id, recv_packet);
		}
		break;
	default:
		SvScene::HandleRecvPacket(cl, header, recv_packet);
	}
}

void SvPlayScene::Go()
{
/*	if (_begin)
	{
		UpdateLogic();
	}
	*/
	Broadcast();
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

void SvPlayScene::Broadcast()
{
	static string outbuf;

	Packet sendpacket;
	sendpacket << TO_UINT16(SV_TO_CL_BROADCAST)
		<< _inputs.size();

	for (auto & input : _inputs)
	{
		input.AppendToString(&outbuf);
		sendpacket << outbuf;
		outbuf.clear();
	}

	sendpacket << TO_UINT16(_void_input_players.size());
	for (ID cid : _void_input_players)
		sendpacket << cid;

	for (auto & cl : svG.client_map)
	{
		SafeSend(cl.key(), sendpacket);
	}

	_inputs.clear();
}

/*
void SvPlayScene::UpdateLogic()
{
	if (IsGotAllCommands()) DelegateCommands();
}
*/

void SvPlayScene::SaveInputs(ID cid, Packet & recv_packet)
{
	size_t nr_inputs;
	recv_packet >> nr_inputs;
	if (nr_inputs == 0)
	{
		auto & ct = _void_input_players;
		if (find(ct.begin(), ct.end(), cid) == ct.end())
			ct.push_back(cid);
		return;
	}

	static string inbuf;
	for (size_t i = 0U; i < nr_inputs; ++i)
	{
		Input input;
		recv_packet >> inbuf;
		input.ParseFromString(inbuf);
		input.set_pid(cid);
		_inputs.push_back(input);
		inbuf.clear();
	}
}