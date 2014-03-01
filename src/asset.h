/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	asset.h
=======================================================*/

#pragma once

#include "internal.h"

//	file system에 있는 자원들을 load합니다. (font, image, sound)

bool LoadSystemAssets();
bool LoadGameAssets();
void ReleaseAssets();