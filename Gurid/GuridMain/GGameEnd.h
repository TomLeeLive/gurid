#pragma once
class GGameEnd
{
public:

	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release() { return true; };

	GGameEnd();
	virtual ~GGameEnd();
};

