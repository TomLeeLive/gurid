#include "_StdAfx.h"


bool GWaveManager::Frame(){


	if (WAVE_ST_NEWENEMY_ADD == g_pMain->m_nWavePhase) {
		//ToDo. 적추가 한다 wave카운트+2 //첫판은 적 탱크 3대

		g_pMain->m_nWaveBeforePhase = WAVE_ST_NEWENEMY_ADD;
		g_pMain->m_nWavePhase = WAVE_ST_READY;
	}
	
	if (WAVE_ST_READY == g_pMain->m_nWavePhase) {
		
		g_pMain->m_nWaveBeforePhase = WAVE_ST_READY;
		g_pMain->m_nWavePhase = WAVE_ST_PLAY;
	}
	if (WAVE_ST_PLAY == g_pMain->m_nWavePhase) {
		


		g_pMain->m_nWaveBeforePhase = WAVE_ST_PLAY;
	}
	if (WAVE_ST_GAMEOVER == g_pMain->m_nWavePhase) {

		g_pMain->m_nWaveBeforePhase = WAVE_ST_GAMEOVER;
		g_pMain->m_nWavePhase = WAVE_ST_TO_MENU;
	}
	if (WAVE_ST_TO_MENU == g_pMain->m_nWavePhase) {

		g_pMain->m_nWaveBeforePhase = WAVE_ST_TO_MENU;

	}

	//g_iWave = 1;	//Wave Count, 표시
	//g_iHP = 100;	//주인공 체력 표시
	//g_iBoost = 100; //주인공 부스트 표시
	//g_iShell = 10;  //주인공 잔탄 표시


	return true;
};

GWaveManager::GWaveManager()
{
}


GWaveManager::~GWaveManager()
{
}
