#pragma once

#include "widget.h"
#include "item.h"

#include <vector>
#include <string>
#include <functional>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;
class Menu : public IWidget
{
public:
	struct delta_t
	{
		Vector2f	delta_pos;
		float		delta_rotation;
		float		delta_scale;
		delta_t(void);
		delta_t(const Vector2f & dpos, float drotation, float dscale);
	};

public:
			 Menu();
			 Menu(const delta_t & delta, Keyboard::Key done_key);
	virtual	~Menu();

	void	 SetDelta(const delta_t & delta);

	void	 PushItem(IItem * item);
	void	 Clear();

	bool	 HandleWindowEvent(const Event & e);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	delta_t			_delta;
	vector<IItem *>	_arr;
	size_t			_index;
	View			_view;
	Keyboard::Key	_done_key;
	
	bool	HandleKeyPressed(Event::KeyEvent key);
	bool	HandleMouseMoved(const Event::MouseMoveEvent & e);
	bool	HandleMouseButtonPressed(const Event::MouseButtonEvent & e);

	void	UpdateItems();
};