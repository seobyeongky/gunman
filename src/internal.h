/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS 
=========================================================
	internal.h

	여러 기본적인 header file들을 포함시켰습니다.
=======================================================*/

#pragma once

// OPZNET
#include <opznet/client.h>

// SFML
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// audiere
#include <audiere.h>

// WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

// CRT
#include <cstring>
#include <cassert>
#ifdef _DEBUG
	#include <cstdio>
#endif

// STL
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <string>

// BASIC
#include "basic/s_map.h"
#include "basic/d_input.h"
#include "basic/logger.h"
#include "basic/edit_text.h"
#include "basic/widget.h"
#include "basic/menu.h"
#include "basic/string_convert.h"
#include "basic/item.h"
#include "basic/cleaner.h"
#include "basic/observable.h"
#include "basic/flag.h"

// PROTOCOL
#include "protocol.h"

using namespace std;
using namespace opznet;
using namespace sf;

#define SafeDelete(p) if(p != nullptr){delete p; p = nullptr; }
#define SafeDeleteArray(p) if(p != nullptr){delete[] p; p = nullptr; }
#define SafeRelease(p) if(p != nullptr){p->Release(); p = nullptr; }

#define UNICODE_ESC L'\x1b'