#include "util.h"

#ifdef _WIN32
#   include <io.h>
#   include <direct.h>
#else
#   include "osx/utils.h"
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <unistd.h>
#   include <dirent.h>
#   include <fnmatch.h>
#endif

#include <locale.h>
//#include "basic/string_convert.h"


#ifndef _WIN32
// fake version of _vscwprintf
int _vscwprintf(const wchar_t *format, va_list argptr)
{
    return(vswprintf(0, 0, format, argptr));
}
#endif

void Msgbox(const wchar_t * format_string, va_list arg_list, const wchar_t * title)
{
	int length = _vscwprintf(format_string, arg_list) + 1;  
#ifdef _WIN32
    wchar_t *buf = static_cast<wchar_t *>(
		_malloca(length * sizeof(wchar_t)));
#else
    wchar_t * buf = static_cast<wchar_t *>(alloca(length * sizeof(wchar_t)));
#endif

#ifdef _WIN32
    vswprintf_s(buf, length, format_string, arg_list);
#else
    vswprintf(buf, length, format_string, arg_list);
#endif

    //	LeaveLog(buf);
	G.window.setMouseCursorVisible(true);

#ifdef _WIN32
    MessageBox(G.window.getSystemHandle(), buf, title, 0U);
#else
    std::string theTitle, theMessage;
    uni2multi(buf, &theMessage);
    uni2multi(title, &theTitle);
    MessageBox_cocoa(theTitle.c_str(), theMessage.c_str());
#endif
	
    G.window.setMouseCursorVisible(false);

#ifdef _WIN32
	_freea(buf);
#endif
}

void ErrorMsg(const wchar_t * format_string, ...)
{
	va_list args;
	va_start(args, format_string);
	Msgbox(format_string, args, L"에러");
	va_end(args);
}

// Return true if the folder exists, false otherwise
bool folderExists(const char* folderName)
{
#ifdef _WIN32
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
#else
    
    struct stat info;
    if( stat( folderName, &info ) != 0 )
        return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
        return true;
    else
        return false;

#endif
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

/*    if (chdir(c_str)) {
        return true;
    }
*/
    // Create the folders iteratively
    for (list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
#ifdef _WIN32
        if (CreateDirectoryA(it->c_str(), NULL) == 0) {
            return true;
        }
#else
        mkdir(it->c_str(),  0755);
#endif
        chdir(it->c_str());
    }

//    return false;
    return true;
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
    std::wstring uni;
    multi2uni(_pre_dir, &uni);
	Change(uni.c_str());
}

void DirChanger::Change(const wchar_t * new_dir)
{
#ifdef _WIN32
    _wchdir(new_dir);
#else
    std::string multi;
    uni2multi(new_dir, &multi);
    chdir(multi.c_str());
#endif
}

void DirChanger::Init()
{
	{
		// Save current working directory
		size_t			buf_size = 128;
		vector<char>	buf(buf_size);
	
		for(;;)
		{
            getcwd(&buf[0], buf_size);

            if(buf[0] != '\0')
				break;
			buf_size *= 2;
			if(buf_size > 1024)
				G.logger->Error(L"DirChanger : working directory¿« ∞Ê∑Œ ¿Ã∏ß ±Ê¿Ã∞° ≥ π´ ±È¥œ¥Ÿ.");
			buf.resize(buf_size);
		}
		_pre_dir.assign(&buf[0], buf.size());
	}
}

bool GetMatchedFileList(vector<wstring> * ptr, const wstring & wfilename)
{
	ptr->clear();
    string filename;
    uni2multi(wfilename, &filename);

#ifdef _WIN32
	_finddata_t fd;
    long handle;
	int count = 0;
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
#else
    DIR           *d;
    struct dirent *dir;
    d = opendir(".");
    
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
//            printf("%s\n", dir->d_name);
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
            if (fnmatch(filename.c_str(), dir->d_name, FNM_FILE_NAME) == 0)
            {
                wstring file_name;
                multi2uni(dir->d_name, &file_name);
                ptr->push_back(file_name);
            }
        }
        
        closedir(d);
    }
    
#endif
}

bool GetTextFromFile(const wstring & wfilename, wstring * buf)
{
	buf->clear();

	FILE * in = nullptr;
#ifdef _WIN32
	_wfopen_s(&in, wfilename.c_str(), L"r");
#else
    string filename;
    uni2multi(wfilename, &filename);
    in = fopen(filename.c_str(), "r");
#endif
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