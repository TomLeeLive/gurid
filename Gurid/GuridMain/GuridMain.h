#pragma once
#include "GSprite.h"
#include "GSpriteMgr.h"



class GuridMain : public GBASISLib_0
{


public:

	GGameEnd		m_GameEnd;
	GGameMenu		m_GameMenu;
	GGameSingle		m_GameSingle;
	GGameMulti		m_GameMulti;
	GGameHowto		m_GameHowto;
	GGameCredit		m_GameCredit;


	GAME_STATE		m_nGamePhase;					//게임 상태 변경(시작화면, 게임, 종료
	GAME_STATE		m_nGameBeforePhase;				//게임 이전 상태 저장(for 게임오버 화면에서 점수 출력용. 멀티인지 싱글인지)

	GSound			m_pSound;				// 사운드(fmod)


	bool		SoundLoad();

	//Default
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		DrawDebug();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	GuridMain(void);
	~GuridMain(void);
};

extern GuridMain*	g_pMain; //외부 연결 범위 선언
