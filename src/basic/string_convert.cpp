
#include "string_convert.h"
#include <stdlib.h>

// #include <sys/errno.h>

const size_t MY_BUFSIZE = 1024;


// fixme : Possible buffer overflow
void multi2uni(const std::string & src, std::wstring * dest)
{
	wchar_t buf[MY_BUFSIZE];
	const char * srcptr = src.c_str();
	dest->clear();
    mbstowcs(buf, srcptr, MY_BUFSIZE - 1);
    dest->assign(buf);
}

// fixme : Possible buffer overflow
void uni2multi(const std::wstring & src, std::string * dest)
{
	char buf[MY_BUFSIZE];
	const wchar_t * srcptr = src.c_str();
	dest->clear();
	wcstombs(buf, srcptr, MY_BUFSIZE - 1);
	dest->assign(buf);
}
