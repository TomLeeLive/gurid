#pragma once
class GGameCredit
{
public:

	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release() { return true; };

	GGameCredit();
	virtual ~GGameCredit();
};

