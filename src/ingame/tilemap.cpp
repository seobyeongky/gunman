#include "tilemap.h"
#include "../global.h"

TileMap g_tilemap;

TileMap::TileMap()
	: _texture()
	, _vertices()
{
}

TileMap::~TileMap()
{
}

void TileMap::LoadFromTmx(const Tmx::Map & tmxmap)
{
	const Tmx::Tileset * tileset = tmxmap.GetTileset(0);
	if (!_texture.loadFromFile(tileset->GetImage()->GetSource()))
	{
		G.logger->Error(L"%s 텍스처 로드 실패", tileset->GetImage()->GetSource().c_str());
		return;
	}
	// fix me ... 어떻게 하는지 모름 방식을 모름
	unsigned int nr_tile_row = static_cast<unsigned int>(
		floorf(static_cast<float>(_texture.getSize().x) / tmxmap.GetTileWidth()));
	unsigned int nr_tile_col = static_cast<unsigned int>(
		floorf(static_cast<float>(_texture.getSize().y) / tmxmap.GetTileHeight()));

	_vertices.resize(4 * tmxmap.GetWidth() * tmxmap.GetHeight());
	_vertices.setPrimitiveType(Quads);

	const Tmx::Layer * layer = tmxmap.GetLayer(0);
	for (int y = 0; y < layer->GetHeight(); ++y)
	{
		for (int x = 0; x < layer->GetWidth(); ++x)
		{
			unsigned int gid = layer->GetTileId(x, y);
			Vertex * v = &_vertices[4 * (x + y * tmxmap.GetWidth())];
			v[0].position.x = static_cast<float>(x) * tmxmap.GetTileWidth();
			v[0].position.y = static_cast<float>(y) * tmxmap.GetTileHeight() - 0.1f;
			v[1].position.x = static_cast<float>(x+1) * tmxmap.GetTileWidth();
			v[1].position.y = static_cast<float>(y) * tmxmap.GetTileHeight() - 0.1f;
			v[2].position.x = static_cast<float>(x+1) * tmxmap.GetTileWidth();
			v[2].position.y = static_cast<float>(y+1) * tmxmap.GetTileHeight() + 0.1f;
			v[3].position.x = static_cast<float>(x) * tmxmap.GetTileWidth();
			v[3].position.y = static_cast<float>(y+1) * tmxmap.GetTileHeight() + 0.1f;
			
			float tx = static_cast<float>((gid % nr_tile_row) * tmxmap.GetTileWidth());
			float ty = static_cast<float>((gid / nr_tile_row) * tmxmap.GetTileHeight());
			v[0].texCoords.x = tx;
			v[0].texCoords.y = ty;
			v[1].texCoords.x = tx + tmxmap.GetTileWidth();
			v[1].texCoords.y = ty;
			v[2].texCoords.x = tx + tmxmap.GetTileWidth();
			v[2].texCoords.y = ty + tmxmap.GetTileHeight();
			v[3].texCoords.x = tx;
			v[3].texCoords.y = ty + tmxmap.GetTileHeight();
		}
	}
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &_texture;

    // draw the vertex array
    target.draw(_vertices, states);
}
