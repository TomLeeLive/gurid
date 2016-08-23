#ifndef _STDAFX_H_ //인클루드 가드
#define	 _STDAFX_H_

#define GMAIN			g_pMain
#define GHINST			g_pMain->m_hInstance
#define GHWND			g_pMain->m_hWnd
#define GDEVICE			g_pMain->m_pd3dDevice
#define GCONTEXT		g_pMain->m_pImmediateContext
//#define GGAME			g_pGameplay
//#define GGAMEMULTI		g_pGameMultiplay

#define G_MACRO_ADD_SCORE 100			//적 폭파시 점수 값
#define G_MACRO_ENEMY_SHELL_DAMAGE 10	//적 탄환 데미지 값
#define G_MACRO_CAR_SPEED 10.0f			//차 Speed
#define G_MACRO_CAR_MAX_SPEED 20.0f		//차 Max Speed
#define G_MACRO_TIRES 6					//타이어 갯수
#define G_MACRO_TANK_TIRES 6			//탱크 타이어 갯수
#define G_MACRO_MAX_TANK 2				//탱크 최소 갯수. 첫판 탱크 갯수= G_MACRO_MAX_TANK + Wave count
#define G_MACRO_CANNON_LENGTH 5.0f		//탄환 발사시 포탑 앞에서 발사하도록 계산시 사용하는 길이
#define G_MACRO_READY_WAIT_TIME 3.0f	//Ready시 대기 시간

extern int g_iWave;   //Wave Count, 표시
extern int g_iHP;	  //주인공 체력 표시
extern int g_iBoost;  //주인공 부스트 표시
extern int g_iShell;  //주인공 잔탄 표시

enum SNDRESOURCES {
	SND_HOWTO			= 0,	//0.Hotwo
	SND_INTRO			= 1,	//1.intro.wav
	SND_GETREADY		= 2,	//2.Get ready!
	SND_WAVECLEAR		= 3,	//3.Wave Clear!
	SND_WAVESTART		= 4,	//4.Wave Start!
	SND_GAMEOVER		= 5,	//5.Game Over!
	SND_TANKMOVE		= 6,	//1.탱크진행소리
	SMD_TANKFASTMOVE	= 7,	//2.탱크빨리진행소리
	SND_TANKHEADROT		= 8,	//3.탱크머리회전소리
	SND_TANKSHELLHIT	= 9,	//4.포탄맞는소리
	SND_TANKSHELLPASSBY = 10,	//5.포탄지나가는소리
	SND_TANKSHELLFIRE	= 11,	//7.포탄발사소리
	SND_TANKEXPLSION	= 12,	//8.탱크터지는소리
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




