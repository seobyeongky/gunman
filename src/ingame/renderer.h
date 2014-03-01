#pragma once

#include "renderable.h"

namespace Renderer
{
	void		Init();
	void		Uninit();

	int			AddRenderable(IRenderable * renderable);
	void		RemoveRenderable(int id);

	void		Render();

	void		Move(float dx, float dy);
	Vector2f	TransformPoint(const Vector2f & p);
};