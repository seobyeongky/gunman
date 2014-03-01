#include "logger.h"

#include <chrono>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <io.h>
#include <locale.h>
#include <direct.h>

Logger::Logger(const wstring & file_name, WindowHandle h_wnd)
	: _f_stream(), _clock(), _h_wnd(h_wnd)
{
	_f_stream.imbue(locale("korean"));
	_f_stream.open(file_name, fstream::app);

	typedef chrono::system_clock clock_t;
	auto		now = clock_t::now();
	time_t		now_c = clock_t::to_time_t(now);
	struct tm	parts;
	localtime_s(&parts, &now_c);

	_clock.restart();

	_f_stream	<< L"=======================================================" << endl
				<< L"\t" << (1900 + parts.tm_year) << L"년"
				<< (1 + parts.tm_mon) << L"월 "
				<< parts.tm_mday << L"일\t"
				<< parts.tm_hour << L"시 "
				<< parts.tm_min << L"분 "
				<< parts.tm_sec << L"초부터" << endl
				<< L"=======================================================" << endl;
}

Logger::~Logger()
{
	_f_stream.close();
}

void Logger::Info(const wchar_t * format, ...)
{
	va_list args;
	va_start(args, format);
	int length = _vscwprintf(format, args) + 1;  
	WCHAR *buf = static_cast<WCHAR *>(_malloca(length * sizeof(WCHAR)));
	vswprintf_s(buf, length, format, args);

	float elapsed_time = _clock.getElapsedTime().asSeconds();
	_f_stream << elapsed_time << L"초 : " << buf << endl;
	wprintf(L"%f초 : %s\n", elapsed_time , buf);
	va_end(args);
}

void Logger::Warning(const wchar_t * format, ...)
{
	va_list args;
	va_start(args, format);
	int length = _vscwprintf(format, args) + 1;  
	WCHAR *buf = static_cast<WCHAR *>(_malloca(length * sizeof(WCHAR)));
	vswprintf_s(buf, length, format, args);

	float elapsed_time = _clock.getElapsedTime().asSeconds();
	_f_stream << elapsed_time << L"초 : [경고]" << buf << endl;
	wprintf(L"%f초 : [경고]%s\n", elapsed_time , buf);
	va_end(args);
}

void Logger::Error(const wchar_t * format, ...)
{
	va_list args;
	va_start(args, format);
	int length = _vscwprintf(format, args) + 1;  
	WCHAR *buf = static_cast<WCHAR *>(_malloca(length * sizeof(WCHAR)));
	vswprintf_s(buf, length, format, args);

	float elapsed_time = _clock.getElapsedTime().asSeconds();
	_f_stream << elapsed_time << L"초 : [오류]" << buf << endl;
	wprintf(L"%f초 : [오류]%s\n", elapsed_time , buf);

	MessageBox(_h_wnd, buf, L"오류", MB_OK | MB_ICONERROR);
	va_end(args);
}