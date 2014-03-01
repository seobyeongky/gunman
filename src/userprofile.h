#pragma once

#include "internal.h"

struct userprofile
{
	wstring	name;
};

namespace profile
{
	extern wstring name;
	void Set(const userprofile & to);

	bool Load();
	bool Save();
	bool GetList(vector<userprofile> * ptr);
	bool MakeNew(const wstring & name);
	bool RemoveOne(const wstring & name);
}