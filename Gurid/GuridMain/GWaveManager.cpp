#include "_StdAfx.h"


bool GWaveManager::Frame(){


	if (WAVE_ST_NEWENEMY_ADD == g_pMain->m_nWavePhase) {
		//ToDo. ���߰� �Ѵ� waveī��Ʈ+2 //ù���� �� ��ũ 3��

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

	//g_iWave = 1;	//Wave Count, ǥ��
	//g_iHP = 100;	//���ΰ� ü�� ǥ��
	//g_iBoost = 100; //���ΰ� �ν�Ʈ ǥ��
	//g_iShell = 10;  //���ΰ� ��ź ǥ��


	return true;
};

GWaveManager::GWaveManager()
{
}


GWaveManager::~GWaveManager()
{
}
