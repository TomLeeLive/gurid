#include "_StdAfx.h"



GCarManager::GCarManager()
{
	srand(time(NULL));

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
	
	D3DXVECTOR3 vPlayerPos;
	bool		bNeedForPlayerStatus = true;

	vector<shared_ptr<GCar>>::iterator _F = m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_vecCars.end();


	for (; _F != _L; ++_F)
	{
		if(bNeedForPlayerStatus){
			vPlayerPos = D3DXVECTOR3((*_F)->m_matWorld_body._41, (*_F)->m_matWorld_body._42, (*_F)->m_matWorld_body._43);

			
			g_iHP = (*_F)->m_iHP;
			g_iBoost = (*_F)->m_iBoost;
			g_iShell = (*_F)->m_iShell;

			bNeedForPlayerStatus = false;
		}

		(*_F)->frame(timer->GetSPF(), camera, vPlayerPos);
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