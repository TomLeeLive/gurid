#ifndef _STDAFX_H_ //��Ŭ��� ����
#define	 _STDAFX_H_

#define GMAIN			g_pMain
#define GHINST			g_pMain->m_hInstance
#define GHWND			g_pMain->m_hWnd
#define GDEVICE			g_pMain->m_pd3dDevice
#define GCONTEXT		g_pMain->m_pImmediateContext
//#define GGAME			g_pGameplay
//#define GGAMEMULTI		g_pGameMultiplay

#define G_MACRO_ADD_SCORE 100			//�� ���Ľ� ���� ��
#define G_MACRO_ENEMY_SHELL_DAMAGE 10	//�� źȯ ������ ��
#define G_MACRO_CAR_SPEED 10.0f			//�� Speed
#define G_MACRO_CAR_MAX_SPEED 20.0f		//�� Max Speed
#define G_MACRO_TIRES 6					//Ÿ�̾� ����
#define G_MACRO_TANK_TIRES 6			//��ũ Ÿ�̾� ����
#define G_MACRO_MAX_TANK 2				//��ũ �ּ� ����. ù�� ��ũ ����= G_MACRO_MAX_TANK + Wave count
#define G_MACRO_CANNON_LENGTH 5.0f		//źȯ �߻�� ��ž �տ��� �߻��ϵ��� ���� ����ϴ� ����
#define G_MACRO_READY_WAIT_TIME 3.0f	//Ready�� ��� �ð�

extern int g_iWave;   //Wave Count, ǥ��
extern int g_iHP;	  //���ΰ� ü�� ǥ��
extern int g_iBoost;  //���ΰ� �ν�Ʈ ǥ��
extern int g_iShell;  //���ΰ� ��ź ǥ��

enum SNDRESOURCES {
	SND_HOWTO			= 0,	//0.Hotwo
	SND_INTRO			= 1,	//1.intro.wav
	SND_GETREADY		= 2,	//2.Get ready!
	SND_WAVECLEAR		= 3,	//3.Wave Clear!
	SND_WAVESTART		= 4,	//4.Wave Start!
	SND_GAMEOVER		= 5,	//5.Game Over!
	SND_TANKMOVE		= 6,	//1.��ũ����Ҹ�
	SMD_TANKFASTMOVE	= 7,	//2.��ũ��������Ҹ�
	SND_TANKHEADROT		= 8,	//3.��ũ�Ӹ�ȸ���Ҹ�
	SND_TANKSHELLHIT	= 9,	//4.��ź�´¼Ҹ�
	SND_TANKSHELLPASSBY = 10,	//5.��ź�������¼Ҹ�
	SND_TANKSHELLFIRE	= 11,	//7.��ź�߻�Ҹ�
	SND_TANKEXPLSION	= 12,	//8.��ũ�����¼Ҹ�
	SND_RESOURCES_LAST	= 13
};

enum WAVE_STATE {
	WAVE_ST_NEWGAME = 0,
	//WAVE_ST_NEWENEMY_ADD,
	WAVE_ST_READY,
	WAVE_ST_PLAY,
	WAVE_ST_GAMEOVER,
	WAVE_ST_TO_MENU
};

enum GAME_STATE {
	ST_START = 0,
	ST_SINGLEGAME,
	ST_MULTI,
	ST_END,
	ST_HOWTO,
	ST_CREDIT = 5
};

#include "GBasisLib_0.h"
#include "GShape.h"
#include "GBackViewCamera.h"


#include "GPlaneRect.h"				//for UI
#include "GImeUI.h"					//for UI
#pragma comment( lib, "IMM32.lib" ) //for UI


#include "GWaveManager.h"
#include "GSound.h"
#include "GBoundingBoxOBB.hpp"
#include "GGuridCamera.h"
#include "GCar.h"
#include "GCarManager.h"
#include "GShell.h"
#include "GShellManager.h"
#include "GSkyBox.h"

#include "GGameEnd.h"

#include "GGameMulti.h"
#include "GGameMenu.h"
#include "GGameHowto.h"
#include "GGameCredit.h"
#include "GGameSingle.h"
#include "GuridMain.h"

//#include "GSprite.h"
#endif




