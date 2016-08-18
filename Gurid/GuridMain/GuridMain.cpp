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

// ������ �޼��� 
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
		MessageBox(0, _T("m_pDirectionLIne ����"), _T("Fatal error"), MB_OK);
		return 0;
	}

	D3DXMATRIX matRotX, matScale;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	D3DXMatrixMultiply(&m_matWorldPlaneBase, &matScale, &matRotX);

	// ī�޶� ����
	D3DXVECTOR3 vTargetPosition = D3DXVECTOR3(0.0f, -0.1f, 0.0f);
	D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -0.1f);

	//SAFE_NEW(m_pCamera, GCamera);
	// ���� ī�޶� �� ��� ����
	SAFE_NEW(m_pMainCamera, GGuridCamera);
	vCameraPosition = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	m_pMainCamera->SetViewMatrix(vCameraPosition, vTargetPosition, vUpVector);

	// ����Ʈ�� ��� �°� ī�޶� ����.
	m_pMainCamera->SetObjectView(D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(-2.0f, -2.0f, -2.0f));

	// ������� ����
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
		MessageBox(0, _T("m_pDirectionLIne ����"), _T("Fatal error"), MB_OK);
		return 0;
	}

	m_pSkyBoxObj->CreateTextureArray(GetDevice(), GetContext());


	for (int i = 0; i < G_MACRO_MAX_TANK; i++) {
		m_pTank[i]->init(GetDevice());
	}


	GMapDesc MapDesc = { 100, 100, 10.0f, L"data/pull.jpg", L"data/shader/CustomizeMap.hlsl" };
	m_CustomMap.Init(GetDevice(), m_pImmediateContext);
	if (FAILED(m_CustomMap.Load(MapDesc)))
	{
		return false;
	}

	return true;
}
bool GuridMain::Frame()
{
	//--------------------------------------------------------------------------------------
	// ������ �ִ� �� �� ���� ��� ����
	//--------------------------------------------------------------------------------------

	//m_pMainCamera->Update(m_Timer.GetSPF());
	//m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));
	

	for (int i = 0; i < G_MACRO_MAX_TANK; i++) {
		//m_pTank[i].reset();
		m_pTank[i]->frame(m_Timer.GetSPF(), m_pMainCamera);
	}


	m_CustomMap.Frame();
	

	for (int i = 0; i < G_MACRO_MAX_TANK; i++) {
		m_ShellManager.frame(m_pTank[i].get(), &m_Timer, m_pMainCamera);
	}



	ColCheck();

	
	return true;
}
void GuridMain::ColCheck() {
	vector<shared_ptr<GShell>>::iterator _F = m_ShellManager.m_vecShell.begin();
	vector<shared_ptr<GShell>>::iterator _L = m_ShellManager.m_vecShell.end();

	
	for (; _F != _L; ++_F)
	{
		// Collision Detection Test!
		for (int i = 0; i < G_MACRO_MAX_TANK; i++) {
			int nRet = BoxBoxIntersectionTest(*(m_pTank[i].get()), *(*_F));
			if (nRet == 1) {
				// ������ collision handling�� �� �� �ְ�����, ������ �ܼ��� ���߱�

				(*_F).reset();//delete (*_F);
				*_F = 0;
				break;
			}
		}
	}

	_F = m_ShellManager.m_vecShell.begin();
	while (_F != m_ShellManager.m_vecShell.end())
	{
		if (*_F == 0) {

			_F = m_ShellManager.m_vecShell.erase(_F);
		}
		else {
			_F++;
		}
	}


}
bool GuridMain::Render()
{
	HRESULT hr;
	
	m_pSkyBoxObj->SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_pSkyBoxObj->Render(m_pImmediateContext, m_pMainCamera);

	for (int i = 0; i < G_MACRO_MAX_TANK; i++) {
		m_pTank[i]->render(m_pImmediateContext, m_pMainCamera);
	}

	//DX::ApplyDSS(m_pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
	//DX::ApplyBS(m_pImmediateContext, DX::GDxState::g_pAlphaBlend);
	m_CustomMap.SetMatrix(m_pMainCamera->GetWorldMatrix(), m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_CustomMap.Render(m_pImmediateContext);

	//�Ѿ� ������
	m_ShellManager.render();

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

	// �Ʒ��� ���� GetDevice()->ClearState();�� ȣ������ ���� �� �߻��Ѵ�.
	//D3D10: INFO: ID3D11Device::RSSetState: The currently bound RasterizerState is being deleted; 
	//so naturally, will no longer be bound. [ STATE_SETTING INFO #46: RSSETSTATE_UNBINDDELETINGOBJECT ]
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}
bool GuridMain::Release()
{

	for (int i = 0; i < G_MACRO_MAX_TANK; i++) {
		m_pTank[i].reset();
	}

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

GuridMain::GuridMain(void)
{
	m_iScore = 0;
	m_fPlayTime = 0.0f;

	//m_pCar[SEDAN] = new GCar(SEDAN);
	//m_pCar[TANK] = new GCar(TANK);
	//m_pCar[TRUCK] = new GCar(TRUCK);
	//m_pCar[JEEP] = new GCar(JEEP);

	m_pTank[0] = make_shared<GCar>(TANK,true);
	for (int i = 1; i < G_MACRO_MAX_TANK; i++){
		m_pTank[i] = make_shared<GCar>(TANK);
	}

	// �߰�
	m_fCameraYaw = 0.0f;
	m_fCameraPitch = 0.0f;
	m_fCameraRoll = 0.0f;
	m_fRadius = 0.0f;

	SAFE_ZERO(m_pMainCamera);
}

GuridMain::~GuridMain(void)
{

}
GBASIS_RUN(Gurid v0.0.1);
