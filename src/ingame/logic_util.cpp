#include "logic_util.h"

Vector2i GetPosFromLocation(const Tmx::Object & loc, const Vector2i & size, int index)
{
	int x = loc.GetX()+loc.GetWidth()/2;
	int y = loc.GetY()+loc.GetHeight()/2;
	
	return Vector2i();
}