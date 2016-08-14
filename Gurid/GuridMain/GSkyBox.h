#pragma once

#define MAX_SKYBOX_TEXTURE 6



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

	////--------------------------------------------------------------------------------------
	//DXGI_FORMAT MAKE_TYPELESS(DXGI_FORMAT format)
	//{
	//	switch (format)
	//	{
	//	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	//	case DXGI_FORMAT_R8G8B8A8_UNORM:
	//	case DXGI_FORMAT_R8G8B8A8_UINT:
	//	case DXGI_FORMAT_R8G8B8A8_SNORM:
	//	case DXGI_FORMAT_R8G8B8A8_SINT:
	//		return DXGI_FORMAT_R8G8B8A8_TYPELESS;

	//	case DXGI_FORMAT_BC1_UNORM_SRGB:
	//	case DXGI_FORMAT_BC1_UNORM:
	//		return DXGI_FORMAT_BC1_TYPELESS;
	//	case DXGI_FORMAT_BC2_UNORM_SRGB:
	//	case DXGI_FORMAT_BC2_UNORM:
	//		return DXGI_FORMAT_BC2_TYPELESS;
	//	case DXGI_FORMAT_BC3_UNORM_SRGB:
	//	case DXGI_FORMAT_BC3_UNORM:
	//		return DXGI_FORMAT_BC3_TYPELESS;
	//	};

	//	return format;
	//}
	//DXGI_FORMAT MAKE_SRGB(DXGI_FORMAT format)
	//{
	//	switch (format)
	//	{
	//	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	//	case DXGI_FORMAT_R8G8B8A8_UNORM:
	//	case DXGI_FORMAT_R8G8B8A8_UINT:
	//	case DXGI_FORMAT_R8G8B8A8_SNORM:
	//	case DXGI_FORMAT_R8G8B8A8_SINT:
	//		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//	case DXGI_FORMAT_BC1_TYPELESS:
	//	case DXGI_FORMAT_BC1_UNORM:
	//		return DXGI_FORMAT_BC1_UNORM_SRGB;
	//	case DXGI_FORMAT_BC2_TYPELESS:
	//	case DXGI_FORMAT_BC2_UNORM:
	//		return DXGI_FORMAT_BC2_UNORM_SRGB;
	//	case DXGI_FORMAT_BC3_TYPELESS:
	//	case DXGI_FORMAT_BC3_UNORM:
	//		return DXGI_FORMAT_BC3_UNORM_SRGB;

	//	};

	//	return format;
	//}

};

#define D3DX_FROM_FILE          ((UINT) -3)
