//
//  fake_windows.cpp
//  gunman
//
//  Created by 서병기 on 2014. 10. 15..
//  Copyright (c) 2014년 ooparts. All rights reserved.
//

#include "fake_windows.h"
#include "../util.h"

int _vscwprintf(const wchar_t *format, va_list argptr)
{
    return(vswprintf(0, 0, format, argptr));
}

void _wfopen_s(FILE **fp, const wchar_t * path, const wchar_t * mode)
{
    std::string mpath, mmode;
    uni2multi(path, &mpath);
    uni2multi(mode, &mmode);
    *fp = fopen(mpath.c_str(), mmode.c_str());
}
