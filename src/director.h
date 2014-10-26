#pragma once

#include "scene.h"

namespace Director
{
	void SwitchScene(IScene * next_scene);

	void Begin(IScene * first_scene);
	void End();

	void SetTimeout(int ms, function<void()> fn);

	void SetCursor(const Sprite & cursor);
	void SetDefaultCursor();

	void SafeCall(function<void(bool)> fn);

#ifdef _DEBUG
	void PushDebugDrawRect(const FloatRect & rect);
	void PushDebugFunction(function<void()> f);
#endif
}