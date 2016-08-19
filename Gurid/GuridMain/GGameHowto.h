#pragma once
class GGameHowto
{
public:

	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release() { return true; };

	GGameHowto();
	virtual ~GGameHowto();
};

