#pragma once

#include "sv_scene.h"
#include "../proto/command.pb.h"

class SvPlayScene : public SvScene {
public:
	SvPlayScene();
	~SvPlayScene();

	virtual void Init();
	virtual void HandleNewCl(const client_t & new_cl);
	virtual void HandleGoneCl(const client_t & gone_cl);
	virtual void HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet);
	virtual void Go();

private:
	smap<ID, bool>	_ready_map;
	bool			_begin;

	vector<Command>	_commands;
	smap<ID, bool>	_ack_map;

	void CheckClReady();

	bool IsGotAllCommands();
	void DelegateCommands();
	void UpdateLogic();

	void PushCommands(ID cid, Packet & recv_packet);
};