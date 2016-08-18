#include "_StdAfx.h"



GCarManager::GCarManager()
{
	m_vecCars.push_back(make_shared<GCar>(TANK, true));
	for (int i = 0; i < G_MACRO_MAX_TANK-1; i++) {
		m_vecCars.push_back(make_shared<GCar>(TANK));
	}
}

bool GCarManager::init() {

	vector<shared_ptr<GCar>>::iterator _F = m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_vecCars.end();


	for (; _F != _L; ++_F)
	{
		(*_F)->init(g_pd3dDevice);
	}
	return true;
}

GCarManager::~GCarManager()
{
}


bool GCarManager::frame(GTimer* timer, GGuridCamera* camera) {
	

	vector<shared_ptr<GCar>>::iterator _F = m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_vecCars.end();


	for (; _F != _L; ++_F)
	{
		(*_F)->frame(timer->GetSPF(), camera);
	}
	return true;

}
bool GCarManager::render(GGuridCamera* camera) {

	vector<shared_ptr<GCar>>::iterator _F = m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_vecCars.end();
	for (; _F != _L; ++_F)
	{
		(*_F)->render(g_pImmediateContext, camera);
		//printf("name : %s, num : %d \n", (*_F)->m_szStr, (*_F)->m_iNum);
	}
	return true;
}