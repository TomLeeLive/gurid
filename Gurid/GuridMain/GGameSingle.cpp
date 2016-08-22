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
HRESULT GGameSingle::SetBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC			BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_COLOR;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&BlendState, &m_pBS[0])))
	{
		H(hr);
		return hr;
	}

	ID3D11BlendState*			m_pBlend1;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&BlendState, &m_pBS[1])))
	{
		H(hr);
		return hr;
	}
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&BlendState, &m_pBS[2])))
	{
		H(hr);
		return hr;
	}

	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&BlendState, &m_pBS[3])))
	{
		H(hr);
		return hr;
	}
	return hr;
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

#pragma region Effect
	UINT iIndex;

	//--------------------------------------------------------------------------------------
	// 텍스처 에니메이션
	//--------------------------------------------------------------------------------------	

#ifdef _DEBUG
	SAFE_NEW(m_pSprite[0], GSprite);
	if (m_pSprite[0]->Create(g_pMain->GetDevice(),
		L"data/shader/Plane.hlsl", 0) == false)
	{
		MessageBox(0, _T("m_pSprite->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
#else
	if (iIndex = I_Sprite.Add(GetDevice(), 0, L"data/shader/Plane.hlsl",
		DX::GDxState::g_pBSMaxOneOne, true))
		//DX::TDxState::g_pAlphaBlend, true);
	{
		MessageBox(0, _T("m_pSprite[0]->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pSprite[0] = I_Sprite.GetPtr(iIndex);
#endif
	T_STR_VECTOR FileList;
	for (int iTex = 0; iTex < 18; iTex++)
	{
		TCHAR pBuffer[50];
		if (iTex < 10)
			_stprintf_s(pBuffer, L"data/sou0%d.dds", iTex);
		else
			_stprintf_s(pBuffer, L"data/sou%d.dds", iTex);
		FileList.push_back(pBuffer);
	}
	m_pSprite[0]->SetTextureArray(FileList);
	m_pSprite[0]->SetRectAnimation(1.0f);

	////--------------------------------------------------------------------------------------
	//// UV 에니메이션
	////--------------------------------------------------------------------------------------
	SAFE_NEW(m_pSprite[0], GSprite);	//if (!m_pSprite[0]) m_pSprite[0] = new GSprite;
										//SAFE_NEW(m_pSprite, GSprite);
#ifdef _DEBUG
	if (m_pSprite[0]->Create(g_pMain->GetDevice(), L"data/shader/Plane.hlsl", L"data/potatoo.dds") == false)
	{
		MessageBox(0, _T("m_pSprite->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
#else
	if (iIndex = I_Sprite.Add(GetDevice(), L"data/potatoo.dds", L"data/shader/Plane.hlsl",
		DX::GDxState::g_pBSMaxOneOne, true))
		//DX::TDxState::g_pAlphaBlend, true);
	{
		MessageBox(0, _T("m_pSprite[1]->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pSprite[0] = I_Sprite.GetPtr(iIndex);
#endif
	m_pSprite[0]->SetRectAnimation(1.0f, 4, 128, 4, 128);

	//--------------------------------------------------------------------------------------
	// 월드  행렬
	//--------------------------------------------------------------------------------------
	D3DXMatrixIdentity(&m_matPlaneWorld);
	//--------------------------------------------------------------------------------------
	// 카메라  행렬 
	//--------------------------------------------------------------------------------------	

	//m_pMainCamera = make_shared<GGuridCamera>();
	//m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 0.0f, -5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight);

	if (FAILED(SetBlendState()))
	{
		return false;
	}
#pragma endregion


	D3DXMatrixIdentity(&m_mPlaneWorld[0]);
	D3DXMatrixIdentity(&m_mPlaneWorld[1]);
	D3DXMatrixIdentity(&m_mPlaneWorld[2]);
	//텍스트 ---------------------------------------------------------------------------------------------
	HRESULT hr = g_pMain->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)m_pBackBuffer.GetAddressOf());
	m_Font.Set(g_hWnd, g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight, m_pBackBuffer.Get());


}
bool	GGameSingle::Release() {
	m_CustomMap.Release();
	m_pPlane.Release();
	m_pDirectionLine.Release();
	m_Font.Release();
	for (int i = 0; i < 3; i++)
		m_pBar[i].Release();

	//SAFE_DEL(m_pCamera);
	SAFE_DEL(m_pMainCamera);
	
	SAFE_DEL(m_pSprite[0]);
	SAFE_DEL(m_pSprite[1]);

	return true;
}
bool		GGameSingle::Frame() {
	if ( g_pMain->m_nGameBeforePhase != g_pMain->m_nGamePhase) {
		g_pMain->m_pSound.Stop(SND_HOWTO);
		g_pMain->m_pSound.Stop(SND_INTRO);
		 g_pMain->m_nGameBeforePhase = g_pMain->m_nGamePhase;
	}

	m_WaveManager.Frame();

	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------

	//m_pMainCamera->Update(m_Timer.GetSPF());
	//m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));

	 m_TankManager.frame(&g_pMain->m_Timer,  m_pMainCamera);

	 m_CustomMap.Frame();

	 ShellManagerFrame();

	 ColCheck();



	 m_fPlayTime = (int)g_fDurationTime;

#pragma region Effect
	//// 2초당 1회전( 1 초 * D3DX_PI = 3.14 )
	//float t = g_pMain->m_Timer.GetElapsedTime() * (float)D3DX_PI;
	////--------------------------------------------------------------------------------------
	//// 엔진에 있는 카메라의 월드행렬 갱신
	////--------------------------------------------------------------------------------------
	//g_pMain->m_pMainCamera->Update(g_fSecPerFrame);
	//g_pMain->m_matWorld = *g_pMain->m_pMainCamera->GetWorldMatrix();
	////m_pBox->m_cbData.Color = D3DXVECTOR4(1, 1, 1, cosf(m_Timer.GetElapsedTime())*0.5f + 0.5f);


	//// 빌보드 행렬
	//FLOAT fDeterminant;
	//D3DXMATRIX matBillboard;
	//D3DXMatrixInverse(&matBillboard, &fDeterminant, g_pMain->m_pMainCamera->GetViewMatrix());
	//matBillboard._41 = 0.0f;
	//matBillboard._42 = 0.0f;
	//matBillboard._43 = 0.0f;
	//matBillboard._44 = 1.0f;

	//for (int iPlane = 0; iPlane < 2; iPlane++)
	//{
	//	g_pMain->m_pSprite[iPlane]->SetMatrix(&matBillboard, g_pMain->m_pMainCamera->GetViewMatrix(), g_pMain->m_pMainCamera->GetProjMatrix());
	//	g_pMain->m_pSprite[iPlane]->Frame(g_pMain->m_pImmediateContext, g_pMain->m_Timer.GetElapsedTime(), g_fSecPerFrame);
	//}
#pragma endregion

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
		 m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		 m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		RECT rc1 = { 30,0,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight / 2 };
		 m_Font.DrawText(rc1,
			pBuffer,
			D2D1::ColorF(1, 0, 0, 1)
			);

		RECT rc2 = { 30,225,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Score : %d"),  m_iScore);

		 m_Font.DrawText(rc2,
			pBuffer,
			D2D1::ColorF(0, 1, 0, 1)
			);

		RECT rc3 = { 30,250,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Wave : %d"), g_iWave);

		 m_Font.DrawText(rc3,
			pBuffer,
			D2D1::ColorF(0, 0, 1, 1)
			);

		RECT rc4 = { 30,275,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("HP : %d"), g_iHP);

		 m_Font.DrawText(rc4,
			pBuffer,
			D2D1::ColorF(1, 0, 1, 1)
			);

		RECT rc5 = { 30,300,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Boost : %d"), g_iBoost);

		 m_Font.DrawText(rc5,
			pBuffer,
			D2D1::ColorF(1, 1, 0, 1)
			);

		RECT rc6 = { 30,325,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		_stprintf_s(pBuffer, _T("Cannon : %d"), g_iShell);

		 m_Font.DrawText(rc6,
			pBuffer,
			D2D1::ColorF(0, 1, 1, 1)
			);
		 RECT rc7 = { 30,350,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
		 _stprintf_s(pBuffer, _T("EnemyTank : %d"), m_TankManager.m_vecCars.size()-1);

		 m_Font.DrawText(rc7,
			 pBuffer,
			 D2D1::ColorF(0, 0, 0, 1)
			 );

		 m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		 if(m_nWavePhase == WAVE_ST_GAMEOVER){
			 RECT rc8 = { -80,250,  g_pMain->m_iWindowWidth,  g_pMain->m_iWindowHeight };
			 _stprintf_s(pBuffer, _T("Gave Over. Press \"Enter\" to Return to Menu"));

			 m_Font.DrawText(rc8,
				 pBuffer,
				 D2D1::ColorF(0, 0, 0, 1)
				 );
		 }
		 m_Font.End();
	}
	
	
#pragma region Effect
	////-----------------------------------------------------------------------
	//// 현재 세팅된 뷰포트 정보 저장
	////-----------------------------------------------------------------------
	//D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	//UINT nViewPorts = 1;
	//g_pMain->m_pImmediateContext->RSGetViewports(&nViewPorts, vpOld);

	//// Store the old render targets
	//ID3D11RenderTargetView* pOldRTV;
	//ID3D11DepthStencilView* pOldDSV;
	//g_pMain->m_pImmediateContext->OMGetRenderTargets(1, &pOldRTV, &pOldDSV);
	//{
	//	ApplyDSS(g_pMain->m_pImmediateContext, GDxState::g_pDSSDepthDisableNoWrite);
	//	g_pMain->m_pImmediateContext->OMSetBlendState(GDxState::g_pAlphaBlend, 0, -1);
	//	D3DXMATRIX matWorld;
	//	D3DXMatrixScaling(&matWorld, 10.0f, 10.0f, 10.0f);
	//	matWorld._43 = 5.0f;
	//	g_pMain->m_BigPlane.SetMatrix(&matWorld, g_pMain->m_pMainCamera->GetViewMatrix(), g_pMain->m_pMainCamera->GetProjMatrix());
	//	g_pMain->m_BigPlane.Render(g_pMain->m_pImmediateContext);
	//	//-----------------------------------------------------------------------
	//	// 0번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------
	//	g_pMain->m_pImmediateContext->OMSetBlendState(g_pMain->m_pBS[0].Get(), 0, -1);
	//	static float fTime = 0;
	//	fTime += g_fSecPerFrame;
	//	if (fTime > 1.0f)
	//	{
	//		g_pMain->m_pSprite[0]->m_cbData.Color.x = randstep(0.0f, 1.0f);
	//		g_pMain->m_pSprite[0]->m_cbData.Color.y = 0;// randstep(0.0f, 1.0f);
	//		g_pMain->m_pSprite[0]->m_cbData.Color.z = 0;// randstep(0.0f, 1.0f);
	//		g_pMain->m_pSprite[0]->m_cbData.Color.w = 1.0f;
	//		fTime = 0;
	//	}
	//	RenderScene(g_pMain->m_pSprite[0]);
	//	//-----------------------------------------------------------------------
	//	// 1번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------
	//	RenderScene(g_pMain->m_pSprite[1]);
	//	//-----------------------------------------------------------------------
	//	// 2번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------		
	//	g_pMain->m_pImmediateContext->OMSetBlendState(g_pMain->m_pBS[1].Get(), 0, -1);
	//	RenderScene(g_pMain->m_pSprite[0]);
	//	g_pMain->m_matPlaneWorld._43 = -2.0f;
	//	RenderScene(g_pMain->m_pSprite[1]);
	//	//-----------------------------------------------------------------------
	//	// 3번 뷰 포트 랜더링
	//	//-----------------------------------------------------------------------		
	//	g_pMain->m_pImmediateContext->OMSetBlendState(g_pMain->m_pBS[2].Get(), 0, -1);
	//	RenderScene(g_pMain->m_pSprite[1]);
	//	g_pMain->m_matPlaneWorld._43 = -1.0f;
	//	RenderScene(g_pMain->m_pSprite[0]);
	//}
	////-----------------------------------------------------------------------
	//// 기본 뷰포트 정보로 복원
	////-----------------------------------------------------------------------
	//g_pMain->m_pImmediateContext->RSSetViewports(nViewPorts, vpOld);
	//g_pMain->m_pImmediateContext->OMSetRenderTargets(1, &pOldRTV, pOldDSV);

	//// OMGetRenderTargets함수를 사용하였다면 반드시 아래와 같이 소멸시켜야 한다.
	//SAFE_RELEASE(pOldRTV);
	//SAFE_RELEASE(pOldDSV);

	////-----------------------------------------------------------------------
	//// 뷰포트 번호 출력
	////-----------------------------------------------------------------------
	////for (int iVp = 0; iVp < 4; iVp++)
	////{
	//	RECT rc;
	//	rc.left = g_pMain->m_ViewPort.m_vp.TopLeftX + g_pMain->m_ViewPort.m_vp.Width*0.5f;
	//	rc.top = g_pMain->m_ViewPort.m_vp.TopLeftY;
	//	rc.right = g_pMain->m_ViewPort.m_vp.Width + rc.left;
	//	rc.bottom = g_pMain->m_ViewPort.m_vp.Height + rc.top;
	//	T_STR str = L"Vp";
	//	TCHAR strNumber[32];
	//	str += _itow(0, strNumber, 10);// _wtoi //ivp 0으로
	//	g_pMain->DrawDebugRect(&rc, const_cast<TCHAR*>(str.c_str()));
	////}
#pragma endregion
	
	
	g_pMain->m_pSwapChain->Present(0, 0);
	return true;
};
bool		GGameSingle::RenderScene(GSprite* pPlane)
{
	m_ViewPort.Apply(g_pImmediateContext, g_pMain->GetRenderTargetView(), g_pMain->GetDepthStencilView());

	pPlane->SetMatrix(&m_matPlaneWorld, m_pMainCamera->GetViewMatrix(), &m_ViewPort.m_matProj);
	pPlane->Render(g_pImmediateContext);
	return true;
}

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
	m_nWavePhase = WAVE_ST_NEWGAME;
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

	SAFE_ZERO(m_pSprite[0]);
	SAFE_ZERO(m_pSprite[1]);

}


GGameSingle::~GGameSingle()
{
}
