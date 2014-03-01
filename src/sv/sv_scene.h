#pragma once

#include "sv_internal.h"

class SvScene {
public:
	// member 변수들을 초기화시키는 함수입니다.
	// derived scene은 instance가 재사용되기 때문에 생성자함수가 아닌 Init함수로 초기화시키십시오.
	virtual void		Init() = 0;

	// 새로운 client가 접속한 직후에 호출됩니다.
	// server context에는 새 client가 아직 없는 상태입니다.
	virtual void		HandleNewCl(const client_t & new_cl) = 0;

	// 어떤 client가 서버에서 나간 뒤에 호출됩니다.
	// server context에는 나간 client가 아직 있는 상태입니다.
	virtual void		HandleGoneCl(const client_t & gone_cl) = 0;


	virtual void		HandleRecvPacket(	const client_t & cl,
											cl_to_sv_t header,
											Packet & recv_packet);

	virtual void		Go() = 0;
};