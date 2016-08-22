#include "_StdAfx.h"


GuridMain* g_pMain;




//--------------------------------------------------------------------------------------
// CameraViewStyle
//--------------------------------------------------------------------------------------
T_STR CameraViewStyle[] =
{
	_T("Top g_matView"),
	_T("Front g_matView"),
	_T("Side g_matView"),
	_T("User g_matView"),
};

// ������ �޼��� 
int GuridMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_GameSingle.WndProc(hWnd, msg, wParam, lParam);

	return -1;
}
bool GuridMain::Init()
{
	HRESULT hr = S_OK;

	m_GameSingle.Init();

	m_GameMenu.Init();
	m_GameHowto.Init();
	m_GameCredit.Init();

	//���� �ε�
	hr = this->SoundLoad();
	//m_pSound.Play(1, true);

	return true;
}



bool GuridMain::Frame()
{

	switch (m_nGamePhase)
	{
	case ST_START:
		m_GameMenu.Frame();
		break;

	case ST_SINGLEGAME:
		m_GameSingle.Frame();
		break;

	case ST_MULTI:
		m_GameMulti.Frame();
		break;

	case ST_END:
		m_GameEnd.Frame();
		break;
	case ST_HOWTO:
		m_GameHowto.Frame();
		break;
	case ST_CREDIT:
		m_GameCredit.Frame();
		break;
	}
	m_pSound.Frame();


	return true;
}
bool GuridMain::Render()
{
	HRESULT hr;


	switch (m_nGamePhase)
	{
	case ST_START:
		m_GameMenu.Render();
		break;

	case ST_SINGLEGAME:
		m_GameSingle.Render();
		break;

	case ST_MULTI:
		m_GameMulti.Render();
		break;

	case ST_END:
		m_GameEnd.Render();
		break;
	case ST_HOWTO:
		m_GameHowto.Render();
		break;
	case ST_CREDIT:
		m_GameCredit.Render();
		break;
	}
	
	return true;
}

HRESULT GuridMain::CreateResource()
{
	HRESULT hr;


	m_GameMenu.CreateResource();
	m_GameHowto.CreateResource();
	m_GameCredit.CreateResource();
	m_GameSingle.CreateResource();



	return S_OK;
}

HRESULT GuridMain::DeleteResource()
{
	HRESULT hr = S_OK;
	
	
	m_GameMenu.DeleteResource();
	m_GameHowto.DeleteResource();
	m_GameCredit.DeleteResource();
	m_GameSingle.DeleteResource();




	return S_OK;
}

bool GuridMain::Release()
{
	m_GameMenu.Release();
	m_GameHowto.Release();
	m_GameCredit.Release();
	m_GameSingle.Release();




	m_pSound.Release();
	m_Font.Release();

	//SAFE_DEL(m_pSprite);
	

	return true;
}

bool GuridMain::DrawDebug()
{
	m_GameSingle.DrawDebug();

	return GBASISLib_0::DrawDebug();
}

bool GuridMain::SoundLoad()
{
	HRESULT hr = 0;

	m_pSound.Init();

	int iIndex = 0;

	iIndex = m_pSound.Load("data/sound/0_howto.wav");					//0.Hotwo
	iIndex = m_pSound.Load("data/sound/1_intro.wav");					//1.intro.wav
	iIndex = m_pSound.Load("data/sound/2_get_ready.wav");				//2.Get ready!
	iIndex = m_pSound.Load("data/sound/3_wave_clear.wav");				//3.Wave Clear!
	iIndex = m_pSound.Load("data/sound/4_wave_start.wav");				//4.Wave Start!
	iIndex = m_pSound.Load("data/sound/5_game_over.wav");				//5.Game Over!
	iIndex = m_pSound.Load("data/sound/tank_1_move.wav");				//1.��ũ����Ҹ�
	iIndex = m_pSound.Load("data/sound/tank_2_move_fast.wav");			//2.��ũ��������Ҹ�
	iIndex = m_pSound.Load("data/sound/tank_3_head_move.wav");			//3.��ũ�Ӹ�ȸ���Ҹ�
	iIndex = m_pSound.Load("data/sound/tank_4_cannon_shoot.wav");		//4.��ź�´¼Ҹ�
	iIndex = m_pSound.Load("data/sound/tank_5_shell_passby.wav");		//5.��ź�������¼Ҹ�
	iIndex = m_pSound.Load("data/sound/tank_7_shell_hit.wav");			//7.��ź�߻�Ҹ�
	iIndex = m_pSound.Load("data/sound/tank_8_explosion.wav");			//8.��ũ�����¼Ҹ�

/*
	------------------------------------------
0	0.HowTo.
	�ȳ��ϼ���. ������ ������ �Դϴ�.

	��ũ ������ W,A,S,D �� �������¿� �̵�
	Space�� ��ź�߻�
	Q,E�� ��ũ ��ž ȸ��
	LShift�� ����(Boost)
	���콺 ������ Ŭ���� �巡���Ͽ� ���� ����

	�Դϴ�.
	------------------------------------------
1	1_intro.wav ���� �ϰ� ���� ������ �� ��ũ ������ �˰� �ִ�.(I Know the tank game you did last summer.)
2	2.Get ready!
3	3.Wave Clear!
4	4.Wave Start!
5	5.Game Over!
	------------------------------------------
6	1.��ũ����Ҹ�
7	2.��ũ��������Ҹ�
8	3.��ũ�Ӹ�ȸ���Ҹ�
9	4.��ź�´¼Ҹ�
10	5.��ź�������¼Ҹ�
11	7.��ź�߻�Ҹ�
12	8.��ũ�����¼Ҹ�
	------------------------------------------
*/
	return hr;
}



//HRESULT GuridMain::ScreenViewPort(UINT iWidth, UINT iHeight)
//{
//	HRESULT hr = S_OK;
//
//	UINT iRectWidth = iWidth / 2;
//	UINT iRectHeight = iHeight / 2;
//
//	m_ViewPort.Set(GetDevice(), 0, 0, iRectWidth, iRectHeight, 0.0f, 1.0f);
//	//m_ViewPort[1].Set(GetDevice(), iRectWidth, 0, iRectWidth, iRectHeight, 0.0f, 1.0f);
//	//m_ViewPort[2].Set(GetDevice(), iRectWidth, iRectHeight, iRectWidth, iRectHeight, 0.0f, 1.0f);
//	//m_ViewPort[3].Set(GetDevice(), 0, iRectHeight, iRectWidth, iRectHeight, 0.0f, 1.0f);
//	return hr;
//}



GuridMain::GuridMain(void)
{

	m_nGamePhase = ST_START;//ST_START;
	m_nGameBeforePhase = ST_END;



}

GuridMain::~GuridMain(void)
{
}
GBASIS_RUN(I KNOW THE TANK GAME YOU DID LAST SUMMER v0.0.3);
