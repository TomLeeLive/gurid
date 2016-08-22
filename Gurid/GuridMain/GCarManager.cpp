#include "_StdAfx.h"


void GCarManager::NewCarPush() {
	srand(time(NULL));
	for (int i = 0; i < G_MACRO_MAX_TANK + g_iWave; i++) {
		m_vecCars.push_back(make_shared<GCar>(TANK));
	}
	this->init();

}
GCarManager::GCarManager()
{
	m_vecCars.push_back(make_shared<GCar>(TANK, true));
	//
	//NewCarPush();

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
		if((*_F)->m_bPlayer == true && bNeedForPlayerStatus){
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
	if (m_vecCars.size() == 0)
		return false;

	vector<shared_ptr<GCar>>::iterator _F = m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_vecCars.end();
	for (; _F != _L; ++_F)
	{
		/*if (*_F == 0){
			(*_F).reset();
			continue;
		}*/

		(*_F)->render(g_pImmediateContext, camera);
		//printf("name : %s, num : %d \n", (*_F)->m_szStr, (*_F)->m_iNum);
	}
	return true;
}