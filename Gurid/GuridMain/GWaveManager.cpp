#include "_StdAfx.h"

void GWaveManager::ResetGameObjectsAndNew() {
	
	g_iWave = 1;




	if(g_pMain->m_GameSingle.m_ShellManager.m_vecShells.size() !=0){
		vector<shared_ptr<GShell>>::iterator _F = g_pMain->m_GameSingle.m_ShellManager.m_vecShells.begin();
		vector<shared_ptr<GShell>>::iterator _L = g_pMain->m_GameSingle.m_ShellManager.m_vecShells.end();

		_F = g_pMain->m_GameSingle.m_ShellManager.m_vecShells.begin();
		_L = g_pMain->m_GameSingle.m_ShellManager.m_vecShells.end();
		for (; _F != _L; ++_F)
		{

			(*_F).reset();//delete (*_F);
			*_F = 0;

		}

		_F = g_pMain->m_GameSingle.m_ShellManager.m_vecShells.begin();
		while (_F != g_pMain->m_GameSingle.m_ShellManager.m_vecShells.end())
		{
			if (*_F == 0) {

				_F = g_pMain->m_GameSingle.m_ShellManager.m_vecShells.erase(_F);
			}
			else {
				_F++;
			}
		}
	}

	if(g_pMain->m_GameSingle.m_TankManager.m_vecCars.size()!=0)
	{
		vector<shared_ptr<GCar>>::iterator _B = g_pMain->m_GameSingle.m_TankManager.m_vecCars.begin();
		vector<shared_ptr<GCar>>::iterator _E = g_pMain->m_GameSingle.m_TankManager.m_vecCars.end();

		_B = g_pMain->m_GameSingle.m_TankManager.m_vecCars.begin();
		_E = g_pMain->m_GameSingle.m_TankManager.m_vecCars.end();
		for (; _B != _E; ++_B)
		{

			if ((*_B)->m_bPlayer != true) {
				(*_B).reset();//delete (*_F);
				*_B = 0;
			}
		}

		_B = g_pMain->m_GameSingle.m_TankManager.m_vecCars.begin();
		while (_B != g_pMain->m_GameSingle.m_TankManager.m_vecCars.end())
		{
			if (*_B == 0) {

				_B = g_pMain->m_GameSingle.m_TankManager.m_vecCars.erase(_B);
			}
			else {
				_B++;
			}
		}
	}
	//g_pMain->m_GameSingle.m_TankManager.m_vecCars.push_back(make_shared<GCar>(TANK, true));
	//srand(time(NULL));
	g_pMain->m_GameSingle.m_TankManager.NewCarPush();

}
void GWaveManager::ResetPlayerStatus() {

		bool		bPlayerStatusReset = false;

		vector<shared_ptr<GCar>>::iterator _F = g_pMain->m_GameSingle.m_TankManager.m_vecCars.begin();
		vector<shared_ptr<GCar>>::iterator _L = g_pMain->m_GameSingle.m_TankManager.m_vecCars.end();


		for (; _F != _L; ++_F)
		{
			if ((*_F)->m_bPlayer == true && bPlayerStatusReset == false) {

				(*_F)->m_iHP = 100;
				(*_F)->m_iBoost = 100;
				(*_F)->m_iShell = 10;

				bPlayerStatusReset = true;
			}

			return;
		}
		return;
}

bool GWaveManager::CheckPlayerDead() {

	bool		bPlayerStatusChecked = false;

	vector<shared_ptr<GCar>>::iterator _F = g_pMain->m_GameSingle.m_TankManager.m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = g_pMain->m_GameSingle.m_TankManager.m_vecCars.end();


	for (; _F != _L; ++_F)
	{
		if ((*_F)->m_bPlayer == true && bPlayerStatusChecked == false) {

			if ((*_F)->m_iHP < 0)
				return true;

			bPlayerStatusChecked = true;
		}

		return false;
	}
	return false;
}

bool GWaveManager::Frame(){

	if (WAVE_ST_NEWGAME == g_pMain->m_GameSingle.m_nWavePhase) {
		if (g_pMain->m_GameSingle.m_nWaveBeforePhase != g_pMain->m_GameSingle.m_nWavePhase) {

			m_fReadyStartTime = g_pMain->m_Timer.m_fDurationTime;

			ResetGameObjectsAndNew();

			g_pMain->m_GameSingle.m_nWaveBeforePhase = g_pMain->m_GameSingle.m_nWavePhase;
		}

		if (g_pMain->m_Timer.m_fDurationTime - m_fReadyStartTime > m_fReadyTime) {
			g_pMain->m_GameSingle.m_nWaveBeforePhase = WAVE_ST_NEWGAME;
			g_pMain->m_GameSingle.m_nWavePhase = WAVE_ST_READY;
		}
	
	}
	if (WAVE_ST_READY == g_pMain->m_GameSingle.m_nWavePhase) {
		if (g_pMain->m_GameSingle.m_nWaveBeforePhase != g_pMain->m_GameSingle.m_nWavePhase) {
			
			m_fReadyStartTime = g_pMain->m_Timer.m_fDurationTime;

			//주인공 탱크: 부스트, 탄환,HP 만땅
			ResetPlayerStatus();

			//사운드 출력
			g_pMain->m_pSound.Play(SND_GETREADY, true, true);

			if(g_iWave !=1){
				//사운드 출력
				g_pMain->m_pSound.Play(SND_WAVECLEAR, true, true);


			}




			g_pMain->m_GameSingle.m_nWaveBeforePhase = g_pMain->m_GameSingle.m_nWavePhase;
		}

		if (g_pMain->m_Timer.m_fDurationTime - m_fReadyStartTime > m_fReadyTime) {
			g_pMain->m_GameSingle.m_nWaveBeforePhase = WAVE_ST_READY;
			g_pMain->m_GameSingle.m_nWavePhase = WAVE_ST_PLAY;
		}
	}
	if (WAVE_ST_PLAY == g_pMain->m_GameSingle.m_nWavePhase) {
		
		if (g_pMain->m_GameSingle.m_nWaveBeforePhase != g_pMain->m_GameSingle.m_nWavePhase) {
			//사운드 출력
			g_pMain->m_pSound.Play(SND_WAVESTART, true, true);
		
			g_pMain->m_GameSingle.m_nWaveBeforePhase = g_pMain->m_GameSingle.m_nWavePhase;
		}

		if (g_pMain->m_GameSingle.m_TankManager.m_vecCars.size()==1) {
			g_iWave+=1;
			
			//적탱크 생성
			//srand(time(NULL));
			g_pMain->m_GameSingle.m_TankManager.NewCarPush();

			g_pMain->m_GameSingle.m_nWaveBeforePhase = WAVE_ST_PLAY;
			g_pMain->m_GameSingle.m_nWavePhase = WAVE_ST_READY;
		}
		
		if(CheckPlayerDead()) {
			g_pMain->m_GameSingle.m_nWaveBeforePhase = WAVE_ST_PLAY;
			g_pMain->m_GameSingle.m_nWavePhase = WAVE_ST_GAMEOVER;
		}
	}
	if (WAVE_ST_GAMEOVER == g_pMain->m_GameSingle.m_nWavePhase) {

		if (g_pMain->m_GameSingle.m_nWaveBeforePhase != g_pMain->m_GameSingle.m_nWavePhase) {
			//사운드 출력
			g_pMain->m_pSound.Play(SND_GAMEOVER, true, true);

			g_pMain->m_GameSingle.m_nWaveBeforePhase = g_pMain->m_GameSingle.m_nWavePhase;
		}

		if (I_Input.KeyCheck(DIK_RETURN) == KEY_HOLD)
		{
			g_pMain->m_GameSingle.m_nWaveBeforePhase = WAVE_ST_GAMEOVER;
			g_pMain->m_GameSingle.m_nWavePhase = WAVE_ST_NEWGAME;

			g_pMain->m_nGamePhase = ST_START;
			g_pMain->m_nGameBeforePhase = ST_SINGLEGAME;
		}

	}
	if (WAVE_ST_TO_MENU == g_pMain->m_GameSingle.m_nWavePhase) {

		g_pMain->m_GameSingle.m_nWaveBeforePhase = WAVE_ST_TO_MENU;

	}

	//g_iWave = 1;	//Wave Count, 표시
	//g_iHP = 100;	//주인공 체력 표시
	//g_iBoost = 100; //주인공 부스트 표시
	//g_iShell = 10;  //주인공 잔탄 표시


	return true;
};

GWaveManager::GWaveManager()
{
	m_fReadyStartTime = 0.0f;
	m_fReadyTime = G_MACRO_READY_WAIT_TIME;
}


GWaveManager::~GWaveManager()
{
}
