#include "_StdAfx.h"

int g_iWave = 1;	//Wave Count, 표시
int g_iHP = 100;	//주인공 체력 표시
int g_iBoost = 100; //주인공 부스트 표시
int g_iShell = 10;  //주인공 잔탄 표시

HRESULT GGameSingle::ScreenViewPort(UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;

	return hr;
}


HRESULT		GGameSingle::DeleteResource() {
	HRESULT hr = S_OK;
	// 아래의 경고는 GetDevice()->ClearState();를 호출하지 않을 때 발생한다.
	//D3D10: INFO: ID3D11Device::RSSetState: The currently bound RasterizerState is being deleted; 
	//so naturally, will no longer be bound. [ STATE_SETTING INFO #46: RSSETSTATE_UNBINDDELETINGOBJECT ]
	if (g_pMain->m_pImmediateContext) g_pMain->m_pImmediateContext->ClearState();

	return S_OK;
}
HRESULT GGameSingle::CreateResource()
{
	HRESULT hr;

	if (FAILED(hr = ScreenViewPort(g_pMain->m_SwapChainDesc.BufferDesc.Width, g_pMain->m_SwapChainDesc.BufferDesc.Height)))
	{
		return hr;
	}
	if (m_pMainCamera)
	{
		// Setup the camera's projection parameters
		float fAspectRatio = g_pMain->m_SwapChainDesc.BufferDesc.Width / (FLOAT)g_pMain->m_SwapChainDesc.BufferDesc.Height;
		m_pMainCamera->SetWindow(g_pMain->m_SwapChainDesc.BufferDesc.Width, g_pMain->m_SwapChainDesc.BufferDesc.Height);
	}

	return S_OK;
}


bool GGameSingle::DrawDebug() {
	return true;
}
void GGameSingle::ShellManagerFrame() {
	vector<shared_ptr<GCar>>::iterator _F = m_TankManager.m_vecCars.begin();
	vector<shared_ptr<GCar>>::iterator _L = m_TankManager.m_vecCars.end();

	for (; _F != _L; ++_F)
	{
		m_ShellManager.frame((*_F).get(), &g_pMain->m_Timer, m_pMainCamera);

	}
}
void GGameSingle::ColCheck() {
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
					g_pMain->m_pSound.Play(SND_TANKEXPLSION, true);

					m_iScore += G_MACRO_ADD_SCORE;
					(*_B).reset();
					*_B = 0;
				}

				//주인공탱크에 적총알이 맞았을때
				if ((*_F)->m_bPlayer == false && (*_B)->m_bPlayer == true) {

					//포탄 맞는 소리
					g_pMain->m_pSound.Play(SND_TANKSHELLHIT, true);
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

bool	GGameSingle::Init() {
	if (FAILED(m_pDirectionLine.Create(g_pMain->GetDevice(), L"data/shader/Line.hlsl")))
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
	float fAspectRatio = g_pMain->m_iWindowWidth / (FLOAT)g_pMain->m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight);


	m_pSkyBoxObj = make_shared<GSkyBox>();
	if (m_pSkyBoxObj->Create(g_pMain->GetDevice(), L"data/shader/SkyBox.hlsl") == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	m_pSkyBoxObj->CreateTextureArray(g_pMain->GetDevice(), g_pMain->GetContext());


	m_TankManager.init();


	GMapDesc MapDesc = { 100, 100, 10.0f, L"data/pull.jpg", L"data/shader/CustomizeMap.hlsl" };
	m_CustomMap.Init(g_pMain->GetDevice(), g_pMain->m_pImmediateContext);
	if (FAILED(m_CustomMap.Load(MapDesc)))
	{
		return false;
	}
	//게이지 hp 부스터 총알----------------------------------------------------------------------------------------
	for (int iPlane = 0; iPlane < 3; iPlane++)
	{
		if (FAILED(m_pBar[iPlane].Create(g_pMain->GetDevice(), L"data/shader/Plane.hlsl", L"data/grids.jpg")))
		{
			MessageBox(0, _T("m_pPlane 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}
	D3DXMatrixIdentity(&m_mPlaneWorld[0]);
	D3DXMatrixIdentity(&m_mPlaneWorld[1]);
	D3DXMatrixIdentity(&m_mPlaneWorld[2]);
	//텍스트 ---------------------------------------------------------------------------------------------
	HRESULT hr = g_pMain->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)m_pBackBuffer.GetAddressOf());
	m_Font.Set(g_hWnd, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight, m_pBackBuffer.Get());


}
bool	GGameSingle::Release() {
	m_CustomMap.Release();
	//SAFE_DEL(m_pCamera);
	SAFE_DEL(m_pMainCamera);
	
	return true;
}
bool		GGameSingle::Frame() {
	if ( g_pMain->m_nGameBeforePhase != g_pMain->m_nGamePhase) {
		g_pMain->m_pSound.Stop(SND_HOWTO);
		g_pMain->m_pSound.Stop(SND_INTRO);
		 g_pMain->m_nGameBeforePhase = g_pMain->m_nGamePhase;
	}

	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------

	//m_pMainCamera->Update(m_Timer.GetSPF());
	//m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));

	 m_TankManager.frame(&g_pMain->m_Timer,  m_pMainCamera);

	 m_CustomMap.Frame();

	 ShellManagerFrame();

	 ColCheck();

	 m_WaveManager.Frame();

	 m_fPlayTime = (int)g_fDurationTime;

	return true;
};
bool		GGameSingle::Render() {


	 m_pSkyBoxObj->SetMatrix(0,  m_pMainCamera->GetViewMatrix(),  m_pMainCamera->GetProjMatrix());
	 m_pSkyBoxObj->Render(g_pMain->m_pImmediateContext,  m_pMainCamera);

	//DX::ApplyDSS(m_pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
	//DX::ApplyBS(m_pImmediateContext, DX::GDxState::g_pAlphaBlend);
	 m_CustomMap.SetMatrix( m_pMainCamera->GetWorldMatrix(),  m_pMainCamera->GetViewMatrix(),  m_pMainCamera->GetProjMatrix());
	 m_CustomMap.Render(g_pMain->m_pImmediateContext);

	//탱크 렌더링
	 m_TankManager.render( m_pMainCamera);
	//총알 렌더링
	 m_ShellManager.render();
	//게이지 렌더링
	for (int iBar = 0; iBar < 3; iBar++)
	{
		 m_pBar[iBar].SetMatrix(& m_mPlaneWorld[iBar], & m_pMainCamera->m_matView, & m_pMainCamera->m_matProj);
		 m_pBar[iBar].Render(g_pMain->m_pImmediateContext);
	}
	//텍스트 test
	TCHAR pBuffer[256];
	memset(pBuffer, 0, sizeof(TCHAR) * 256);
	_stprintf_s(pBuffer, _T("PlayTime : %d"),  m_fPlayTime);
	if ( m_Font.m_pTextFormat)
	{
		D2D1_SIZE_F rtSize =  m_Font.m_pRT->GetSize();
		//Draw a grid background.
		int width = static_cast <int> (rtSize.width);
		int height = static_cast <int> (rtSize.height);
		 m_Font.Begin();
		 m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		 m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		RECT rc1 = { 0,0,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight / 2 };
		 m_Font.DrawText(rc1,
			pBuffer,
			D2D1::ColorF(1, 0, 0, 1)
			);

		RECT rc2 = { 30,25,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Score : %d"),  m_iScore);

		 m_Font.DrawText(rc2,
			pBuffer,
			D2D1::ColorF(0, 1, 0, 1)
			);

		RECT rc3 = { 30,50,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Wave : %d"), g_iWave);

		 m_Font.DrawText(rc3,
			pBuffer,
			D2D1::ColorF(0, 0, 1, 1)
			);

		RECT rc4 = { 30,75,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("HP : %d"), g_iHP);

		 m_Font.DrawText(rc4,
			pBuffer,
			D2D1::ColorF(1, 0, 1, 1)
			);

		RECT rc5 = { 30,100,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Boost : %d"), g_iBoost);

		 m_Font.DrawText(rc5,
			pBuffer,
			D2D1::ColorF(1, 1, 0, 1)
			);

		RECT rc6 = { 30,125,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Cannon : %d"), g_iShell);

		 m_Font.DrawText(rc6,
			pBuffer,
			D2D1::ColorF(0, 1, 1, 1)
			);



		 m_Font.End();
	}
	g_pMain->m_pSwapChain->Present(0, 0);


	return true;
};
int GGameSingle::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
	}
	return -1;
}
GGameSingle::GGameSingle()
{
	m_nWavePhase = WAVE_ST_NEWENEMY_ADD;
	m_nWaveBeforePhase = WAVE_ST_TO_MENU;

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


GGameSingle::~GGameSingle()
{
}
