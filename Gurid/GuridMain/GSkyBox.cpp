#include "_StdAfx.h"

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
		loadInfo.Format = /*GSkyBox::*/MAKE_TYPELESS(DXGI_FORMAT_R8G8B8A8_UNORM);
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
	SRVDesc.Format = /*GSkyBox::*/MAKE_SRGB(DXGI_FORMAT_R8G8B8A8_UNORM);
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
		L"data/st00_cm_front.bmp",
		L"data/st00_cm_back.bmp",
		L"data/st00_cm_right.bmp",
		L"data/st00_cm_left.bmp",
		L"data/st00_cm_up.bmp",
		L"data/st00_cm_down.bmp"
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
		L"data/st00_cm_front.bmp",
		L"data/st00_cm_back.bmp",
		L"data/st00_cm_right.bmp",
		L"data/st00_cm_left.bmp",
		L"data/st00_cm_up.bmp",
		L"data/st00_cm_down.bmp"
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
	m_pCubeTexSRV.Attach(DX::CreateShaderResourceView(pDevice, L"data/cube/grassenvmap1024.dds"));
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