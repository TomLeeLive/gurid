#pragma once
class GShellManager
{
public:
	float m_fCoolTime;			//��ź �߻��� ��Ÿ��(���� �߻������ �ð�)
	float m_fFireTime;			//������ ��ź �߻�� �ð�
	std::vector<shared_ptr<GShell> >			m_vecShell;	// ��ź.
	bool frame(GCar* car, GTimer* timer, GGuridCamera* camera);
	bool render();
	GShellManager();
	virtual ~GShellManager();
};

