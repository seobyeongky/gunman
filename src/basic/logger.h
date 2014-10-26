/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	logger.h

	Logger class�� header file
=======================================================*/
#pragma once

// STL
#include <string>
#include <fstream>

// SFML
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

class Logger {
/*	file�� ����� ����ϴ�. */

public:
			 Logger(const wstring & file_name, WindowHandle h_wnd);
			~Logger();

	// ���� �˸�������, file�� ����� ����ϴ�.
	void	 Info(const wchar_t * format, ...);

	// �������� file�� ����� ����ϴ�.
	void	 Warning(const wchar_t * format, ...);

	// ������ ���� �� �Դϴ�. file�� ����� �����, MessageBox�� �����ݴϴ�.
	void	 Error(const wchar_t * format, ...);

private:
	wofstream		_f_stream;
	Clock			_clock;

	WindowHandle	_h_wnd;
};