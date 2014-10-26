//
//  fake_windows.h
//  gunman
//
//  Created by 서병기 on 2014. 10. 15..
//  Copyright (c) 2014년 ooparts. All rights reserved.
//

#ifndef __gunman__fake_windows__
#define __gunman__fake_windows__

#ifdef _WIN32
#error not_for_windows
#endif

#include <stdio.h>
#include <wchar.h>

int _vscwprintf(const wchar_t *format, va_list argptr);
void _wfopen_s(FILE **fp, const wchar_t * path, const wchar_t * mode);

#define vswprintf_s vswprintf
#define _malloca alloca
#define _freea

#endif /* defined(__gunman__fake_windows__) */
