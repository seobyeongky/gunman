#include "userprofile.h"
#include "util.h"

namespace profile
{
	wstring				name;

	void Set(const userprofile & to)
	{
		name = to.name;
	}


	vector<userprofile>	list;

	bool LoadFile(const wstring & filename, userprofile * ptr)
	{
		if (ptr == nullptr) return false;

		ptr->name = filename;

		FILE * in = nullptr;
		_wfopen_s(&in, filename.c_str(), L"r");
		if (in == nullptr)
			return false;
		
		fclose(in);
		return true;
	}

	bool Load()
	{
		DirChanger			cd(L"data\\system\\profile");
		vector<wstring>		files;

		if (!GetMatchedFileList(&files, L"*"))
			return false;

		userprofile buf;
		for (auto & filename : files)
		{
			if (!LoadFile(filename, &buf))
				continue;

			list.push_back(buf);
		}
		return true;
	}

	bool SaveFile(const userprofile & profile)
	{
		const wstring & filename = profile.name;

		FILE * out = nullptr;
		_wfopen_s(&out, filename.c_str(), L"w");
		if (out == nullptr)
			return false;

		fclose(out);
		return true;
	}

	bool Save()
	{
		DirChanger cd(L"data\\system\\profile");

		for (auto & profile : list)
		{
			if (!SaveFile(profile))
			{
				G.logger->Warning(L"프로필(%s) 저장 실패", profile.name.c_str());
				continue;
			}
		}
		return true;
	}

	bool GetList(vector<userprofile> * ptr)
	{
		*ptr = list;
		return true;
	}

	bool FindOne(const wstring & name, vector<userprofile>::iterator * it = nullptr)
	{
		for (vector<userprofile>::iterator i = list.begin(); i != list.end(); i++)
		{
			if (i->name == name)
			{
				if (it) *it = i;
				return true;
			}
		}
		return false;
	}

	bool MakeNew(const wstring & name)
	{
		if (FindOne(name)) return false;
		userprofile newone;
		newone.name = name;
		list.push_back(newone);
		return true;
	}

	bool RemoveOne(const wstring & name)
	{
		vector<userprofile>::iterator it;
		if (!FindOne(name, &it))
			return false;

		list.erase(it);
		return true;
	}
}