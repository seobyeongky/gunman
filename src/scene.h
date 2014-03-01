/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	scene.h

	�⺻ scene class�� header file�Դϴ�.
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