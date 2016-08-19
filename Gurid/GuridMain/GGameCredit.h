#pragma once
class GGameCredit : public GGameMenu
{
public:
	void		KeyCheck();
	bool		Init();
	//bool		Frame() { return true; };
	//bool		Render() { return true; };
	//bool		Release() { return true; };
	GGameCredit();
	virtual ~GGameCredit();
};

