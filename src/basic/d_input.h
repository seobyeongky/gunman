/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	d_input.h

	Direct Input API를 제공합니다.

	D_INPUT_API Singleton 객체를 이용하십시오.

	경고 : Thread safe를 보장할 수 없습니다.
=======================================================*/

#pragma once

#include <SFML/Window.hpp>

using namespace sf;

enum key_state_t
{
	KEY_DESCENDING,
	KEY_ASCENDING,
	KEY_BOTTOM_HELD,
	KEY_TOP_HELD
};

struct d_input_api_t
{
	bool Create(WindowHandle h_wnd);
	void Delete();

	void Update();
	bool CheckKeyState(int v_key, key_state_t state);
};

extern d_input_api_t D_INPUT_API;