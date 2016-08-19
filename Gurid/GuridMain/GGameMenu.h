#pragma once
class GGameMenu
{
public:

	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release() { return true; };

	GGameMenu();
	virtual ~GGameMenu();
};

