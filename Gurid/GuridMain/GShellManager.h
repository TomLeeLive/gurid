#pragma once
class GShellManager
{
public:
	float m_fCoolTime;			//포탄 발사후 쿨타임(다음 발사까지의 시간)
	float m_fFireTime;			//마지막 포탄 발사된 시간
	std::vector<shared_ptr<GShell> >			m_vecShell;	// 포탄.
	bool frame(GCar* car, GTimer* timer, GGuridCamera* camera);
	bool render();
	GShellManager();
	virtual ~GShellManager();
};

