#include "_StdAfx.h"


GuridMain* g_pMain;

int g_iWave = 1;	//Wave Count, 표시
int g_iHP = 100;	//주인공 체력 표시
int g_iBoost = 100; //주인공 부스트 표시
int g_iShell = 10;  //주인공 잔탄 표시


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

// 윈도우 메세지 
int GuridMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
	}
	return -1;
}
bool GuridMain::Init()
{

	if (FAILED(m_pDirectionLine.Create(GetDevice(), L"data/shader/Line.hlsl")))
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	D3DXMATRIX matRotX, matScale;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	D3DXMatrixMultiply(&m_matWorldPlaneBase, &matScale, &matRotX);
	// 카메라 세팅
	D3DXVECTOR3 vTargetPosition = D3DXVECTOR3(0.0f, -0.1f, 0.0f);
	D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -0.1f);

	//SAFE_NEW(m_pCamera, GCamera);
	// 메인 카메라 뷰 행렬 세팅
	SAFE_NEW(m_pMainCamera, GGuridCamera);
	vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	m_pMainCamera->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);

	// 뷰포트에 들어 맞게 카메라 조정.
	m_pMainCamera->SetObjectView(D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));

	// 투영행렬 세팅
	m_pMainCamera->SetProjMatrix(D3DX_PI * 0.25f,
		(float)m_ViewPort.m_vp.Width / (float)m_ViewPort.m_vp.Height,
		1.0f,
		100.0f);



	//m_pMainCamera->SetModelCenter( D3DXVECTOR3( 1.0f, -1.0f, -1.0f ) );
	m_pMainCamera->SetModelCenter(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMainCamera->SetViewMatrix(*m_pMainCamera->GetEyePt(), *m_pMainCamera->GetLookAtPt(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pMainCamera->m_vCameraDestination = *m_pMainCamera->GetEyePt();

	// Setup the camera's projection parameters
	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);


	m_pSkyBoxObj = make_shared<GSkyBox>();
	if (m_pSkyBoxObj->Create(GetDevice(), L"data/shader/SkyBox.hlsl") == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	m_pSkyBoxObj->CreateTextureArray(GetDevice(), GetContext());


	m_TankManager.init();


	GMapDesc MapDesc = { 100, 100, 10.0f, L"data/pull.jpg", L"data/shader/CustomizeMap.hlsl" };
	m_CustomMap.Init(GetDevice(), m_pImmediateContext);
	if (FAILED(m_CustomMap.Load(MapDesc)))
	{
		return false;
	}
	//게이지 hp 부스터 총알----------------------------------------------------------------------------------------
	for (int iPlane = 0; iPlane < 3; iPlane++)
	{
		if (FAILED(m_pBar[iPlane].Create(GetDevice(), L"data/shader/Plane.hlsl", L"data/grids.jpg")))
		{
			MessageBox(0, _T("m_pPlane 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}
	D3DXMatrixIdentity(&m_mPlaneWorld[0]);
	D3DXMatrixIdentity(&m_mPlaneWorld[1]);
	D3DXMatrixIdentity(&m_mPlaneWorld[2]);
	//텍스트 ---------------------------------------------------------------------------------------------
	HRESULT hr = GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)m_pBackBuffer.GetAddressOf());
	m_Font.Set(g_hWnd, m_iWindowWidth, m_iWindowHeight, m_pBackBuffer.Get());


	//사운드 로딩
	hr = this->SoundLoad();






	m_GameMenu.Init();
	m_GameHowto.Init();
	m_GameCredit.Init();




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
void GuridMain::ShellManagerFrame(){
	vector<shared_ptr<GCar>>::iterator _F = m_TankManager.m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_TankManager.m_vecCars.end();

	for (; _F != _L; ++_F)
	{
		m_ShellManager.frame((*_F).get(), &m_Timer, m_pMainCamera);

	}
}
void GuridMain::ColCheck() {
	vector<shared_ptr<GShell>>::iterator _F = m_ShellManager.m_vecShells.begin();
	vector<shared_ptr<GShell>>::iterator _L = m_ShellManager.m_vecShells.end();

	
	for (; _F != _L; ++_F)
	{
		// Collision Detection Test!
		vector<shared_ptr<GCar>>::iterator _B = m_TankManager.m_vecCars.begin();
		vector<shared_ptr<GCar>>::iterator _E = m_TankManager.m_vecCars.end();

		for (; _B != _E; ++_B)
		{	
			int nRet = BoxBoxIntersectionTest(*((*_B).get()), *(*_F));
			if (nRet == 1) {
				
				//적탱크에 주인공 총알이 맞았을때.
				if ((*_F)->m_bPlayer == true && (*_B)->m_bPlayer == false) {

					//탱크 터지는 소리
					m_pSound.Play(SND_TANKEXPLSION, true);
					
					m_iScore += G_MACRO_ADD_SCORE;
					(*_B).reset();
					*_B = 0;
				}

				//주인공탱크에 적총알이 맞았을때
				if ((*_F)->m_bPlayer == false && (*_B)->m_bPlayer == true) {

					//포탄 맞는 소리
					m_pSound.Play(SND_TANKSHELLHIT, true);
					//m_pSound.Play(SND_TANKEXPLSION, true);

					(*_B)->m_iHP -= G_MACRO_ENEMY_SHELL_DAMAGE;
				}

				//총알 erase를 위해..
				(*_F).reset();//delete (*_F);
				*_F = 0;
				break;
			}
		}

		_B = m_TankManager.m_vecCars.begin();
		while (_B != m_TankManager.m_vecCars.end())
		{
			if (*_B == 0) {

				_B = m_TankManager.m_vecCars.erase(_B);
			}
			else {
				_B++;
			}
		}

	}

	_F = m_ShellManager.m_vecShells.begin();
	while (_F != m_ShellManager.m_vecShells.end())
	{
		if (*_F == 0) {

			_F = m_ShellManager.m_vecShells.erase(_F);
		}
		else {
			_F++;
		}
	}
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

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GuridMain::CreateResource()
{
	HRESULT hr;


	m_GameMenu.CreateResource();
	m_GameHowto.CreateResource();
	m_GameCredit.CreateResource();



	if (FAILED(hr = ScreenViewPort(m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height)))
	{
		return hr;
	}
	if (m_pMainCamera)
	{
		// Setup the camera's projection parameters
		float fAspectRatio = m_SwapChainDesc.BufferDesc.Width / (FLOAT)m_SwapChainDesc.BufferDesc.Height;
		m_pMainCamera->SetWindow(m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height);
	}
	return S_OK;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GuridMain::DeleteResource()
{
	HRESULT hr = S_OK;
	
	
	m_GameMenu.DeleteResource();
	m_GameHowto.DeleteResource();
	m_GameCredit.DeleteResource();




	// 아래의 경고는 GetDevice()->ClearState();를 호출하지 않을 때 발생한다.
	//D3D10: INFO: ID3D11Device::RSSetState: The currently bound RasterizerState is being deleted; 
	//so naturally, will no longer be bound. [ STATE_SETTING INFO #46: RSSETSTATE_UNBINDDELETINGOBJECT ]
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}
bool GuridMain::Release()
{
	m_GameMenu.Release();
	m_GameHowto.Release();
	m_GameCredit.Release();





	m_pSound.Release();
	m_Font.Release();
	m_CustomMap.Release();
	//SAFE_DEL(m_pCamera);
	SAFE_DEL(m_pMainCamera);
	return true;
}
bool GuridMain::DrawDebug()
{

	return GBASISLib_0::DrawDebug();
}
HRESULT GuridMain::ScreenViewPort(UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;

	return hr;
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
	iIndex = m_pSound.Load("data/sound/tank_1_move.wav");				//1.탱크진행소리
	iIndex = m_pSound.Load("data/sound/tank_2_move_fast.wav");			//2.탱크빨리진행소리
	iIndex = m_pSound.Load("data/sound/tank_3_head_move.wav");			//3.탱크머리회전소리
	iIndex = m_pSound.Load("data/sound/tank_4_cannon_shoot.wav");		//4.포탄맞는소리
	iIndex = m_pSound.Load("data/sound/tank_5_shell_passby.wav");		//5.포탄지나가는소리
	iIndex = m_pSound.Load("data/sound/tank_7_shell_hit.wav");			//7.포탄발사소리
	iIndex = m_pSound.Load("data/sound/tank_8_explosion.wav");			//8.탱크터지는소리

/*
	------------------------------------------
0	0.HowTo.
	안녕하세요. 개발자 이재준 입니다.

	탱크 조작은 W,A,S,D 로 전후진좌우 이동
	Space로 포탄발사
	Q,E로 탱크 포탑 회전
	LShift로 가속(Boost)
	마우스 오른쪽 클릭후 드래그하여 시점 변경

	입니다.
	------------------------------------------
1	1_intro.wav 나는 니가 지난 여름에 한 탱크 게임을 알고 있다.(I Know the tank game you did last summer.)
2	2.Get ready!
3	3.Wave Clear!
4	4.Wave Start!
5	5.Game Over!
	------------------------------------------
6	1.탱크진행소리
7	2.탱크빨리진행소리
8	3.탱크머리회전소리
9	4.포탄맞는소리
10	5.포탄지나가는소리
11	7.포탄발사소리
12	8.탱크터지는소리
	------------------------------------------
*/
	return hr;
}

GuridMain::GuridMain(void)
{

	m_nGamePhase = ST_START;//ST_START;
	m_nWavePhase = WAVE_ST_NEWENEMY_ADD;

	m_iScore = 0;
	m_fPlayTime = 0.0f;

	//m_pCar[SEDAN] = new GCar(SEDAN);
	//m_pCar[TANK] = new GCar(TANK);
	//m_pCar[TRUCK] = new GCar(TRUCK);
	//m_pCar[JEEP] = new GCar(JEEP);



	// 추가
	m_fCameraYaw = 0.0f;
	m_fCameraPitch = 0.0f;
	m_fCameraRoll = 0.0f;
	m_fRadius = 0.0f;

	SAFE_ZERO(m_pMainCamera);
}

GuridMain::~GuridMain(void)
{

}
GBASIS_RUN(Gurid v0.0.2);
