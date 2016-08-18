#pragma once

#define G_MACRO_MAX_TANK 2

class GCarManager
{
public:
	float m_fCoolTime;			//��ź �߻��� ��Ÿ��(���� �߻������ �ð�)
	float m_fFireTime;			//������ ��ź �߻�� �ð�
	std::vector<shared_ptr<GCar> >			m_vecCars;	// ��ź.
	bool init();
	bool frame(GTimer* timer, GGuridCamera* camera);
	bool render(GGuridCamera* camera);
	GCarManager();
	virtual ~GCarManager();
};

