#include "util.h"

#include <io.h>
#include <locale.h>
#include <direct.h>

void Msgbox(LPCWSTR format_string, va_list arg_list, LPCWSTR title)
{
	int length = _vscwprintf(format_string, arg_list) + 1;  
	WCHAR *buf = static_cast<WCHAR *>(
		_malloca(length * sizeof(WCHAR)));
	vswprintf_s(buf, length, format_string, arg_list);
	
//	LeaveLog(buf);
	G.window.setMouseCursorVisible(true);
	MessageBox(G.window.getSystemHandle(), buf, title, 0U);
	G.window.setMouseCursorVisible(false);

	_freea(buf);
}

void ErrorMsg(LPCWSTR format_string, ...)
{
	va_list args;
	va_start(args, format_string);
	Msgbox(format_string, args, L"에러");
	va_end(args);
}

// Return true if the folder exists, false otherwise
bool folderExists(const char* folderName) {
    if (_access(folderName, 0) == -1) {
        //File not found
        return false;
    }

    DWORD attr = GetFileAttributesA((LPCSTR)folderName);
    if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        // File is not a directory
        return false;
    }

    return true;
}

bool createFolder(std::string folderName) {
    list<std::string> folderLevels;
    char* c_str = (char*)folderName.c_str();

    // Point to end of the string
    char* strPtr = &c_str[strlen(c_str) - 1];

    // Create a list of the folders which do not currently exist
    do {
        if (folderExists(c_str)) {
            break;
        }
        // Break off the last folder name, store in folderLevels list
        do {
            strPtr--;
        } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
        folderLevels.push_front(string(strPtr + 1));
        strPtr[1] = 0;
    } while (strPtr >= c_str);

    if (_chdir(c_str)) {
        return true;
    }

    // Create the folders iteratively
    for (list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
        if (CreateDirectoryA(it->c_str(), NULL) == 0) {
            return true;
        }
        _chdir(it->c_str());
    }

    return false;
}

DirChanger::DirChanger()
	: _pre_dir()
{
	Init();
}

DirChanger::DirChanger(const wchar_t * new_dir)
	: _pre_dir()
{
	Init();
	// Make directory if not exists
	string mnew_dir;
	uni2multi(new_dir, &mnew_dir);
	createFolder(mnew_dir.c_str());

	Change(new_dir);
}

DirChanger::~DirChanger()
{
	Change(_pre_dir.c_str());
}

void DirChanger::Change(const wchar_t * new_dir)
{
	_wchdir(new_dir);
}

void DirChanger::Init()
{
	{
		// Save current working directory
		size_t			buf_size = 128;
		vector<wchar_t>	buf(buf_size);
	
		for(;;)
		{
			_wgetcwd(&buf[0], buf_size);
			if(buf[0] != L'\0')
				break;
			buf_size *= 2;
			if(buf_size > 1024)
				G.logger->Error(L"DirChanger : working directory의 경로 이름 길이가 너무 깁니다.");
			buf.resize(buf_size);
		}
		_pre_dir.assign(&buf[0], buf.size());
	}
}

bool GetMatchedFileList(vector<wstring> * ptr, const wstring & wfilename)
{
	ptr->clear();

	_finddata_t fd;
    long handle;
	int count = 0;
	string filename;
	uni2multi(wfilename, &filename);
    handle=_findfirst(filename.c_str(), &fd);
    if (handle == -1) return false;

	do
	{
		WCHAR buf[512];
		size_t len = strlen(fd.name);
		swprintf_s(buf, L"%hs", fd.name, len);
		size_t wlen = wcslen(buf);
		wstring file_name(buf, buf + wlen);
		if (file_name != L"." && file_name != L"..")
			ptr->push_back(file_name);
     } while (_findnext(handle, &fd) != -1);
     
	_findclose(handle);

	 return true;
}

bool GetTextFromFile(const wstring & wfilename, wstring * buf)
{
	buf->clear();

	FILE * in = nullptr;
	_wfopen_s(&in, wfilename.c_str(), L"r");
	if (in == nullptr) return false;
	
	wchar_t BUF[1024];

	while (fgetws(BUF, sizeof(buf), in) != NULL)
	{
		*buf += BUF;
	}

	fclose(in);

	return true;
}

void JoinPath(const wstring& p1, const wstring& p2, wstring * ptr)
{
	if (ptr == nullptr) return;

	wchar_t sep = L'/';

#ifdef _WIN32
	sep = L'\\';
#endif

	if (p1.back() != sep)
		*ptr = p1 + sep + p2;
	else
		*ptr = p1 + p2;
}

void TransferCoord(const Transform & trans, const Event & e, Event * ne)
{
	*ne = e;
	// event should be transformed
	{
		if (e.type == Event::MouseMoved ||
			e.type == Event::MouseButtonPressed ||
			e.type == Event::MouseButtonReleased)
		{
			Vector2i point;
			if (e.type == Event::MouseMoved)
			{
				point.x = e.mouseMove.x;
				point.y = e.mouseMove.y;
			}
			else if (e.type == Event::MouseButtonPressed ||
				e.type == Event::MouseButtonReleased)
			{
				point.x = e.mouseButton.x;
				point.y = e.mouseButton.y;
			}

			Vector2f pointf(static_cast<float>(point.x), static_cast<float>(point.y));
			Vector2f nextpointf(trans.getInverse().transformPoint(pointf));
			Vector2i nextpoint(static_cast<int>(nextpointf.x), static_cast<int>(nextpointf.y));

			if (e.type == Event::MouseMoved)
			{
				ne->mouseMove.x = nextpoint.x;
				ne->mouseMove.y = nextpoint.y;
			}
			else if (e.type == Event::MouseButtonPressed ||
				e.type == Event::MouseButtonReleased)
			{
				ne->mouseButton.x = nextpoint.x;
				ne->mouseButton.y = nextpoint.y;
			}
		}
	}
}


CircleShape dbgpoint(10.f);