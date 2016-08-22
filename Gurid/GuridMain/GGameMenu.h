#pragma once

struct SimpleVertex
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};

class GGameMenu
{
public:
	VS_CONSTANT_BUFFER			m_cbData;
	std::vector<GControlUI*>	m_pUIList;
	GControlUI*					m_pSelectPlane;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV[3];
	GScene						m_Scene;
public:
	virtual void		SoundPlay();
	virtual void		KeyCheck();

	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	GControlUI* AddRect(GUI_TYPE type);
	GControlUI* SelectRect();

	GGameMenu();
	virtual ~GGameMenu();
};

