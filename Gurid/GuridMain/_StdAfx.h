#ifndef _STDAFX_H_ //��Ŭ��� ����
#define	 _STDAFX_H_

#define GMAIN			g_pApp
#define GHINST			g_pApp->m_hInstance
#define GHWND			g_pApp->m_hWnd
#define GDEVICE			g_pApp->m_pd3dDevice
#define GCONTEXT		g_pApp->m_pImmediateContext
#define GGAME			g_pGameplay
#define GGAMEMULTI		g_pGameMultiplay

#define G_MACRO_ADD_SCORE 100			//�� ���Ľ� ���� ��
#define G_MACRO_ENEMY_SHELL_DAMAGE 10	//�� źȯ ������ ��
#define G_MACRO_CAR_SPEED 3.0f			//�� Speed
#define G_MACRO_CAR_MAX_SPEED 10.0f		//�� Max Speed
#define G_MACRO_TIRES 6					//Ÿ�̾� ����
#define G_MACRO_TANK_TIRES 6			//��ũ Ÿ�̾� ����
#define G_MACRO_MAX_TANK 4				//�ִ� ��ũ ���� for Debug
#define G_MACRO_CANNON_LENGTH 5.0f		//źȯ �߻�� ��ž �տ��� �߻��ϵ��� ���� ����ϴ� ����

extern int g_iWave;   //Wave Count, ǥ��
extern int g_iHP;	  //���ΰ� ü�� ǥ��
extern int g_iBoost;  //���ΰ� �ν�Ʈ ǥ��
extern int g_iShell;  //���ΰ� ��ź ǥ��

enum SNDRESOURCES {
	SND_HOTWO			= 0,	//0.Hotwo
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

#include "GBasisLib_0.h"
#include "GShape.h"
#include "GBackViewCamera.h"

#include "GSound.h"
#include "GBoundingBoxOBB.hpp"
#include "GGuridCamera.h"
#include "GCar.h"
#include "GCarManager.h"
#include "GShell.h"
#include "GShellManager.h"
#include "GSkyBox.h"
#include "GuridMain.h"
#endif




