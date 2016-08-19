#pragma once
class GGameSingle
{
public:

	bool		Init() { return true; };
	bool		Frame();
	bool		Render();
	bool		Release() { return true; };

	GGameSingle();
	virtual ~GGameSingle();
};

