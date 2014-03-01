#include "ani00.h"
#include "../global.h"
#include "renderer.h"

Ani00::Ani00()
	: _sprite(*G.sprite_map[L"ani00"])
	, _lifetime(LIFETIME)
	, _renderable_id(0)
{
	_renderable_id  = Renderer::AddRenderable(this);
	_sprite.setOrigin(23.5f,23.5f);
	Update();
}

Ani00::~Ani00()
{
	Renderer::RemoveRenderable(_renderable_id);
}

void Ani00::Consume(int amount)
{
	_lifetime -= amount;
	Update();
}

bool Ani00::IsDead() const
{
	return _lifetime <= 0;
}

void Ani00::draw(RenderTarget& target, RenderStates states) const
{
	if (IsDead()) return;
	states.transform *= getTransform();
	target.draw(_sprite, states);
}

void Ani00::Update()
{
	if (_lifetime <= 0) return;

	int index = NR_FRAMES - 1  - _lifetime * NR_FRAMES / LIFETIME;
	IntRect rect;
	rect.left = index * WIDTH;
	rect.top = 0;
	rect.width = WIDTH;
	rect.height = HEIGHT;
	_sprite.setTextureRect(rect);
}