#pragma once
#include "GMap.h"

#define BOX_CNT 3

struct G_RAY
{
	float			fExtent;
	D3DXVECTOR3		vOrigin;
	D3DXVECTOR3		vDirection;
};

class GuridMain : public GBASISLib_0
{
public:
	GMap			m_CustomMap;
	GCar*						m_pCar[4];	// 0: �¿���, 1 : Ʈ��, 2 : ������
	std::vector<shared_ptr<GShell> >			m_vecShell;	// ��ź.
	shared_ptr<GSkyBox >   		m_pSkyBoxObj;

	GPlaneShape					m_pPlane;
	GDirectionLineShape			m_pDirectionLine;
	//GCamera*					m_pCamera;
	GGuridCamera*					m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// Matrix
	//--------------------------------------------------------------------------------------
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_World[2];
	D3DXMATRIX					m_matWorldPlaneBase;
	//--------------------------------------------------------------------------------------
	// ����Ʈ ����
	//--------------------------------------------------------------------------------------	
	GDxRT						m_ViewPort;
	HRESULT						ScreenViewPort(UINT iWidth, UINT iHeight);
	//--------------------------------------------------------------------------------------
	// ī�޶� 
	//--------------------------------------------------------------------------------------	
	//UINT						m_iCameraType;
	//--------------------------------------------------------------------------------------
	// ī�޶� ȸ�� ����
	//--------------------------------------------------------------------------------------
	float						m_fCameraYaw;
	float						m_fCameraPitch;
	float						m_fCameraRoll;
	float						m_fRadius;
	//OBB
public:
	G_RAY		m_Ray;
	G_BOX		m_Box[3];
	GBoxShape	m_Geom[3];
	GShape*		m_pLine;
//	D3DXMATRIX	m_matWorld;
	D3DXVECTOR3 m_vIntersection;
	D3DXVECTOR3 m_vDxR;

public:
	//OBB
	bool ChkOBBToRay(G_RAY* pRay, G_BOX* pBox);
	bool IntersectBox(G_RAY* pRay, G_BOX* pBox);
	//Default
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		DrawDebug();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	GuridMain(void);
	~GuridMain(void);
};