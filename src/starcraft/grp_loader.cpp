#include "grp_loader.h"
#include "../global.h"
#include "../colors.h"

#include <grpapi.h>
#include <sstream>
#include <thread>

namespace grp
{
#define RGBRESERVE {0,0,0,0xFF}

void GetColorFromPalette(int index, Color * ptr, const Color & teamcolor)
{
	if (index < 0 || index >= 256) return;

	static const int teamcolor_opacity[8] = {255, 222, 189, 156, 124, 91, 58, 30};

	static const RGBQUAD default_palette[256] =
	{
		{0,0,0}      , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		{24,36,44}   , {72,36,20}   , {92,44,20}   , {112,48,20}  , {104,60,36}  , {124,64,24}  , {120,76,44}  , {168,8,8}    ,
		{140,84,48}  , {132,96,68}  , {160,84,28}  , {196,76,24}  , {188,104,36} , {180,112,60} , {208,100,32} , {220,148,52} ,
		{224,148,84} , {236,196,84} , {52,68,40}   , {64,108,60}  , {72,108,80}  , {76,128,80}  , {80,140,92}  , {92,160,120} ,
		{0,0,24}     , {0,16,52}    , {0,8,80}     , {36,52,72}   , {48,64,84}   , {20,52,124}  , {52,76,108}  , {64,88,116}  ,
		{72,104,140} , {0,112,156}  , {88,128,164} , {64,104,212} , {24,172,184} , {36,36,252}  , {100,148,188}, {112,168,204},
		{140,192,216}, {148,220,244}, {172,220,232}, {172,252,252}, {204,248,248}, {252,252,0}  , {244,228,144}, {252,252,192},
		{12,12,12}   , {24,20,16}   , {28,28,32}   , {40,40,48}   , {56,48,36}   , {56,60,68}   , {76,64,48}   , {76,76,76}   ,
		{92,80,64}   , {88,88,88}   , {104,104,104}, {120,132,108}, {104,148,108}, {116,164,124}, {152,148,140}, {144,184,148},
		{152,196,168}, {176,176,176}, {172,204,176}, {196,192,188}, {204,224,208}, {240,240,240}, {28,16,8}    , {40,24,12}   ,
		{52,16,8}    , {52,32,12}   , {56,16,32}   , {52,40,32}   , {68,52,8}    , {72,48,24}   , {96,0,0}     , {84,40,32}   ,
		{80,64,20}   , {92,84,20}   , {132,4,4}    , {104,76,52}  , {124,56,48}  , {112,100,32} , {124,80,80}  , {164,52,28}  ,
		{148,108,0}  , {152,92,64}  , {140,128,52} , {152,116,84} , {184,84,68}  , {176,144,24} , {176,116,92} , {244,4,4}    ,
		{200,120,84} , {252,104,84} , {224,164,132}, {252,148,104}, {252,204,44} , {16,252,24}  , {12,0,32}    , {28,28,44}   ,
		{36,36,76}   , {40,44,104}  , {44,48,132}  , {32,24,184}  , {52,60,172}  , {104,104,148}, {100,144,252}, {124,172,252},
		{0,228,252}  , {156,144,64} , {168,148,84} , {188,164,92} , {204,184,96} , {232,216,128}, {236,196,176}, {252,252,56} ,
		{252,252,124}, {252,252,164}, {8,8,8}      , {16,16,16}   , {24,24,24}   , {40,40,40}   , {52,52,52}   , {76,60,56}   ,
		{68,68,68}   , {72,72,88}   , {88,88,104}  , {116,104,56} , {120,100,92} , {96,96,124}  , {132,116,116}, {132,132,156},
		{172,140,124}, {172,152,148}, {144,144,184}, {184,184,232}, {248,140,20} , {16,84,60}   , {32,144,112} , {44,180,148} ,
		{4,32,100}   , {72,28,80}   , {8,52,152}   , {104,48,120} , {136,64,156} , {12,72,204}  , {188,184,52} , {220,220,60} ,
		{16,0,0}     , {36,0,0}     , {52,0,0}     , {72,0,0}     , {96,24,4}    , {140,40,8}   , {200,24,24}  , {224,44,44}  ,
		{232,32,32}  , {232,80,20}  , {252,32,32}  , {232,120,36} , {248,172,60} , {0,20,0}     , {0,40,0}     , {0,68,0}     ,
		{0,100,0}    , {8,128,8}    , {36,152,36}  , {60,156,60}  , {88,176,88}  , {104,184,104}, {128,196,128}, {148,212,148},
		{12,20,36}   , {36,60,100}  , {48,80,132}  , {56,92,148}  , {72,116,180} , {84,132,196} , {96,148,212} , {120,180,236},
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   ,
		RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , RGBRESERVE   , {255,255,255}
	};

	if (index == 0)
	{
		// Transparent Color
		ptr->r = ptr->g = ptr->b = 0;
		ptr->a = 0;
	}
	else if (8 <= index && index < 15)
	{
		// Team color
		int opacity = teamcolor_opacity[index - 8];
		ptr->r = teamcolor.r*opacity/255;
		ptr->g = teamcolor.g*opacity/255;
		ptr->b = teamcolor.b*opacity/255;
		ptr->a = 255;
	}
	else if (index == 192)
	{
		// Protoss shield color(darker)
		ptr->r = ptr->g = ptr->b = 255;
		ptr->a = 200;
	}
	else if (index == 192)
	{
		// Protoss shield color
		ptr->r = ptr->g = ptr->b = 255;
		ptr->a = 220;
	}
	else
	{
		const RGBQUAD & color = default_palette[index];
		ptr->r = color.rgbRed;
		ptr->g = color.rgbGreen;
		ptr->b = color.rgbBlue;
		ptr->a = 255;
	}
}

IntRect GetRectOf(const Vector2i & size, int index)
{
	int nr_col = MAX_TEXTURE_WIDTH / size.x;
	int nr_row = MAX_TEXTURE_HEIGHT / size.y;

	if (nr_col * nr_row <= index)
	{
		G.logger->Error(L"최대 텍스쳐 크기가 모자랍니다. index:%d, size:(%d,%d)", index, size.x, size.y);
	}

	return IntRect((index%nr_col)*size.x,(index/nr_row)*size.y, size.x, size.y);
}

smap<wstring, unit_t*> unitmap;


struct context_t
{
	Image	image; // Frame Image
	int		i; // player index
} context;

void Fill(Image * img, const Color & fillcolor)
{
	auto size = img->getSize();
	for (unsigned int y = 0; y < size.y; y++)
		for (unsigned int x = 0; x < size.x; x++)
			img->setPixel(x, y, fillcolor);
}

void AddUnit(const wstring & unitname, const wstring & wgrppath, const wstring & winfopath, int nr_player)
{
	string grppath;
	uni2multi(wgrppath, &grppath);
	HANDLE handle = LoadGrp(grppath.c_str());
	if (FAILED(handle))
	{
		G.logger->Warning(L"LoadUnitGrp : Failed to open %s", grppath.c_str());
		return;
	}
	
	unit_t * unit = new unit_t();

	GRPHEADER header;
	GetGrpInfo(handle, &header);
	unit->frame_size = Vector2i(header.wMaxWidth, header.wMaxHeight);

	context.image.create(header.wMaxWidth, header.wMaxHeight);

	int nr_col = MAX_TEXTURE_WIDTH / header.wMaxWidth;
	int nr_row = MAX_TEXTURE_HEIGHT / header.wMaxHeight;
	if (nr_col * nr_row <= header.nFrames)
	{
		G.logger->Error(L"최대 텍스쳐 크기가 모자랍니다. nFrames:%d, size:(%d,%d)", header.nFrames, header.wMaxWidth, header.wMaxHeight);
	}
	nr_row = (header.nFrames-1) / nr_col + 1;

	for (int i = 0; i < nr_player; i++)
	{
		// 플레이어마다 색깔이 다르니까 플레이어 수만큼 랜더링한다.
		context.i = i;

		SetFunctionSetPixel([](HDC hDC, int X, int Y, COLORREF clrColor)
		{
			Color c;
			GetColorFromPalette(clrColor, &c, colors::GetPlayerColor(context.i));
			context.image.setPixel(X, Y, c);
		});

		Image img;
		img.create(header.wMaxWidth*nr_col, header.wMaxHeight*nr_row, Color::Transparent);

		for (int j = 0; j < header.nFrames; j++)
		{
			// 프레임마다 랜더링한다.
			DWORD left, top, width, height;
			GetGrpFrameInfo(handle, j, &left, &top, &width, &height);
	
			Fill(&context.image, Color::Transparent);

			DrawGrp(handle
				, reinterpret_cast<HDC>(1) // dummy
				, 0
				, 0
				, j
				, nullptr
				, USE_INDEX
				, 0x404040);

			// context.image가 적절하게 채워져있을 것이다.
			auto rect = GetRectOf(Vector2i(header.wMaxWidth,header.wMaxHeight),j);
			img.copy(context.image, rect.left, rect.top, IntRect(0, 0, 0, 0), true);
		}

		unit->textures[i].loadFromImage(img);
	}

	if (unitmap.find(unitname))
	{
		// Clear호출하지않았을 가능성이 농후함
		G.logger->Error(L"grpunit map에서 같은 이름(%s) 발견", unitname.c_str());
	}

	{
		FILE * in = nullptr;
		_wfopen_s(&in, winfopath.c_str(), L"r");
		if (in == nullptr)
		{
			G.logger->Error(L"grpunit info 파일(%s) 열기 실패", winfopath.c_str());
			return;
		}
		wchar_t buf[256];
		wstring framename;
		while (fgetws(buf, 256, in))
		{
			if (buf[wcslen(buf)-1] == L'\n') buf[wcslen(buf)-1]=L'\0';
			if (wcslen(buf) == 0) continue;
			if (isdigit(buf[0]))
			{
				int d = _wtoi(buf);
				unit->frame_map[framename].push_back(d);
			}
			else
			{
				framename = buf;
			}
		}
		fclose(in);
	}

	unitmap[unitname] = unit;
}


/*void AddUnitList_th(vector<unitloadinfo_t> arr, int nr_player, function<void(bool)> next)
{
}


void AddUnitList(vector<unitloadinfo_t> arr, int nr_player, function<void(bool)> next)
{
	thread t(&AddUnitList_th, arr, nr_player, next);
	t.detach();
}*/

void Clear()
{
	for (auto & unit : unitmap)
	{
		delete unit.element();
	}
	unitmap.clear();
}

}