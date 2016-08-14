#include "_StdAfx.h"

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
	if (FAILED(m_pDirectionLine.Create(GetDevice(), L"../../data/shader/Line.hlsl")))
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	if (FAILED(m_pPlane.Create(GetDevice(), L"../../data/shader/Plane.hlsl", L"../../data/grids.jpg")))
	{
		MessageBox(0, _T("m_pPlane 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}


	D3DXMatrixIdentity(&m_World[0]);
	D3DXMatrixIdentity(&m_matWorld);

	D3DXMATRIX matRotX, matScale;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	D3DXMatrixMultiply(&m_matWorldPlaneBase, &matScale, &matRotX);

	// 카메라 세팅
	D3DXVECTOR3 vTargetPosition = D3DXVECTOR3(0.0f, -0.1f, 0.0f);
	D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -0.1f);

	SAFE_NEW(m_pCamera, GCamera);
	vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	m_pCamera->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);

	// 뷰포트에 들어 맞게 카메라 조정.
	m_pCamera->SetObjectView(D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));

	// 투영행렬 세팅
	m_pCamera->SetProjMatrix(D3DX_PI * 0.25f,
		(float)m_ViewPort.m_vp.Width / (float)m_ViewPort.m_vp.Height,
		1.0f,
		100.0f);


	// 메인 카메라 뷰 행렬 세팅
	SAFE_NEW(m_pMainCamera, GBackViewCamera);
	//m_pMainCamera->SetModelCenter( D3DXVECTOR3( 1.0f, -1.0f, -1.0f ) );
	m_pMainCamera->SetModelCenter(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMainCamera->SetViewMatrix(*m_pCamera->GetEyePt(), *m_pCamera->GetLookAtPt(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pMainCamera->m_vCameraDestination = *m_pCamera->GetEyePt();

	// Setup the camera's projection parameters
	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);


	m_pSkyBoxObj = make_shared<GSkyBox>();
	if (m_pSkyBoxObj->Create(GetDevice(), L"SkyBox.hlsl") == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	m_pSkyBoxObj->CreateTextureArray(GetDevice(), GetContext());

	for (int i = 0; i < CARTYPE_LAST; i++){
		m_pCar[i]->init(GetDevice());
	}

	return true;
}
bool GuridMain::Frame()
{
	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------
	m_pMainCamera->Update(m_Timer.GetSPF());

	m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));
	
	m_pCar[TANK]->frame(m_matWorld , m_Timer.GetSPF(), m_pMainCamera);

	return true;
}
bool GuridMain::Render()
{
	HRESULT hr;

	m_pSkyBoxObj->SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_pSkyBoxObj->Render(m_pImmediateContext, m_pMainCamera);

	m_pCar[TANK]->render(m_pImmediateContext, m_pMainCamera);

	m_pPlane.SetMatrix(&m_matWorldPlaneBase, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pPlane.Render(m_pImmediateContext);


	return true;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GuridMain::CreateResource()
{
	HRESULT hr;
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

	// 아래의 경고는 GetDevice()->ClearState();를 호출하지 않을 때 발생한다.
	//D3D10: INFO: ID3D11Device::RSSetState: The currently bound RasterizerState is being deleted; 
	//so naturally, will no longer be bound. [ STATE_SETTING INFO #46: RSSETSTATE_UNBINDDELETINGOBJECT ]
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}
bool GuridMain::Release()
{
	for (int i = 0; i < CARTYPE_LAST; i++) {
		SAFE_DEL(m_pCar[i]);
	}

	SAFE_DEL(m_pCamera);
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

GuridMain::GuridMain(void)
{

	m_pCar[SEDAN] = new GCar(SEDAN);
	m_pCar[TANK] = new GCar(TANK);
	m_pCar[TRUCK] = new GCar(TRUCK);
	m_pCar[JEEP] = new GCar(JEEP);


	// 추가
	m_fCameraYaw = 0.0f;
	m_fCameraPitch = 0.0f;
	m_fCameraRoll = 0.0f;
	m_fRadius = 0.0f;

	SAFE_ZERO(m_pCamera);
	SAFE_ZERO(m_pMainCamera);
}

GuridMain::~GuridMain(void)
{

}
GBASIS_RUN(Gurid v0.0.1);