#include "map_list.h"

#include <io.h>
#include <direct.h>
#include <wchar.h>
#include <stdio.h>

namespace MapList 
{
	void GetList(std::vector<std::wstring> * ptr)
	{
		_finddata_t fd;
		long handle = _findfirst("data/*", &fd);

		if (handle == -1) return;

		do
		{
			wchar_t buf[512];
			size_t len = strlen(fd.name);
			swprintf_s(buf, L"%hs", fd.name, len);
			if (fd.attrib & _A_SUBDIR)
			{
				size_t wlen = wcslen(buf);
				std::wstring file_name(buf, buf + wlen);
				if (file_name != L"." && file_name != L".." && file_name != L"system")
					ptr->push_back(file_name);
			}
		} while (_findnext(handle, &fd) != -1);
		
		_findclose(handle);
	}
}