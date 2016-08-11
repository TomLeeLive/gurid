#include "Sample.h"
#include "GTimer.h"


D3DXMATRIX g_matWorld_body;
D3DXMATRIX g_matWorld_head;
D3DXMATRIX g_matWorld_tire[4];

// 윈도우 메세지 
int Sample::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
	}
	return -1;  
}
bool Sample::Init()
{
	if (FAILED(m_pDirectionLine.Create(GetDevice(), L"../../data/shader/Line.hlsl")))
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	if (FAILED(m_pPlane.Create(GetDevice(), L"../../data/shader/Plane.hlsl", L"../../data/Sand.jpg")))
	{
		MessageBox(0, _T("m_pPlane 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pBox.Create(GetDevice(), L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pBox2.Create(GetDevice(), L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox2 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}


	for (int i = 0; i < G_MACRO_TIRES; i++){
		if (FAILED(m_pTire[i].Create(GetDevice(), L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
		{
			MessageBox(0, _T("m_pTire1 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}
	
    D3DXMatrixIdentity( &m_World[0] );
	D3DXMatrixIdentity( &m_matWorld );

	D3DXMATRIX matRotX, matScale;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	D3DXMatrixMultiply(&m_matWorldPlaneBase, &matScale, &matRotX);
	
	// 카메라 세팅
	D3DXVECTOR3 vTargetPosition	= D3DXVECTOR3( 0.0f, -0.1f, 0.0f );
	D3DXVECTOR3 vUpVector( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 vCameraPosition	= D3DXVECTOR3( 0.0f, 1.0f, -0.1f );
	
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
	return true;
}
bool Sample::Frame()
{

	
	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------
	m_pMainCamera->Update(m_Timer.GetSPF());

	D3DXMATRIX temp_s, temp_r, temp_t, temp_r_y, temp_r_z;
	D3DXMatrixIdentity(&temp_s);
	D3DXMatrixIdentity(&temp_r);
	D3DXMatrixIdentity(&temp_t);

	m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));	


	m_matWorld._42 -= 0.5f;
	g_matWorld_body = m_matWorld;
	g_matWorld_body._11 *= 3.0f;
	g_matWorld_body._33 *= 5.0f;


	g_matWorld_head = m_matWorld;
	g_matWorld_head._11 *= 3.0f;
	g_matWorld_head._33 *= 3.0f;


	static float rot = 0.0f;
	rot += 100.0f*m_Timer.GetSPF();

	D3DXMatrixScaling(&temp_s, 3.0f, 0.5f, 3.0f);
	//D3DXMatrixRotationZ(&temp_r, D3DXToRadian(90.0f));

	D3DXMatrixRotationY(&temp_r_y, D3DXToRadian(-rot));
	D3DXMatrixRotationZ(&temp_r_z, D3DXToRadian(90.0f));

	D3DXMatrixTranslation(&temp_t, -2.0f, -2.0f, 3.0f);

	g_matWorld_tire[0] = temp_s*temp_r_y*temp_r_z*temp_t*m_matWorld;
	D3DXMatrixTranslation(&temp_t, 2.0f, -2.0f, 3.0f);
	g_matWorld_tire[1] = temp_s*temp_r_y*temp_r_z*temp_t*m_matWorld;
	D3DXMatrixTranslation(&temp_t, -2.0f, -2.0f, -3.0f);
	g_matWorld_tire[2] = temp_s*temp_r_y*temp_r_z*temp_t*m_matWorld;
	D3DXMatrixTranslation(&temp_t, 2.0f, -2.0f, -3.0f);
	g_matWorld_tire[3] = temp_s*temp_r_y*temp_r_z*temp_t*m_matWorld;

	return true;
}
bool Sample::Render()
{
	HRESULT hr;	

	m_pBox.SetMatrix(&g_matWorld_body, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pBox.Render(m_pImmediateContext);

	//matWorld2._41 = ;
	g_matWorld_head._42 += 2.0f ;
	//matWorld2._43 += 30.0f;

	m_pBox2.SetMatrix(&g_matWorld_head, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pBox2.Render(m_pImmediateContext);


	for (int i = 0; i < G_MACRO_TIRES; i++){
		m_pTire[i].SetMatrix(&g_matWorld_tire[i], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pTire[i].Render(m_pImmediateContext);
	}

	m_pPlane.SetMatrix(&m_matWorldPlaneBase, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pPlane.Render(m_pImmediateContext);	
	//-----------------------------------------------------------------------
	// 현재 세팅된 뷰포트 정보 저장
	//-----------------------------------------------------------------------
	//D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
 //   UINT nViewPorts = 1;
 //   m_pImmediateContext->RSGetViewports( &nViewPorts, vpOld );	

	//-----------------------------------------------------------------------
	// 기본 뷰포트 정보로 복원
	//-----------------------------------------------------------------------
	//m_pImmediateContext->RSSetViewports(nViewPorts, vpOld);
	
	
	return true;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateResource()
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
HRESULT Sample::DeleteResource()
{
	HRESULT hr= S_OK;	

	// 아래의 경고는 GetDevice()->ClearState();를 호출하지 않을 때 발생한다.
	//D3D10: INFO: ID3D11Device::RSSetState: The currently bound RasterizerState is being deleted; 
	//so naturally, will no longer be bound. [ STATE_SETTING INFO #46: RSSETSTATE_UNBINDDELETINGOBJECT ]
	if( m_pImmediateContext ) m_pImmediateContext->ClearState();
	return S_OK;
}
bool Sample::Release()
{
	SAFE_DEL(m_pCamera);
	SAFE_DEL(m_pMainCamera);
	return true;
}
bool Sample::DrawDebug()
{
	
	return GBASISLib_0::DrawDebug();
}
HRESULT Sample::ScreenViewPort(UINT iWidth, UINT iHeight)
{	
	HRESULT hr = S_OK;
	
	return hr;
}

Sample::Sample(void)
{	
	// 추가
	m_fCameraYaw			= 0.0f;
	m_fCameraPitch			= 0.0f;
	m_fCameraRoll			= 0.0f;
	m_fRadius				= 0.0f;

	SAFE_ZERO(m_pCamera);
	SAFE_ZERO(m_pMainCamera);
}

Sample::~Sample(void)
{

}
GBASIS_RUN(L"Gurid Test Sample");