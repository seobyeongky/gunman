#include "renderer.h"
#include "../global.h"
#include "tilemap.h"

namespace Renderer
{
	vector<IRenderable*>	sorted_renderables;
	smap<int, IRenderable*>	renderables;
	
	View					view;
	Transform				contextcam;

	void Init()
	{
		view = G.window.getDefaultView();
		contextcam = Transform::Identity;
	}

	void Uninit()
	{
		sorted_renderables.clear();
		renderables.clear();
	}

	int AddRenderable(IRenderable * renderable)
	{
		for (int i = 0; i < 100000; i++)
		{
			if (renderables.find(i) == false)
			{
				renderables[i] = renderable;
				return i;
			}
		}
		G.logger->Error(L"PlayScene : 예외상황");
		return -1;
	}

	void RemoveRenderable(int id)
	{
		renderables.erase(id);
	}

	void Render()
	{
		G.window.setView(view);
		{
			G.window.draw(g_tilemap);

			sorted_renderables.clear();
			for (auto it : renderables)
				sorted_renderables.push_back(it.element());
			sort(sorted_renderables.begin(), sorted_renderables.end(), [](const IRenderable * lhs, const IRenderable * rhs){
				if (lhs->GetPriority() > rhs->GetPriority())
					return true;			
				else if (lhs->GetPriority() < rhs->GetPriority())
					return false;
				else
					return lhs->getPosition().y < rhs->getPosition().y;
			});
			for (auto it : sorted_renderables)
				G.window.draw(*it);
		}
		G.window.setView(G.window.getDefaultView());
	}

	void Move(float dx, float dy)
	{
		view.move(dx, dy);
		contextcam.translate(dx, dy);
	}

	Vector2f TransformPoint(const Vector2f & p)
	{
		return contextcam.transformPoint(p);
	}
};