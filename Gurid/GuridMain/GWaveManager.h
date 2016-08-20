#pragma once

class GWaveManager
{
public:
	float		m_fReadyStartTime;
	float		m_fReadyTime;

	void		ResetGameObjectsAndNew();
	void		ResetPlayerStatus();
	bool		CheckPlayerDead();

	bool		Init() { return true; };
	bool		Frame();
	bool		Render() { return true; };
	bool		Release() { return true; };

	GWaveManager();
	virtual ~GWaveManager();
};

