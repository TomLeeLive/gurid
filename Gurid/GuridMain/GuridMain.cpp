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

// bounding box 관련 함수들
D3DXMATRIX* GetBoxTransform(D3DXMATRIX *pMat, CBox* pBox);
void SetBoxTransform(const D3DXMATRIX *pMat, CBox* pBox);
void initBox(CBox *pBox, const D3DXVECTOR3& vecMin, const D3DXVECTOR3& vecMax);
void moveBox(CBox *pBox, const D3DXMATRIX& mat);


D3DXMATRIX* GetBoxTransform(D3DXMATRIX *pMat, CBox* pBox)
{
	int i, j;
	real fMat[16];
	pBox->GetTransform(fMat);
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			(*pMat)(i, j) = fMat[i * 4 + j];
	return pMat;
}

void SetBoxTransform(const D3DXMATRIX* pMat, CBox* pBox)
{
	int i, j;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j)
			pBox->axis[i][j] = (*pMat)(i, j);
		pBox->translation[i] = (*pMat)(3, i);
	}
}

void initBox(CBox *pBox, const D3DXVECTOR3& vecMin, const D3DXVECTOR3& vecMax)
{
	pBox->center[0] = (vecMin.x + vecMax.x) / 2.0F;
	pBox->center[1] = (vecMin.y + vecMax.y) / 2.0F;
	pBox->center[2] = (vecMin.z + vecMax.z) / 2.0F;

	pBox->extent[0] = vecMax.x - pBox->center[0];
	pBox->extent[1] = vecMax.y - pBox->center[1];
	pBox->extent[2] = vecMax.z - pBox->center[2];
	// identity world coordinate axis
	pBox->axis[0][0] = 1.0F; pBox->axis[0][1] = 0.0F; pBox->axis[0][2] = 0.0F;
	pBox->axis[1][0] = 0.0F; pBox->axis[1][1] = 1.0F; pBox->axis[1][2] = 0.0F;
	pBox->axis[2][0] = 0.0F; pBox->axis[2][1] = 0.0F; pBox->axis[2][2] = 1.0F;
	pBox->translation[0] = 0.0F; pBox->translation[1] = 0.0F; pBox->translation[2] = 0.0F;
}

void moveBox(CBox *pBox, const D3DXMATRIX& mat)
{
	D3DXMATRIX matBox;
	// 박스의 transform을 가져온다.
	GetBoxTransform(&matBox, pBox);
	// 박스의 transform을 바꾼다.
	matBox *= mat;
	SetBoxTransform(&matBox, pBox);
	// 박스의 center 좌표도 바꾼다.
	D3DXVECTOR3 vecCenter(pBox->center[0], pBox->center[1], pBox->center[2]);
	D3DXVec3TransformCoord(&vecCenter, &vecCenter, &mat);
	pBox->center[0] = vecCenter.x; pBox->center[1] = vecCenter.y; pBox->center[2] = vecCenter.z;
}
//
////Axis-aligned bounding box(AABB )
//bool GuridMain::IntersectBox(G_RAY* pRay, G_BOX* pBox)
//{
//	D3DXVECTOR3 tmin;
//	tmin.x = (pBox->vMin.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f;
//	tmin.y = (pBox->vMin.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
//	tmin.z = (pBox->vMin.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;
//
//	D3DXVECTOR3 tmax;
//	tmax.x = (pBox->vMax.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f;
//	tmax.y = (pBox->vMax.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
//	tmax.z = (pBox->vMax.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;
//
//	D3DXVECTOR3 real_min;
//	real_min.x = min(tmin.x, tmax.x);
//	real_min.y = min(tmin.y, tmax.y);
//	real_min.z = min(tmin.z, tmax.z);
//	D3DXVECTOR3 real_max;
//	real_max.x = max(tmin.x, tmax.x);
//	real_max.y = max(tmin.y, tmax.y);
//	real_max.z = max(tmin.z, tmax.z);
//
//	float minmax = min(min(real_max.x, real_max.y), real_max.z);
//	float maxmin = max(max(real_min.x, real_min.y), real_min.z);
//
//	if (minmax >= maxmin)
//	{
//		m_vIntersection = pRay->vOrigin + pRay->vDirection* maxmin;
//		return true;
//	}
//	return false;
//}
////Oriented bounding box(OBB : RAY)
//bool GuridMain::ChkOBBToRay(G_RAY* pRay, G_BOX* pBox)
//{
//	float  f[3], fa[3], s[3], sa[3];
//	D3DXVECTOR3 vDir = pRay->vDirection;// * 1000.0f;
//	D3DXVECTOR3 vR = pRay->vOrigin - pBox->vCenter;
//	f[0] = D3DXVec3Dot(&vDir, &pBox->vAxis[0]);
//	s[0] = D3DXVec3Dot(&vR, &pBox->vAxis[0]);
//	fa[0] = fabs(f[0]);
//	sa[0] = fabs(s[0]);
//
//	if (sa[0] > pBox->fExtent[0] && s[0] * f[0] >= 0.0f)
//		return false;
//
//	f[1] = D3DXVec3Dot(&vDir, &pBox->vAxis[1]);
//	s[1] = D3DXVec3Dot(&vR, &pBox->vAxis[1]);
//	fa[1] = fabs(f[1]);
//	sa[1] = fabs(s[1]);
//	if (sa[1] > pBox->fExtent[1] && f[1] * s[1] >= 0.0f)
//		return false;
//
//	f[2] = D3DXVec3Dot(&vDir, &pBox->vAxis[2]);
//	s[2] = D3DXVec3Dot(&vR, &pBox->vAxis[2]);
//	fa[2] = fabs(f[2]);
//	sa[2] = fabs(s[2]);
//	if (sa[2] > pBox->fExtent[2] && f[2] * s[2] >= 0.0f)
//		return false;
//
//	float  fCross[3], fRhs;
//	D3DXVECTOR3 vDxR;
//	D3DXVec3Cross(&vDxR, &pRay->vDirection, &vR);
//	// D X pBox->vAxis[0]  분리축
//	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
//	float fAxis2 = pBox->fExtent[1] * fa[2];
//	float fAxis1 = pBox->fExtent[2] * fa[1];
//	fRhs = fAxis2 + fAxis1;
//	if (fCross[0] > fRhs)
//	{
//		m_vDxR = vDxR;
//		return false;
//	}
//	// D x pBox->vAxis[1]  분리축
//	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
//	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
//	if (fCross[1] > fRhs)
//	{
//		m_vDxR = vDxR;
//		return false;
//	}
//	// D x pBox->vAxis[2]  분리축
//	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
//	fRhs = pBox->fExtent[0] * fa[1] + pBox->fExtent[1] * fa[0];
//	if (fCross[2] > fRhs)
//	{
//		m_vDxR = vDxR;
//		return false;
//	}
//
//	return true;
//}


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

	//if (FAILED(m_pPlane.Create(GetDevice(), L"../../data/shader/Plane.hlsl", L"../../data/grids.jpg")))
	//{
	//	MessageBox(0, _T("m_pPlane 실패"), _T("Fatal error"), MB_OK);
	//	return 0;
	//}


	//D3DXMatrixIdentity(&m_World[0]);
	//D3DXMatrixIdentity(&m_matWorld);

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
	if (m_pSkyBoxObj->Create(GetDevice(), L"SkyBox.hlsl") == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	m_pSkyBoxObj->CreateTextureArray(GetDevice(), GetContext());

	//for (int i = 0; i < CARTYPE_LAST; i++){
	//	m_pCar[i]->init(GetDevice());
	//}
	m_pTank->init(GetDevice());
	m_pEnemy->init(GetDevice());

	GMapDesc MapDesc = { 5, 5, 10.0f, L"../../data/Map.png", L"CustomizeMap.hlsl" };
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
	// 엔진에 있는 뷰 및 투영 행렬 갱신
	//--------------------------------------------------------------------------------------

	//m_pMainCamera->Update(m_Timer.GetSPF());
	//m_matWorld = *m_pMainCamera->GetWorldMatrix();//(const_cast< D3DXMATRIX* > (m_pMainCamera->GetWorldMatrix()));
	
	m_pTank->frame( m_Timer.GetSPF(), m_pMainCamera);
	m_pEnemy->frame(m_Timer.GetSPF(), m_pMainCamera);

	m_CustomMap.Frame();
	
	m_ShellManager.frame(m_pTank.get(),&m_Timer, m_pMainCamera);

	
	return true;
}
bool GuridMain::Render()
{
	HRESULT hr;
	

	m_pSkyBoxObj->SetMatrix(0, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_pSkyBoxObj->Render(m_pImmediateContext, m_pMainCamera);

	m_pTank->render(m_pImmediateContext, m_pMainCamera);
	m_pEnemy->render(m_pImmediateContext, m_pMainCamera);

	//m_pPlane.SetMatrix(&m_matWorldPlaneBase, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	//m_pPlane.Render(m_pImmediateContext);
	DX::ApplyDSS(m_pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
	DX::ApplyBS(m_pImmediateContext, DX::GDxState::g_pAlphaBlend);
	m_CustomMap.SetMatrix(m_pMainCamera->GetWorldMatrix(), m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	m_CustomMap.Render(m_pImmediateContext);


	m_ShellManager.render();

#pragma region OBBOBB
	//D3DXMATRIX matWorld, matRot, matScale;
	//for (int iObj = 0; iObj < 3; iObj++)
	//{
	//	D3DXMatrixIdentity(&matWorld);
	//	D3DXMatrixIdentity(&matScale);
	//	D3DXMatrixIdentity(&matRot);

	//	m_pLine->SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	//	/*m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[0], m_Geom[iObj].m_Box.vPos[1], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[1], m_Geom[iObj].m_Box.vPos[2], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[2], m_Geom[iObj].m_Box.vPos[3], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[3], m_Geom[iObj].m_Box.vPos[0], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );

	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[4], m_Geom[iObj].m_Box.vPos[5], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[5], m_Geom[iObj].m_Box.vPos[6], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[6], m_Geom[iObj].m_Box.vPos[7], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	m_pLine->Draw(	m_Geom[iObj].m_Box.vPos[7], m_Geom[iObj].m_Box.vPos[4], D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	//	*/

	//	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	// OBB
	//	D3DXVECTOR3 vCenter = (m_Geom[iObj].m_Box.vMax + m_Geom[iObj].m_Box.vMin) / 2.0f;
	//	D3DXMatrixScaling(&matScale, m_Geom[iObj].m_Box.fExtent[0],
	//		m_Geom[iObj].m_Box.fExtent[1],
	//		m_Geom[iObj].m_Box.fExtent[2]);
	//	/*matRot._11 = m_Geom[iObj].m_Box.vAxis[0].x;
	//	matRot._12 = m_Geom[iObj].m_Box.vAxis[0].y;
	//	matRot._13 = m_Geom[iObj].m_Box.vAxis[0].z;
	//	matRot._21 = m_Geom[iObj].m_Box.vAxis[1].x;
	//	matRot._22 = m_Geom[iObj].m_Box.vAxis[1].y;
	//	matRot._23 = m_Geom[iObj].m_Box.vAxis[1].z;
	//	matRot._31 = m_Geom[iObj].m_Box.vAxis[2].x;
	//	matRot._32 = m_Geom[iObj].m_Box.vAxis[2].y;
	//	matRot._33 = m_Geom[iObj].m_Box.vAxis[2].z;	*/

	//	memcpy(matRot.m[0], &m_Geom[iObj].m_Box.vAxis[0], sizeof(float) * 3);
	//	memcpy(matRot.m[1], &m_Geom[iObj].m_Box.vAxis[1], sizeof(float) * 3);
	//	memcpy(matRot.m[2], &m_Geom[iObj].m_Box.vAxis[2], sizeof(float) * 3);

	//	matWorld = matScale * matRot;
	//	// obb	
	//	memcpy(matWorld.m[3], &m_Box[iObj].vCenter, sizeof(float) * 3);

	//	/*matWorld._41 = m_Box[iObj].vCenter.x;
	//	matWorld._42 = m_Box[iObj].vCenter.y;
	//	matWorld._43 = m_Box[iObj].vCenter.z;*/
	//	m_Geom[iObj].SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	m_Geom[iObj].SetTechnique(m_Geom[iObj].m_pTechnique);
	//	m_Geom[iObj].SetMatrix(&matWorld, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	//	m_Geom[iObj].Render();

	//	// AABB
	//	D3DXMatrixScaling(&matScale, m_Geom[iObj].m_Box.vMax.x - vCenter.x,
	//		m_Geom[iObj].m_Box.vMax.y - vCenter.y,
	//		m_Geom[iObj].m_Box.vMax.z - vCenter.z);
	//	matScale._41 = m_Geom[iObj].m_Box.vCenter.x;
	//	matScale._42 = m_Geom[iObj].m_Box.vCenter.y;
	//	matScale._43 = m_Geom[iObj].m_Box.vCenter.z;
	//	m_Geom[iObj].SetAmbientColor(0.5f, 0.0f, 0.0f, 0.1f);
	//	m_Geom[iObj].SetTechnique(m_Geom[iObj].m_pTechniqueOnlyAmbientColor);
	//	m_Geom[iObj].SetMatrix(&matScale, m_pMainCamera->GetViewMatrix(), m_pMainCamera->GetProjMatrix());
	//	m_Geom[iObj].Render();

	//	if (I_Input.m_MouseState.rgbButtons[0])
	//	{
	//		/*			T_BOX box = m_Geom[iObj].m_Box;
	//		T_RAY ray = m_Ray;
	//		D3DXMATRIX matInverse;
	//		D3DXMatrixInverse(& matInverse, 0, &matRot);
	//		D3DXVec3TransformCoord( &box.vAxis[0], &box.vAxis[0], &matInverse );
	//		D3DXVec3TransformCoord( &box.vAxis[1], &box.vAxis[1], &matInverse );
	//		D3DXVec3TransformCoord( &box.vAxis[2], &box.vAxis[2], &matInverse );

	//		D3DXVec3TransformCoord( &ray.vOrigin, &ray.vOrigin, &matInverse );
	//		D3DXVec3TransformNormal(&ray.vDirection, &ray.vDirection, &matInverse );
	//		*/// RAY : OBB 충돌
	//		if (ChkOBBToRay(&m_Ray, &m_Geom[iObj].m_Box))
	//			//if( IntersectBox( &ray, &box ) )
	//		{
	//			TCHAR szBuffer[256];
	//			_stprintf_s(szBuffer, _T(" %d번 박스와 직선의 충돌, 교점=(%10.4f, %10.4f, %10.4f)"),
	//				iObj, m_vIntersection.x, m_vIntersection.y, m_vIntersection.z);
	//			MessageBox(0, szBuffer, _T("충돌"), MB_OK);
	//		}
	//	}
	//}
#pragma endregion

	//D3DXVECTOR3 vDir = m_vDxR;
	//m_pLine->Draw(D3DXVECTOR3(0, 0, 0), vDir, D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

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
	//for (int i = 0; i < CARTYPE_LAST; i++) {
	//	SAFE_DEL(m_pCar[i]);
	//}
	m_pTank.reset();
	m_pEnemy.reset();

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

	//m_pCar[SEDAN] = new GCar(SEDAN);
	//m_pCar[TANK] = new GCar(TANK);
	//m_pCar[TRUCK] = new GCar(TRUCK);
	//m_pCar[JEEP] = new GCar(JEEP);
	m_pTank = make_shared<GCar>(TANK,true);
	m_pEnemy = make_shared<GCar>(TANK);

	// 추가
	m_fCameraYaw = 0.0f;
	m_fCameraPitch = 0.0f;
	m_fCameraRoll = 0.0f;
	m_fRadius = 0.0f;



	//SAFE_ZERO(m_pCamera);
	SAFE_ZERO(m_pMainCamera);
}

GuridMain::~GuridMain(void)
{

}
GBASIS_RUN(Gurid v0.0.1);
