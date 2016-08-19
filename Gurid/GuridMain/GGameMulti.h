#pragma once
class GGameMulti
{
public:

	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release() { return true; };

	GGameMulti();
	virtual ~GGameMulti();
};

