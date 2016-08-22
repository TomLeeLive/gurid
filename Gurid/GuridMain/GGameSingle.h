#pragma once
#include "GSprite.h"
#include "GSpriteMgr.h"
class GGameSingle
{
public:

	bool		Init() { return true; };
	bool		Frame();
	bool		Render();
	bool		Release() { return true; };

	bool		RenderScene(GSprite* pPlane);

	GGameSingle();
	virtual ~GGameSingle();
};

