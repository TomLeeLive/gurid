#pragma once
class GShellManager
{
public:
	//float m_fCoolTime;			//��ź �߻��� ��Ÿ��(���� �߻������ �ð�)
	//float m_fFireTime;			//������ ��ź �߻�� �ð�
	std::vector<shared_ptr<GShell> >			m_vecShells;	// ��ź.
	bool frame(GCar* car, GTimer* timer, GGuridCamera* camera);
	//bool colcheck(std::vector<shared_ptr<GCar> >& car) {

	//	return true;
	//}
	bool render();
	GShellManager();
	virtual ~GShellManager();
};

