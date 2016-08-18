#ifndef _STDAFX_H_ //인클루드 가드
#define	 _STDAFX_H_

#define GMAIN			g_pApp
#define GHINST			g_pApp->m_hInstance
#define GHWND			g_pApp->m_hWnd
#define GDEVICE			g_pApp->m_pd3dDevice
#define GCONTEXT		g_pApp->m_pImmediateContext
#define GGAME			g_pGameplay
#define GGAMEMULTI		g_pGameMultiplay

extern int g_iWave;   //Wave Count, 표시
extern int g_iHP;	  //주인공 체력 표시
extern int g_iBoost;  //주인공 부스트 표시
extern int g_iShell;  //주인공 잔탄 표시

#define G_MACRO_ADD_SCORE 100			//적 폭파시 점수 값
#define G_MACRO_ENEMY_SHELL_DAMAGE 10	//적 탄환 데미지 값
#define G_MACRO_CAR_SPEED 3.0f			//차 Speed
#define G_MACRO_CAR_MAX_SPEED 10.0f		//차 Max Speed
#define G_MACRO_TIRES 6					//타이어 갯수
#define G_MACRO_TANK_TIRES 6			//탱크 타이어 갯수
#define G_MACRO_MAX_TANK 4				//최대 탱크 갯수 for Debug
#define G_MACRO_CANNON_LENGTH 5.0f		//탄환 발사시 포탑 앞에서 발사하도록 계산시 사용하는 길이

#include "GBasisLib_0.h"
#include "GShape.h"
#include "GBackViewCamera.h"

#include "KSound.h"
#include "GBoundingBoxOBB.hpp"
#include "GGuridCamera.h"
#include "GCar.h"
#include "GCarManager.h"
#include "GShell.h"
#include "GShellManager.h"
#include "GSkyBox.h"
#include "GuridMain.h"
#endif




