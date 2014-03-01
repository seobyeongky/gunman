#pragma once

#include "sv_scene.h"

namespace SvDirector
{
	void SwitchScene(SvScene * next_scene);

	void Begin(SvScene * first_scene);

	void End();

	bool IsWorking();

	void SetTimeout(int ms, function<void()> fn);
}