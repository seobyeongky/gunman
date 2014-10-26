#pragma once

#include "../internal.h"

#include <tmxparser/Tmx.h>

Vector2i GetPosFromLocation(const Tmx::Object & loc, const Vector2i & size, int index);