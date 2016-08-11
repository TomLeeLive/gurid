#include "Sample.h"
#include "GTimer.h"

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
	
    D3DXMatrixIdentity( &m_World[0] );
	D3DXMatrixIdentity( &m_matWorld );
	
	D3DXMATRIX matRotX, matScale;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	D3DXMatrixMultiply(&m_matWorldPlaneBase, &matScale, &matRotX);
	
	// 4 종류의 카메라 세팅
	D3DXVECTOR3 vTargetPosition	= D3DXVECTOR3( 0.0f, -0.1f, 0.0f );
	D3DXVECTOR3 vUpVector( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 vCameraPosition	= D3DXVECTOR3( 0.0f, 1.0f, -0.1f );
	
	SAFE_NEW(m_pCamera[0], GCamera);
	m_pCamera[0]->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);
	// Front g_matView
	SAFE_NEW(m_pCamera[1], GCamera);
	vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_pCamera[1]->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);
	// Side g_matView
	SAFE_NEW(m_pCamera[2], GCamera);
	vCameraPosition = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_pCamera[2]->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);
	// User g_matView
	SAFE_NEW(m_pCamera[3], GCamera);
	vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	m_pCamera[3]->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);

	// 뷰포트에 들어 맞게 카메라 조정.
	m_pCamera[0]->SetObjectView(D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));
	m_pCamera[1]->SetObjectView(D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));
	m_pCamera[2]->SetObjectView(D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));
	m_pCamera[3]->SetObjectView(D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));

	// 투영행렬 세팅
	for (int iCamera = 0; iCamera < 4; iCamera++)
	{
		m_pCamera[iCamera]->SetProjMatrix(D3DX_PI * 0.25f,
			(float)m_ViewPort[iCamera].m_vp.Width / (float)m_ViewPort[iCamera].m_vp.Height,
			1.0f,
			100.0f);
	}

	// 메인 카메라 뷰 행렬 세팅
	SAFE_NEW(m_pMainCamera, GBackViewCamera);
	//m_pMainCamera->SetModelCenter( D3DXVECTOR3( 1.0f, -1.0f, -1.0f ) );
	m_pMainCamera->SetModelCenter(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMainCamera->SetViewMatrix(*m_pCamera[m_iCameraType]->GetEyePt(), *m_pCamera[3]->GetLookAtPt(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pMainCamera->m_vCameraDestination = *m_pCamera[m_iCameraType]->GetEyePt();

	// Setup the camera's projection parameters
	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);
	return true;
}
bool Sample::Frame()
{
	//--------------------------------------------------------------------------------------
	// 카메라 타입 선택
	//--------------------------------------------------------------------------------------
	if( I_Input.KeyCheck( DIK_1 ) && I_Input.KeyCheck( DIK_LCONTROL ) )
	{
		m_iCameraType++;
		if (m_iCameraType > 3)m_iCameraType = 0;
		m_fCameraYaw = m_pCamera[m_iCameraType]->GetYawAngle();
		m_fCameraPitch = m_pCamera[m_iCameraType]->GetPitchAngle();
		m_fRadius = m_pMainCamera->GetRadius();
		m_pMainCamera->SetViewMatrix( *m_pCamera[m_iCameraType]->GetEyePt(), *m_pCamera[m_iCameraType]->GetLookAtPt() );
		m_pMainCamera->SetRadius( m_fRadius );
		m_pMainCamera->m_vCameraDestination = *m_pCamera[m_iCameraType]->GetEyePt();
	}
	
	//--------------------------------------------------------------------------------------
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------
	m_pMainCamera->Update(m_Timer.GetSPF());
	m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));	
	return true;
}
bool Sample::Render()
{
	HRESULT hr;	
	//-----------------------------------------------------------------------
	// 메인 뷰포트
	//-----------------------------------------------------------------------	
	m_pBox.SetMatrix(&m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pBox.Render(m_pImmediateContext);
	m_pPlane.SetMatrix(&m_matWorldPlaneBase, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pPlane.Render(m_pImmediateContext);	
	//-----------------------------------------------------------------------
	// 현재 세팅된 뷰포트 정보 저장
	//-----------------------------------------------------------------------
	D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
    UINT nViewPorts = 1;
    m_pImmediateContext->RSGetViewports( &nViewPorts, vpOld );	
	//-----------------------------------------------------------------------
	// 상단 뷰포트
	//-----------------------------------------------------------------------	
	for( int iVp = 0; iVp < 4 ; iVp++ )
	{
		m_ViewPort[iVp].Apply(m_pImmediateContext, GetRenderTargetView(), GetDepthStencilView());

		m_pBox.SetMatrix(&m_matWorld, &m_pCamera[iVp]->m_matView, &m_pCamera[iVp]->m_matProj);
		m_pBox.Render(m_pImmediateContext);

		m_pPlane.SetMatrix(&m_matWorld, &m_pCamera[iVp]->m_matView, &m_pCamera[iVp]->m_matProj);
		m_pPlane.Render(m_pImmediateContext);
	}
	
	//-----------------------------------------------------------------------
	// 기본 뷰포트 정보로 복원
	//-----------------------------------------------------------------------
	m_pImmediateContext->RSSetViewports(nViewPorts, vpOld);
	
	//-----------------------------------------------------------------------
	// 뷰포트 번호 출력
	//-----------------------------------------------------------------------
	RECT rc;
	for( int iVp = 0; iVp < 4 ; iVp++ )
	{		
		rc.left		= m_ViewPort[iVp].m_vp.TopLeftX + m_ViewPort[iVp].m_vp.Width*0.5f;		
		rc.top		= m_ViewPort[iVp].m_vp.TopLeftY;
		rc.right	= m_ViewPort[iVp].m_vp.Width+rc.left;				    
		rc.bottom	= m_ViewPort[iVp].m_vp.Height+rc.top;		
		T_STR str = CameraViewStyle[iVp];
		TCHAR strNumber[32];
		str += _itow( iVp, strNumber, 10 );// _wtoi
		DrawDebugRect( &rc, const_cast<TCHAR*>(str.c_str()));
	}	
	//-----------------------------------------------------------------------
	// 적용되어 있는 카메라 타입 표시
	//-----------------------------------------------------------------------	
	rc.left		= m_DefaultRT.m_vp.TopLeftX + m_DefaultRT.m_vp.Width*0.5f;
	rc.top		= m_DefaultRT.m_vp.TopLeftY;
	rc.right	= m_DefaultRT.m_vp.Width;
	rc.bottom	= m_DefaultRT.m_vp.Height;
	T_STR str = CameraViewStyle[m_iCameraType];
	TCHAR strNumber[32];
	str += _itow( m_iCameraType, strNumber, 10 );// _wtoi
	DrawDebugRect( &rc, const_cast<TCHAR*>(str.c_str()));
	//-----------------------------------------------------------------------
	// 적용되어 있는 카메라의 방향벡터 표시
	//-----------------------------------------------------------------------
	str.clear();
	TCHAR pBuffer[256];
	memset( pBuffer, 0, sizeof( TCHAR ) * 256 );
	_stprintf_s( pBuffer, _T("Look:%10.4f,%10.4f,%10.4f \n"),	m_pMainCamera->m_vLookVector.x, 
																m_pMainCamera->m_vLookVector.y, 
																m_pMainCamera->m_vLookVector.z);	
	str += pBuffer;

	memset( pBuffer, 0, sizeof( TCHAR ) * 256 );
	_stprintf_s( pBuffer, _T("Up:%10.4f,%10.4f,%10.4f \n"), m_pMainCamera->m_vUpVector.x, 
															m_pMainCamera->m_vUpVector.y,
															m_pMainCamera->m_vUpVector.z);	
	str += pBuffer;

	memset( pBuffer, 0, sizeof( TCHAR ) * 256 );
	_stprintf_s( pBuffer, _T("Right:%10.4f,%10.4f,%10.4f "),	m_pMainCamera->m_vRightVector.x, 
																m_pMainCamera->m_vRightVector.y,
																m_pMainCamera->m_vRightVector.z);	
	str += pBuffer;

	rc.left		= m_DefaultRT.m_vp.TopLeftX + m_DefaultRT.m_vp.Width*0.5f;
	rc.top		= m_DefaultRT.m_vp.Height-75;
	rc.right	= m_DefaultRT.m_vp.Width;
	rc.bottom	= m_DefaultRT.m_vp.Height;
	DrawDebugRect( &rc, const_cast<TCHAR*>(str.c_str()));
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
	SAFE_DEL(m_pCamera[1]);
	SAFE_DEL(m_pCamera[2]);
	SAFE_DEL(m_pCamera[3]);
	SAFE_DEL(m_pMainCamera);
	return true;
}
bool Sample::DrawDebug()
{
	D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT nViewPorts = 1;
	m_pImmediateContext->RSGetViewports(&nViewPorts, vpOld);

	m_ViewPort[0].Apply(m_pImmediateContext, GetRenderTargetView(), GetDepthStencilView());
	m_pDirectionLine.SetMatrix(NULL, &m_pCamera[0]->m_matView, &m_pCamera[0]->m_matProj);
	m_pDirectionLine.Render(m_pImmediateContext);

	m_ViewPort[1].Apply(m_pImmediateContext, GetRenderTargetView(), GetDepthStencilView());
	m_pDirectionLine.SetMatrix(NULL, &m_pCamera[1]->m_matView, &m_pCamera[1]->m_matProj);
	m_pDirectionLine.Render(m_pImmediateContext);

	m_ViewPort[2].Apply(m_pImmediateContext, GetRenderTargetView(), GetDepthStencilView());
	m_pDirectionLine.SetMatrix(NULL, &m_pCamera[2]->m_matView, &m_pCamera[2]->m_matProj);
	m_pDirectionLine.Render(m_pImmediateContext);

	m_ViewPort[3].Apply(m_pImmediateContext, GetRenderTargetView(), GetDepthStencilView());
	m_pDirectionLine.SetMatrix(NULL, &m_pCamera[3]->m_matView, &m_pCamera[3]->m_matProj);
	m_pDirectionLine.Render(m_pImmediateContext);

	//-----------------------------------------------------------------------
	// 기본 뷰포트 정보로 복원
	//-----------------------------------------------------------------------
	m_pImmediateContext->RSSetViewports(nViewPorts, vpOld);
	//-----------------------------------------------------------------------
	// 기본 뷰포트 정보로 복원
	//-----------------------------------------------------------------------
	m_pImmediateContext->RSSetViewports(nViewPorts, vpOld);

	m_matWorld._41 = m_pMainCamera->GetLookAtPt()->x;
	m_matWorld._42 = m_pMainCamera->GetLookAtPt()->y;
	m_matWorld._43 = m_pMainCamera->GetLookAtPt()->z;
	
	m_pDirectionLine.SetMatrix(&m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pDirectionLine.Render(m_pImmediateContext);

	m_pDirectionLine.SetMatrix( &m_World[0], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pDirectionLine.Render(m_pImmediateContext);
	return GBASISLib_0::DrawDebug();
}
HRESULT Sample::ScreenViewPort(UINT iWidth, UINT iHeight)
{	
	HRESULT hr = S_OK;
	
	UINT iRectWidth = iWidth / 5;
	UINT iRectHeight = iHeight / 4;

	m_ViewPort[0].Set(GetDevice(), 0, 0, iRectWidth, iRectHeight, 0.0f, 1.0f);
	m_ViewPort[1].Set(GetDevice(), 0, iRectHeight, iRectWidth, iRectHeight, 0.0f, 1.0f);
	m_ViewPort[2].Set(GetDevice(), 0, iRectHeight * 2, iRectWidth, iRectHeight, 0.0f, 1.0f);
	m_ViewPort[3].Set(GetDevice(), 0, iRectHeight * 3, iRectWidth, iRectHeight, 0.0f, 1.0f);
	
	return hr;
}

Sample::Sample(void)
{	
	// 추가
	m_iCameraType			= 3; // User Camera	
	m_fCameraYaw			= 0.0f;
	m_fCameraPitch			= 0.0f;
	m_fCameraRoll			= 0.0f;
	m_fRadius				= 0.0f;
	SAFE_ZERO(m_pCamera[0]);
	SAFE_ZERO(m_pCamera[1]);
	SAFE_ZERO(m_pCamera[2]);
	SAFE_ZERO(m_pCamera[3]);
	SAFE_ZERO(m_pMainCamera);
}

Sample::~Sample(void)
{

}
GBASIS_RUN(L"Gurid Test Sample");