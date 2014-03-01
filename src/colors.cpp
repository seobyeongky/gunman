#include "colors.h"

namespace colors
{
	Color color_table[10] = {
		Color(168,8,8), Color(8, 52, 152), Color(32, 144, 112),
		Color(250, 250, 124), Color(0, 127, 127), Color(200, 162, 144),
		Color(64, 64, 64), Color(213, 197, 6), Color(230, 230, 230), Color(20, 20, 20)};

	const Color & GetPlayerColor(int i)
	{
		return color_table[i%10];
	}
}