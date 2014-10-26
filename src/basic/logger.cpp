#include "logger.h"

#include <chrono>
#include <locale.h>

#include "../util.h"

#ifdef _WIN32
#   ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#   include <io.h>
#   include <direct.h>
#else
#   include "../osx/fake_windows.h"
#endif

Logger::Logger(const wstring & file_name, WindowHandle h_wnd)
	: _f_stream(), _clock(), _h_wnd(h_wnd)
{
	_f_stream.imbue(locale("korean"));
#ifdef _WIN32
	_f_stream.open(file_name, fstream::app);
#else
    string mfilename;
    uni2multi(file_name, &mfilename);
    _f_stream.open(mfilename, fstream::app);
#endif
	typedef chrono::system_clock clock_t;
	auto		now = clock_t::now();
	time_t		now_c = clock_t::to_time_t(now);
	struct tm	parts;
#ifdef _WIN32
	localtime_s(&parts, &now_c);
#else
    parts = *localtime(&now_c);
#endif
	_clock.restart();

	_f_stream	<< L"=======================================================" << endl
				<< L"\t" << (1900 + parts.tm_year) << L"년"
				<< (1 + parts.tm_mon) << L"월"
				<< parts.tm_mday << L"일\t"
				<< parts.tm_hour << L"시 "
				<< parts.tm_min << L"분 "
				<< parts.tm_sec << L"초" << endl
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
	wchar_t *buf = static_cast<wchar_t *>(_malloca(length * sizeof(wchar_t)));
	vswprintf_s(buf, length, format, args);

	float elapsed_time = _clock.getElapsedTime().asSeconds();
	_f_stream << elapsed_time << L"√  : " << buf << endl;
	wprintf(L"%f : %s\n", elapsed_time , buf);
    _freea(buf);
	va_end(args);
}

void Logger::Warning(const wchar_t * format, ...)
{
	va_list args;
	va_start(args, format);
	int length = _vscwprintf(format, args) + 1;  
	wchar_t *buf = static_cast<wchar_t *>(_malloca(length * sizeof(wchar_t)));
	vswprintf_s(buf, length, format, args);

	float elapsed_time = _clock.getElapsedTime().asSeconds();
	_f_stream << elapsed_time << L"√  : [∞Ê∞Ì]" << buf << endl;
	wprintf(L"%f : [∞Ê∞Ì]%s\n", elapsed_time , buf);
    _freea(buf);
	va_end(args);
}

void Logger::Error(const wchar_t * format, ...)
{
	va_list args;
	va_start(args, format);
	int length = _vscwprintf(format, args) + 1;  
	wchar_t *buf = static_cast<wchar_t *>(_malloca(length * sizeof(wchar_t)));
	vswprintf_s(buf, length, format, args);

	float elapsed_time = _clock.getElapsedTime().asSeconds();
	_f_stream << elapsed_time << L"√  : [ø¿∑˘]" << buf << endl;
	wprintf(L"%f : [ø¿∑˘]%s\n", elapsed_time , buf);

    ErrorMsg(buf);
    _freea(buf);
	va_end(args);
}