#include "_StdAfx.h"



GShellManager::GShellManager()
{

	m_fFireTime = 0.0f;
	m_fCoolTime = 0.5f;
}


GShellManager::~GShellManager()
{
}


bool GShellManager::frame(GCar* car, GTimer* timer, GGuridCamera* camera) {
	if (I_Input.KeyCheck(DIK_SPACE) == KEY_HOLD)
	{
		if (timer->m_fDurationTime - m_fFireTime > m_fCoolTime) {
			m_vecShell.push_back(make_shared<GShell>(car, timer->m_fDurationTime));
			m_fFireTime = timer->m_fDurationTime;
		}
	}



	vector<shared_ptr<GShell>>::iterator _F = m_vecShell.begin();
	vector<shared_ptr<GShell>>::iterator _L = m_vecShell.end();
	for (; _F != _L; ++_F)
	{
		//(*_F)->m_matWorld = m_pMainCamera->m_matWorld;
		D3DXVECTOR3 temp = (*_F)->m_vPos;//D3DXVECTOR3((*_F)->m_matWorld._41, (*_F)->m_matWorld._42, (*_F)->m_matWorld._43);
		temp = temp + (*_F)->m_fSpeed*timer->GetSPF()*((*_F)->m_vLook);
		(*_F)->m_vPos = temp;

		D3DXMATRIX temp_mat;

		D3DXMatrixIdentity(&temp_mat);

		temp_mat *= (*_F)->m_mat_s;
		temp_mat *= (*_F)->m_mat_r_x;
		temp_mat *= (*_F)->m_matRotation;
		temp_mat._41 = (*_F)->m_vPos.x; temp_mat._42 = (*_F)->m_vPos.y; temp_mat._43 = (*_F)->m_vPos.z;


		(*_F)->SetMatrix(&temp_mat, &camera->m_matView, &camera->m_matProj);
		//printf("name : %s, num : %d \n", (*_F)->m_szStr, (*_F)->m_iNum);
	}



	_F = m_vecShell.begin();
	_L = m_vecShell.end();
	for (; _F != _L; ++_F)
	{
		if (timer->m_fDurationTime - (*_F)->m_fFireTime > (*_F)->m_fEndTime) {
			(*_F).reset();//delete (*_F);
			*_F = 0;
		}
	}

	_F = m_vecShell.begin();
	while (_F != m_vecShell.end())
	{
		if (*_F == 0) {

			_F = m_vecShell.erase(_F);
		}
		else {
			_F++;
		}
	}

	return true;

}
bool GShellManager::render() {

	vector<shared_ptr<GShell>>::iterator _F = m_vecShell.begin();
	vector<shared_ptr<GShell>>::iterator _L = m_vecShell.end();
	for (; _F != _L; ++_F)
	{
		(*_F)->Render(g_pImmediateContext);
		//printf("name : %s, num : %d \n", (*_F)->m_szStr, (*_F)->m_iNum);
	}
	return true;
}