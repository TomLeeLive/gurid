#pragma once
#include "GImeUI.h"
#include "GShape.h"
using namespace DX;

enum GUI_TYPE {
	T_BUTTON = 0,
	T_EDIT,
	T_LISTBOX,
	T_STATITEXT,
};

class GControlUI
{
public:	
	GShape*     m_pShape;
	T_STR		m_Text;
	PCT_VERTEX  m_Vertices[4];
	RECT		m_rtSize[8];
	RECT		m_rt;
	D3DXMATRIX	m_matViewPort;
	D3DXMATRIX  m_matWorld;
	D3DXMATRIX  m_matView;
	D3DXMATRIX  m_matProj;
	D3DXVECTOR3  m_vScale;
	D3DXVECTOR3  m_vRotate;
	D3DXVECTOR3  m_vTrans;
	UINT		 m_iWidthVP;
	UINT		 m_iHeightVP;
public: //event
	virtual void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	virtual void		SetAmbientColor(float fR, float fG, float fB, float fA);
	virtual bool		Init();
	virtual HRESULT		Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile = nullptr,
							const TCHAR* pLoadTextureString = nullptr) 	{return S_OK;};
	virtual HRESULT		CreateResource(int iRectWidth, int iRectHeight);
	virtual bool		Set(UINT iWidth, UINT iHeight);
	virtual void		Move(float x, float y, float z);
	virtual void		Scale(float x, float y, float z);
	virtual void		Rotate(float x, float y, float z);
	virtual bool		Update();
	virtual bool		Begin(ID3D11DeviceContext* pContext);
	virtual bool		End(ID3D11DeviceContext* pContext);
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		Release();
public:
	GControlUI();
	virtual ~GControlUI();
};
class GButtonCtl : public GControlUI
{
public:
	GBoxShape     m_Box;
	HRESULT		Create(ID3D11Device* pDevice,
		const TCHAR* pLoadShaderFile=nullptr,
		const TCHAR* pLoadTextureString = nullptr);
public:
	GButtonCtl() {};
	virtual ~GButtonCtl() {};
};

class GStaticTextCtl : public GButtonCtl
{
public:
	GPlaneShape   m_Plane;
	GStaticTextCtl() {};
	virtual ~GStaticTextCtl() {};
};

class GEditCtl : public GButtonCtl
{
public:
	GPlaneShape   m_Plane;
	T_STR    m_CompString;
	T_STR    m_CompWord;
	T_STR    m_CurrentString;
public:
	HRESULT		Create(ID3D11Device* pDevice,
		const TCHAR* pLoadShaderFile = nullptr,
		const TCHAR* pLoadTextureString = nullptr);
	GEditCtl() {};
	virtual ~GEditCtl() {};
};

class GListBoxCtl : public GButtonCtl
{
public:
	std::vector<GButtonCtl*>  m_ListBox;
	GListBoxCtl() {};
	virtual ~GListBoxCtl() {};
};
class GImageCtl : public GButtonCtl
{
public:
	std::vector<GButtonCtl*>  m_ListBox;
	HRESULT		Create(ID3D11Device* pDevice,
		const TCHAR* pLoadShaderFile = nullptr,
		const TCHAR* pLoadTextureString = nullptr);
	GImageCtl() {};
	virtual ~GImageCtl() {};
};
class GPanel // 1개의 배경에서 뜨는 대화상자 개념.
{
public:
	std::vector<GButtonCtl*>   m_ButtonCtl;
	std::vector<GImageCtl*>   m_ImageCtl;
	std::vector<GEditCtl*>   m_EditCtl;
	std::vector<GListBoxCtl*>   m_BoxCtl;
	std::vector<GStaticTextCtl*>   m_StaticTextCtl;
	GPanel() {};
	virtual ~GPanel() {};
};
class GScene // 배경 화면이 교체 되는 단위
{
public:
	std::vector<GPanel*>  m_SceneList;
	std::vector<GImageCtl*>   m_ImageCtl;
	std::vector<GButtonCtl*>   m_ButtonCtl;
	std::vector<GEditCtl*>   m_EditCtl;
	std::vector<GListBoxCtl*>   m_listBoxCtl;
	std::vector<GStaticTextCtl*>   m_StaticTextCtl;
	GScene() {};
	virtual ~GScene() {};
};