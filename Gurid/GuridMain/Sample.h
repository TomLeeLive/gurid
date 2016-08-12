#pragma once
#define MAX_SKYBOX_TEXTURE 6
#include "GBasisLib_0.h"
#include "GShape.h"
#include "GBackViewCamera.h"





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

#define G_MACRO_TIRES 6
#define G_MACRO_TANK_TIRES 6

enum cartypes {
	SEDAN = 0,	//세단
	TRUCK = 1,	//트럭
	JEEP = 2,	//지프
	TANK = 3,	//탱크
	CARTYPE_LAST//차량 수
};

class GCar {
public:
	enum tirepos {
		L1 = 0, //앞 왼쪽
		L2 = 1, //앞 오른쪽
		L3 = 2, //뒤 왼쪽
		R1 = 3, //앞 왼쪽
		R2 = 4, //앞 오른쪽
		R3 = 5, //뒤 왼쪽

	};

	D3DXMATRIX m_matRotation;	// 차의 방향(회전)
	UINT  m_cartype;			//차 타입
	float m_fHeight;			// 차 전체 높이 조정
	float m_fBodyXScale;		// 차 몸체 X Scale.
	float m_fBodyZScale;		// 차 몸체 Z Scale
	float m_fHeadXScale;		// 차 머리 X Scale
	float m_fHeadZScale;		// 차 머리 Z Scale
	D3DXVECTOR3	m_vTireScale;    // 타이어 스케일
	D3DXVECTOR3 m_vTirePos[G_MACRO_TIRES];	// 타이어 위치
	D3DXVECTOR3 m_vCannonScale;	//탱크 주포 스케일
	D3DXVECTOR3 m_vCannonPos;	//탱크 주포 위치


	D3DXMATRIX	m_matWorld_body;
	D3DXMATRIX	m_matWorld_head;
	D3DXMATRIX  m_matWorld_tire[G_MACRO_TIRES];
	D3DXMATRIX	m_matWorld_cannon;

	GBoxShape					m_pBody;			//차 바디
	GBoxShape					m_pHead;			//차 헤드
	GCylinder					m_pTire[G_MACRO_TIRES];
	GCylinder					m_pCannon;			//탱크 주포

	virtual bool init(ID3D11Device* pDevice);
	virtual bool frame(D3DXMATRIX matWorld, float fTime, GBackViewCamera* mainCamera);
	virtual bool render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera);
	virtual bool release();


	D3DXMATRIX ObjectLookAt(D3DXVECTOR3& vPosition, D3DXVECTOR3& vTarget, D3DXVECTOR3& vUp)
	{
		D3DXMATRIX out;

		D3DXVECTOR3 vDirection = vTarget - vPosition;
		D3DXVec3Normalize(&vDirection, &vDirection);
		float fDot = D3DXVec3Dot(&vUp, &vDirection);
		D3DXVECTOR3 vUpVector = vUp - (vDirection * fDot);
		D3DXVec3Normalize(&vUpVector, &vUpVector);
		D3DXVECTOR3 vRightVector;
		D3DXVec3Cross(&vRightVector, &vUpVector, &vDirection);

		out._11 = vRightVector.x; out._12 = vRightVector.y;  out._13 = vRightVector.z;
		out._21 = vUpVector.x;  out._22 = vUpVector.y;  out._23 = vUpVector.z;
		out._31 = vDirection.x;  out._32 = vDirection.y;  out._33 = vDirection.z;
		out._41 = vPosition.x;
		out._42 = vPosition.y;
		out._43 = vPosition.z;

		return out;
	}

	GCar(cartypes type) {

		D3DXMatrixIdentity(&m_matRotation);
		if (type == TANK) {
			m_cartype = TANK;
			m_fHeight = 0.5f;			// 차 전체 높이 조정
			m_fBodyXScale = 3.0f;		// 차 몸체 X Scale.
			m_fBodyZScale = 5.0f;		// 차 몸체 Z Scale
			m_fHeadXScale = 2.5f;		// 차 머리 X Scale
			m_fHeadZScale = 2.5f;		// 차 머리 Z Scale

			m_vTirePos[L1] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[L2] = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			m_vTirePos[L3] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[R1] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[R2] = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			m_vTirePos[R3] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
			m_vTireScale = D3DXVECTOR3(3.0f, 0.5f, 3.0f);

			m_vCannonScale = D3DXVECTOR3(1.0f, 3.0f, 1.0f);
			m_vCannonPos = D3DXVECTOR3(0.0f, 2.0f, 3.0f);
		}
		else {
			m_cartype = SEDAN;
			m_fHeight = 0.5f;			// 차 전체 높이 조정
			m_fBodyXScale = 3.0f;		// 차 몸체 X Scale.
			m_fBodyZScale = 5.0f;		// 차 몸체 Z Scale
			m_fHeadXScale = 3.0f;		// 차 머리 X Scale
			m_fHeadZScale = 3.0f;		// 차 머리 Z Scale

			m_vTirePos[L1] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[L2] = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			m_vTirePos[L3] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[R1] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[R2] = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			m_vTirePos[R3] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
			m_vTireScale = D3DXVECTOR3(3.0f, 0.5f, 3.0f);
		}
	};
	GCar() {};
	~GCar() {};
};

bool GCar::init(ID3D11Device* pDevice) {

	if (FAILED(m_pBody.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pBody 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pHead 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/tirebyHand.png")))
		{
			MessageBox(0, _T("m_pTire1 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	if (FAILED(m_pCannon.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pCannon 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	return true;
}
bool GCar::frame(D3DXMATRIX matWorld, float fTime, GBackViewCamera* mainCamera) {

	D3DXMATRIX temp_s, temp_t, temp_r_y, temp_r_z, temp_r_x;
	D3DXMatrixIdentity(&temp_s);
	D3DXMatrixIdentity(&temp_t);
	D3DXMatrixIdentity(&temp_r_x);
	D3DXMatrixIdentity(&temp_r_y);
	D3DXMatrixIdentity(&temp_r_z);

	D3DXMatrixIdentity(&m_matWorld_body);
	D3DXMatrixIdentity(&m_matWorld_head);

	D3DXMATRIX temp_rr;
	D3DXMATRIX temp_rrr;
	D3DXMatrixIdentity(&temp_rrr);
	temp_rr = mainCamera->m_mView;
	//temp_rr._41 = 0, temp_rr._42 = 0, temp_rr._43 = 0;
	D3DXMatrixTranspose(&temp_rr,&temp_rr);
	temp_rrr._11 = temp_rr._11;  temp_rrr._13 = temp_rr._13;
	temp_rrr._31 = temp_rr._31; temp_rrr._33 = temp_rr._33;

	//D3DXMATRIX temp_tt;
	//D3DXMatrixIdentity(&temp_tt);
	//temp_tt._11 = temp_rr._11; temp_tt._12 = ; temp_tt._13 = ;
	//temp_tt._21 = temp_rr._21; temp_tt._22 = ; temp_tt._23 = ;
	//temp_tt._31 = ; temp_tt._32 = ; temp_tt._33 = ;



	matWorld._42 -= m_fHeight;//-0.5f;				// 차 전체 높이 조정


	static float angle = 0.0f;//for 차량 회전 각도
	static float rot = 0.0f;// 바퀴 회전 각도

	m_matWorld_body._11 *= m_fBodyXScale;		// 차 몸체 X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;		// 차 몸체 Z Scale

	if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle -=20.0f*fTime));
	}
	if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle += 20.0f*fTime));
	}
	if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
	{
		rot += 100.0f*fTime;// m_Timer.GetSPF();
	}

	if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
	{
		rot -= 100.0f*fTime;// m_Timer.GetSPF();
	}

	m_matWorld_body *= m_matRotation;
	m_matWorld_body *= matWorld;

	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// 차 머리 X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// 차 머리 Z Scale
	m_matWorld_head *= temp_rrr;
	m_matWorld_head *= matWorld;




	D3DXMatrixScaling(&temp_s, m_vTireScale.x, m_vTireScale.y, m_vTireScale.z);
	//D3DXMatrixRotationZ(&temp_r, D3DXToRadian(90.0f));

	D3DXMatrixRotationY(&temp_r_y, D3DXToRadian(-rot));
	D3DXMatrixRotationZ(&temp_r_z, D3DXToRadian(90.0f));

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		D3DXMatrixTranslation(&temp_t, m_vTirePos[i].x, m_vTirePos[i].y, m_vTirePos[i].z);
		m_matWorld_tire[i] = temp_s*temp_r_y*temp_r_z*temp_t*m_matRotation*matWorld;
	}

	if (m_cartype == TANK){
		D3DXMatrixScaling(&temp_s, 1.0f, 3.0f, 1.0f);
		D3DXMatrixRotationX(&temp_r_x, D3DXToRadian(90.0f));
		D3DXMatrixTranslation(&temp_t, m_vCannonPos.x, m_vCannonPos.y, m_vCannonPos.z);
		m_matWorld_cannon = temp_s*temp_r_x*temp_t*temp_rrr*matWorld;
	}

	return true;
}
bool GCar::render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera) {

	DX::ApplyDSS(pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
	m_pBody.SetMatrix(&m_matWorld_body, &pMainCamera->m_matView, &pMainCamera->m_matProj);
	m_pBody.Render(pImmediateContext);

	//matWorld2._41 = ;
	m_matWorld_head._42 += 2.0f;
	//matWorld2._43 += 30.0f;

	m_pHead.SetMatrix(&m_matWorld_head, &pMainCamera->m_matView, &pMainCamera->m_matProj);
	m_pHead.Render(pImmediateContext);


	for (int i = 0; i < G_MACRO_TIRES; i++) {
		m_pTire[i].SetMatrix(&m_matWorld_tire[i], &pMainCamera->m_matView, &pMainCamera->m_matProj);
		m_pTire[i].Render(pImmediateContext);
	}
	
	if (m_cartype == TANK) {
		m_pCannon.SetMatrix(&m_matWorld_cannon, &pMainCamera->m_matView, &pMainCamera->m_matProj);
		m_pCannon.Render(pImmediateContext);
	}

	return true;
}
bool GCar::release() {
	return true;
}

class GSkyBox : public GBoxShape
{
public:
	ComPtr<ID3D11ShaderResourceView>	m_pTexSRV[MAX_SKYBOX_TEXTURE];
	ComPtr<ID3D11PixelShader>			m_pPSTextrueIndex;
	ComPtr<ID3D11PixelShader>			m_pPSTextureArray;
	ComPtr<ID3D11PixelShader>			m_pPSCubeTexture;
	ComPtr<ID3D11Texture2D>				m_pSkyTexture;
	ComPtr<ID3D11ShaderResourceView>    m_pSkyTexRV;
	ComPtr<ID3D11ShaderResourceView>	m_pCubeTexSRV;
	int									m_iRenderType;
public:
	bool				Create(
		ID3D11Device* pDevice,
		TCHAR* pLoadShaderFile,
		TCHAR* pLoadTextureString = 0);
	bool				Render(ID3D11DeviceContext*    pContext, GBackViewCamera*	pMainCamera);
	HRESULT				LoadTextures(ID3D11Device* pDevice, TCHAR* pLoadTextureString);
	HRESULT				LoadTextureArray(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*    pContext,
		TCHAR** szTextureNames,
		int iNumTextures,
		ID3D11Texture2D** ppTex2D,
		ID3D11ShaderResourceView** ppSRV);
	HRESULT				CreateTextureArray(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext);
	GSkyBox();
	~GSkyBox();
};

#define D3DX_FROM_FILE          ((UINT) -3)
//--------------------------------------------------------------------------------------
DXGI_FORMAT MAKE_TYPELESS(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;

	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_TYPELESS;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_TYPELESS;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_TYPELESS;
	};

	return format;
}
DXGI_FORMAT MAKE_SRGB(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM_SRGB;

	};

	return format;
}

HRESULT GSkyBox::LoadTextureArray(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pContext, TCHAR** szTextureNames, int iNumTextures,
	ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV)
{
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	WCHAR str[MAX_PATH];
	for (int i = 0; i < iNumTextures; i++)
	{
		ID3D11Resource* pRes = NULL;
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
		loadInfo.Width = D3DX11_DEFAULT;
		loadInfo.Height = D3DX11_DEFAULT;
		loadInfo.Depth = D3DX11_DEFAULT;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = 1;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = MAKE_TYPELESS(DXGI_FORMAT_R8G8B8A8_UNORM);
		loadInfo.Filter = D3DX11_FILTER_NONE;
		loadInfo.MipFilter = D3DX11_FILTER_NONE;

		V_RETURN(D3DX11CreateTextureFromFile(pd3dDevice, szTextureNames[i], &loadInfo, NULL, &pRes, NULL));
		if (pRes)
		{
			ID3D11Texture2D* pTemp;
			pRes->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTemp);
			pTemp->GetDesc(&desc);

			if (!(*ppTex2D))
			{
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.ArraySize = iNumTextures;
				V_RETURN(pd3dDevice->CreateTexture2D(&desc, NULL, ppTex2D));
			}

			for (UINT iMip = 0; iMip < desc.MipLevels; iMip++)
			{
				D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
				if (SUCCEEDED(pContext->Map((ID3D11Resource*)pTemp, 0, D3D11_MAP_READ, 0, &MappedFaceDest)))
				{
					UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
					pContext->UpdateSubresource((*ppTex2D),
						D3D10CalcSubresource(iMip, i, desc.MipLevels),
						NULL,
						MappedFaceDest.pData,
						MappedFaceDest.RowPitch,
						0);
					pContext->Unmap(pTemp, 0);
				}
			}

			SAFE_RELEASE(pRes);
			SAFE_RELEASE(pTemp);
		}
		else
		{
			return false;
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = MAKE_SRGB(DXGI_FORMAT_R8G8B8A8_UNORM);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
	SRVDesc.Texture2DArray.ArraySize = iNumTextures;
	V_RETURN(pd3dDevice->CreateShaderResourceView(*ppTex2D, &SRVDesc, ppSRV));
	return hr;
}

bool GSkyBox::Create(ID3D11Device* pDevice, TCHAR* pLoadShaderFile, TCHAR* pLoadTextureString)
{
	if (GShape::Create(pDevice, pLoadShaderFile, pLoadTextureString) == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pPSTextrueIndex.Attach(DX::LoadPixelShaderFile(pDevice, pLoadShaderFile, "PS_TextureIndex"));
	m_pPSTextureArray.Attach(DX::LoadPixelShaderFile(pDevice, pLoadShaderFile, "PS_TextureArray"));
	m_pPSCubeTexture.Attach(DX::LoadPixelShaderFile(pDevice, pLoadShaderFile, "PS_CubeTexture"));
	return true;
}
HRESULT GSkyBox::LoadTextures(ID3D11Device* pDevice, TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;

	TCHAR* g_szSkyTextures[] =
	{
		L"..\\..\\data\\st00_cm_front.bmp",
		L"..\\..\\data\\st00_cm_back.bmp",
		L"..\\..\\data\\st00_cm_right.bmp",
		L"..\\..\\data\\st00_cm_left.bmp",
		L"..\\..\\data\\st00_cm_up.bmp",
		L"..\\..\\data\\st00_cm_down.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);

	for (int iTex = 0; iTex < iNumTexture; iTex++)
	{
		m_pTexSRV[iTex].Attach(DX::CreateShaderResourceView(pDevice, g_szSkyTextures[iTex]));
	}

	return hr;
}

HRESULT GSkyBox::CreateTextureArray(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext)
{
	HRESULT hr = S_OK;

	TCHAR* g_szSkyTextures[] =
	{
		L"..\\..\\data\\st00_cm_front.bmp",
		L"..\\..\\data\\st00_cm_back.bmp",
		L"..\\..\\data\\st00_cm_right.bmp",
		L"..\\..\\data\\st00_cm_left.bmp",
		L"..\\..\\data\\st00_cm_up.bmp",
		L"..\\..\\data\\st00_cm_down.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);
	//--------------------------------------------------------------------------------------
	// 서브리소스를 사용하는 방법
	//--------------------------------------------------------------------------------------
	if (FAILED(hr = LoadTextureArray(pDevice, pContext, g_szSkyTextures, iNumTexture,
		m_pSkyTexture.GetAddressOf(),
		m_pSkyTexRV.GetAddressOf())))
	{
		return hr;
	}
	m_pCubeTexSRV.Attach(DX::CreateShaderResourceView(pDevice, L"../../data/cube/grassenvmap1024.dds"));
	return hr;
}
bool GSkyBox::Render(ID3D11DeviceContext*    pContext, GBackViewCamera*			pMainCamera)
{
	D3DXMatrixScaling(&pMainCamera->m_matWorld, 1.0f, 1.0f, 1.0f);
	D3DXMATRIX matSkyBoxView = pMainCamera->m_matView;
	matSkyBoxView._41 = 0;
	matSkyBoxView._42 = 0;
	matSkyBoxView._43 = 0;

	DX::ApplyDSS(pContext, DX::GDxState::g_pDSSDepthDisable);

	ID3D11SamplerState* ppSamplerStates[2] = { DX::GDxState::g_pSSClampPoint, DX::GDxState::g_pSSWrapLinear };
	pContext->PSSetSamplers(0, 2, ppSamplerStates);

	SetMatrix(&pMainCamera->m_matWorld, &matSkyBoxView, &pMainCamera->m_matProj);

	PreRender(pContext);
	pContext->UpdateSubresource(m_Object.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);

	if (m_iRenderType == 0)
	{
		//--------------------------------------------------------------------------------------
		// 한장씩 랜더링 하는 방법
		//--------------------------------------------------------------------------------------
		for (int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
		{
			if (m_pTexSRV[iTex] == nullptr) break;
			pContext->PSSetShaderResources(0, 1, m_pTexSRV[iTex].GetAddressOf());
			pContext->DrawIndexed(6, 6 * iTex, 0);
		}
	}
	else if (m_iRenderType == 1)
	{
		pContext->PSSetShaderResources(1, 6, m_pTexSRV[0].GetAddressOf());
		pContext->PSSetShader(m_pPSTextrueIndex.Get(), NULL, 0);
		pContext->DrawIndexed(36, 0, 0);
	}
	else if (m_iRenderType == 2)
	{
		pContext->PSSetShaderResources(2, 1, m_pSkyTexRV.GetAddressOf());
		pContext->PSSetShader(m_pPSTextureArray.Get(), NULL, 0);
		pContext->DrawIndexed(36, 0, 0);
	}
	else if (m_iRenderType == 3)
	{
		pContext->PSSetShaderResources(3, 1, m_pCubeTexSRV.GetAddressOf());
		pContext->PSSetShader(m_pPSCubeTexture.Get(), NULL, 0);
		pContext->DrawIndexed(36, 0, 0);
	}
	return true;
}
GSkyBox::GSkyBox()
{
	m_iRenderType = 2;
	m_pSkyTexture = nullptr;
	m_pSkyTexRV = nullptr;
}
GSkyBox::~GSkyBox()
{
}

class Sample : public GBASISLib_0
{
public:
	GCar*						m_pCar[4];// 0: 승용차, 1 : 트럭, 2 : 지프차
	shared_ptr<GSkyBox >   		m_pSkyBoxObj;

	GPlaneShape					m_pPlane;
	GDirectionLineShape			m_pDirectionLine;
	GCamera*					m_pCamera;
	GBackViewCamera*			m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// Matrix
	//--------------------------------------------------------------------------------------
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_World[2];
	D3DXMATRIX					m_matWorldPlaneBase;
	//--------------------------------------------------------------------------------------
	// 뷰포트 설정
	//--------------------------------------------------------------------------------------	
	GDxRT						m_ViewPort;
	HRESULT						ScreenViewPort(UINT iWidth, UINT iHeight);
	//--------------------------------------------------------------------------------------
	// 카메라 
	//--------------------------------------------------------------------------------------	
	//UINT						m_iCameraType;
	//--------------------------------------------------------------------------------------
	// 카메라 회전 정보
	//--------------------------------------------------------------------------------------
	float						m_fCameraYaw;
	float						m_fCameraPitch;
	float						m_fCameraRoll;
	float						m_fRadius;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		DrawDebug();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	Sample(void);
	~Sample(void);
};
