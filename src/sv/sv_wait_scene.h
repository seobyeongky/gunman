#pragma once

#include "sv_scene.h"

class SvWaitScene : public SvScene {
public:
	SvWaitScene();
	~SvWaitScene();

	virtual void Init();
	virtual void HandleNewCl(const client_t & new_cl);
	virtual void HandleGoneCl(const client_t & gone_cl);
	virtual void HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet);
	virtual void Go();
private:
	bool	begin_flag;
};