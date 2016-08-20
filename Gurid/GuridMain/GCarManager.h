#pragma once



class GCarManager
{
public:
	float m_fCoolTime;			//포탄 발사후 쿨타임(다음 발사까지의 시간)
	float m_fFireTime;			//마지막 포탄 발사된 시간
	std::vector<shared_ptr<GCar> >			m_vecCars;	// 포탄.
	bool init();
	bool frame(GTimer* timer, GGuridCamera* camera);
	bool render(GGuridCamera* camera);

	void GCarManager::NewCarPush();
	GCarManager();
	virtual ~GCarManager();
};

