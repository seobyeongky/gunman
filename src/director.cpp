#include "director.h"
#include "asset.h"
#include "global.h"
#include "net_interface.h"

namespace Director
{
	bool			switch_flag = false;
	IScene *		next_scene;

	bool			alt_pressed = false;
	Sprite			cursor;
	Text			fps_text;

	struct timeout_t
	{
		int					ms;
		function<void()>	fn;
		timeout_t(int ms_, function<void()> fn_) : ms(ms_), fn(fn_) {}
	};
	typedef vector<timeout_t> timeouts_t;

	timeouts_t timeouts;

#ifdef _DEBUG
	vector<function<void()>> debug_functions;
#endif

	void SwitchScene(IScene * _next_scene)
	{
		switch_flag = true;
		next_scene = _next_scene;
	}

	void PreHandle(const Event & event)
	{
		if (event.type == Event::Closed)
		{
			G.window.close();
		}
	}

	void PostHandle(const Event & event)
	{
		if (event.type == Event::MouseMoved)
		{
			Vector2f pos(static_cast<float>(Mouse::getPosition(G.window).x), static_cast<float>(Mouse::getPosition(G.window).y));
			cursor.setPosition(pos);
		}
	}

	void Begin(IScene * first_scene)
	{
		IScene *		scene = first_scene;
		client_msg_t	cl_msg;
		Event			event;
		Clock			clock;
		int				last_time = 0;

		SetDefaultCursor();
		G.window.setMouseCursorVisible(false);
		fps_text.setFont(G.default_font);

		while(G.window.isOpen())
		{
			G.present_time = clock.getElapsedTime().asMilliseconds();
			G.delta_time = G.present_time - last_time;
			last_time = G.present_time;

			fps_text.setString(to_string(1000.f / G.delta_time));

			G.sfx_mgr.Update();
			NetInterface::Update();

			G.window.clear(sf::Color::Black);

			while(G.window.pollEvent(event))
			{
				PreHandle(event);

				scene->HandleWindowEvent(event);

				PostHandle(event);

				if (switch_flag)
				{
					switch_flag = false;
					delete scene;
					scene = next_scene;
				}
			}

			for (timeouts_t::iterator i = timeouts.begin(); i != timeouts.end();)
			{
				if (i->ms <= G.present_time)
				{
					(i->fn)();
					i = timeouts.erase(i);
					if (switch_flag)
					{
						switch_flag = false;
						delete scene;
						scene = next_scene;
					}
				}
				else
				{
					i++;
				}
			}
		
			scene->FrameMove();
			if (switch_flag)
			{
				switch_flag = false;
				delete scene;
				scene = next_scene;
			}

#ifdef _DEBUG
			for (auto & f : debug_functions)
			{
				f();
				if (switch_flag)
				{
					switch_flag = false;
					delete scene;
					scene = next_scene;
				}
			}
#endif

			G.window.draw(cursor);
#ifdef _DEBUG
			//G.window.draw(fps_text);
#endif
			G.window.display();

#ifdef _WIN32
			_ASSERTE( _CrtCheckMemory( ) );
#endif
		}
		delete scene;

		G.window.setMouseCursorVisible(true);
	}

	void End()
	{
		G.window.close();
	}

	void SetTimeout(int ms, function<void()> fn)
	{
		timeouts.push_back(timeout_t(ms+G.present_time,fn));
	}

	void SetCursor(const Sprite & cursor_)
	{
		cursor = cursor_;

		const auto & mousepos = Mouse::getPosition(G.window);
		Vector2f pos(static_cast<float>(mousepos.x), static_cast<float>(mousepos.y));
		cursor.setPosition(pos);
	}

	void SetDefaultCursor()
	{
		SetCursor(*G.sprite_map[L"cursor"]);
	}
	
#ifdef _DEBUG
	void PushDebugFunction(function<void()> f)
	{
		debug_functions.push_back(f);
	}
#endif
}