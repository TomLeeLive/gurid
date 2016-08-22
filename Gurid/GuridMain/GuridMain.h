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


	GAME_STATE		m_nGamePhase;					//���� ���� ����(����ȭ��, ����, ����
	GAME_STATE		m_nGameBeforePhase;				//���� ���� ���� ����(for ���ӿ��� ȭ�鿡�� ���� ��¿�. ��Ƽ���� �̱�����)

	GSound			m_pSound;				// ����(fmod)


	bool		SoundLoad();

	//Default
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		DrawDebug();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	GuridMain(void);
	~GuridMain(void);
};

extern GuridMain*	g_pMain; //�ܺ� ���� ���� ����
