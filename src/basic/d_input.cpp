#include "d_input.h"

#include <cassert>

///////////////////// DInput class ///////////////////////

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#define KEY_DOWN(name, key) (name[key] & 0x80)

class DInput {
/*	DirectX-8 Input API를 이용한는 keyboard input 관리 class */

public:
			 DInput(WindowHandle h_wnd);
			~DInput();

	bool	 Init();

	void	 Update();

	bool	 CheckKeyState(int v_key, key_state_t state);

	void	 AcquireKeyboardDevice();
	void	 ReleaseKeyboardDevice();
	bool	 CheckKeyboardDeviceAcquired() const;


private:
	WindowHandle			_h_wnd;

	LPDIRECTINPUT8			_d_input8;
	LPDIRECTINPUTDEVICE8	_keyboard;

	CHAR					_prev_keyboard_state[256];
	CHAR					_keyboard_state[256];
};

DInput::DInput(WindowHandle h_wnd) :
	_h_wnd(h_wnd),
	_d_input8(NULL),
	_keyboard(NULL)
{
}

DInput::~DInput()
{
	_keyboard->Unacquire();
	_d_input8->Release();
}

bool DInput::Init()
{
	DirectInput8Create(	GetModuleHandle(NULL),
						DIRECTINPUT_VERSION,
						IID_IDirectInput8,
						(void **)&_d_input8,
						NULL );
	
	if(_d_input8 == NULL)
	{
		MessageBox(_h_wnd, L"DInput : DInput : DirectX-8 Input 만들기 실패!", L"오류", MB_OK | MB_ICONERROR);
		return false;
	}

	_d_input8->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	
	if(_keyboard == NULL)
	{
		MessageBox(_h_wnd, L"DInput : DInput : Keyboard device 만들기 실패!", L"오류", MB_OK | MB_ICONERROR);
		return false;
	}

	_keyboard->SetDataFormat(&c_dfDIKeyboard);
	_keyboard->SetCooperativeLevel(_h_wnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	_keyboard->Acquire();

	return true;
}

void DInput::Update()
{
	memcpy(_prev_keyboard_state, _keyboard_state, sizeof(_prev_keyboard_state));

	_keyboard->GetDeviceState(	sizeof(_keyboard_state),
								(LPVOID)&_keyboard_state);
}

bool DInput::CheckKeyState(int v_key, key_state_t state)
{
	switch(state)
	{
	case KEY_DESCENDING:
		if(	! KEY_DOWN(_prev_keyboard_state, v_key) &&
			KEY_DOWN(_keyboard_state, v_key) ) return true;
		else return false;

	case KEY_ASCENDING:
		if(	KEY_DOWN(_prev_keyboard_state, v_key) &&
			! KEY_DOWN(_keyboard_state, v_key) ) return true;
		else return false;

	case KEY_BOTTOM_HELD:
		if(KEY_DOWN(_keyboard_state, v_key)) return true;
		else return false;

	case KEY_TOP_HELD:
		if( ! KEY_DOWN(_keyboard_state, v_key)) return true;
		else return false;

	default:
		assert(!L"DInput : CheckKeyState : 알 수 없는 state");
	}

	return false;
}

/////////////////// d_input_api_t ////////////////////

DInput * D_INPUT = nullptr;

bool d_input_api_t::Create(WindowHandle h_wnd)
{
	if(D_INPUT == nullptr)
	{
		D_INPUT = new DInput(h_wnd);

		if( !D_INPUT->Init() ) return false; 

		return true;
	}

	return false;
}

void d_input_api_t::Delete()
{
	if(D_INPUT != nullptr)
	{
		delete D_INPUT;
		D_INPUT = nullptr;
	}
}

void d_input_api_t::Update()
{
	assert(D_INPUT != nullptr);

	D_INPUT->Update();
}

bool d_input_api_t::CheckKeyState(int v_key, key_state_t state)
{
	assert(D_INPUT != nullptr);

	return D_INPUT->CheckKeyState(v_key, state);
}