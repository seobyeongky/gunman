/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	tilemap.h

	custom SFML Tilemap class의 header file 입니다.
=======================================================*/

#pragma once

#include "../internal.h"

#include <tmxparser/Tmx.h>

class TileMap : public Drawable, public Transformable
{
/*	tile을 그려줍니다. */
public:
			 TileMap();
	virtual	~TileMap();
	void	 LoadFromTmx(const Tmx::Map & tmxmap);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
    Texture				_texture;
    VertexArray			_vertices;
};

extern TileMap g_tilemap;