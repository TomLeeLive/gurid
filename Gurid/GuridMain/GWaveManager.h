#pragma once
class GWaveManager
{
public:
	bool		Init() { return true; };
	bool		Frame();
	bool		Render() { return true; };
	bool		Release() { return true; };

	GWaveManager();
	virtual ~GWaveManager();
};

