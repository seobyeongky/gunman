#pragma once

#include "../internal.h"

namespace grp
{
	enum
	{
		MAX_TEXTURE_WIDTH = 2048,
		MAX_TEXTURE_HEIGHT = 2048
	};
	
	IntRect GetRectOf(const Vector2i & size, int index);

	struct unit_t
	{
		Texture						textures[10];
		Vector2i					frame_size;
		smap<wstring, vector<int>>	frame_map;
	};

	extern smap<wstring, unit_t*> unitmap;

	void AddUnit(const wstring & unitname, const wstring & wgrppath, const wstring & winfopath, int nr_player);
	
/*	struct unitloadinfo_t
	{
		wstring unitname;
		wstring grpapth;
		wstring infopath;
	};
	void AddUnitList(vector<unitloadinfo_t> arr, int nr_player, function<void(bool)> next);
*/	
	void Clear();
}