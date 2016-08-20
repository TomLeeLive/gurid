#include "_StdAfx.h"



GShellManager::GShellManager()
{

	//m_fFireTime = 0.0f;
	//m_fCoolTime = 0.5f;
}


GShellManager::~GShellManager()
{
}


bool GShellManager::frame(GCar* car, GTimer* timer, GGuridCamera* camera) {

	if (g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_READY && g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER) {

		//ÁÖÀÎ°ø ÅÊÅ© ÃÑ¾Ë ¹ß»ç START
		if (I_Input.KeyCheck(DIK_SPACE) == KEY_HOLD )//&& car->m_bPlayer)
		{
			if (timer->m_fDurationTime - car->m_fFireTime > car->m_fCoolTime) {

				//ÃÑ¾Ë ¹ß»ç ¼Ò¸®
				g_pMain->m_pSound.Play(SND_TANKSHELLFIRE, true);

				m_vecShells.push_back(make_shared<GShell>(car, timer->m_fDurationTime, car->m_bPlayer));
				car->m_fFireTime = timer->m_fDurationTime;
#ifdef _DEBUG
			TCHAR buf[80];
			wsprintf(buf, L"m_vecShells size:%d\n", m_vecShells.size());
			OutputDebugString(buf);
#endif
		}
	}
	//ÁÖÀÎ°ø ÅÊÅ© ÃÑ¾Ë ¹ß»ç END

	//ÀûÅÊÅ© ÃÑ¾Ë ¹ß»ç START


		if (car->m_bPlayer == false) {
			if (timer->m_fDurationTime - car->m_fFireTime > car->m_fCoolTime) {

				////ÃÑ¾Ë ¹ß»ç ¼Ò¸®
				//g_pMain->m_pSound.Play(SND_TANKSHELLHIT, true);

				m_vecShells.push_back(make_shared<GShell>(car, timer->m_fDurationTime, car->m_bPlayer));
				car->m_fFireTime = timer->m_fDurationTime;
#ifdef _DEBUG
				TCHAR buf[80];
				wsprintf(buf, L"m_vecShells size:%d\n", m_vecShells.size());
				OutputDebugString(buf);
#endif
			}
		}
	}
	//ÀûÅÊÅ© ÃÑ¾Ë ¹ß»ç END


	vector<shared_ptr<GShell>>::iterator _F = m_vecShells.begin();
	vector<shared_ptr<GShell>>::iterator _L = m_vecShells.end();
	for (; _F != _L; ++_F)
	{

		//(*_F)->m_matWorld = m_pMainCamera->m_matWorld;
		
		if(g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER){
			//ÃÑ¾Ë ÀÌµ¿
			D3DXVECTOR3 temp = (*_F)->m_vPos;//D3DXVECTOR3((*_F)->m_matWorld._41, (*_F)->m_matWorld._42, (*_F)->m_matWorld._43);
			temp = temp + (*_F)->m_fSpeed*timer->GetSPF()*((*_F)->m_vLook);
			(*_F)->m_vPos = temp;
		}


		D3DXMATRIX temp_mat;

		D3DXMatrixIdentity(&temp_mat);

		temp_mat *= (*_F)->m_mat_s;
		temp_mat *= (*_F)->m_mat_r_x;
		temp_mat *= (*_F)->m_matRotation;
		temp_mat._41 = (*_F)->m_vPos.x; temp_mat._42 = (*_F)->m_vPos.y; temp_mat._43 = (*_F)->m_vPos.z;


		(*_F)->SetMatrix(&temp_mat, &camera->m_matView, &camera->m_matProj);

		(*_F)->BoundingBoxUpdate(temp_mat);
	}



	_F = m_vecShells.begin();
	_L = m_vecShells.end();
	for (; _F != _L; ++_F)
	{
		if (g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER) {
			if (timer->m_fDurationTime - (*_F)->m_fFireTime > (*_F)->m_fEndTime) {
				(*_F).reset();//delete (*_F);
				*_F = 0;
			}
		}
	}

	_F = m_vecShells.begin();
	while (_F != m_vecShells.end())
	{
		if (*_F == 0) {

			_F = m_vecShells.erase(_F);
		}
		else {
			_F++;
		}
	}

	return true;

}
bool GShellManager::render() {

	vector<shared_ptr<GShell>>::iterator _F = m_vecShells.begin();
	vector<shared_ptr<GShell>>::iterator _L = m_vecShells.end();
	for (; _F != _L; ++_F)
	{
		//if (*_F == 0)
		//	continue;

		(*_F)->Render(g_pImmediateContext);
		//printf("name : %s, num : %d \n", (*_F)->m_szStr, (*_F)->m_iNum);
	}
	return true;
}