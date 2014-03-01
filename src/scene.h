/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	scene.h

	기본 scene class의 header file입니다.
=======================================================*/

#pragma once

#include "internal.h"

class IScene
{
public:
	virtual			~IScene(){}

	virtual bool	 HandleWindowEvent(const sf::Event & e) = 0;
	virtual bool	 HandleClientMsg(client_msg_t & msg) = 0;
	virtual void	 FrameMove() = 0;
};