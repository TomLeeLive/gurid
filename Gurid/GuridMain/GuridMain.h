#pragma once
#include "GMap.h"

#define BOX_CNT 3
#define G_MACRO_MAX_TANK 2

struct G_RAY
{
	float			fExtent;
	D3DXVECTOR3		vOrigin;
	D3DXVECTOR3		vDirection;
};

class GuridMain : public GBASISLib_0
{
public:

	GShellManager				m_ShellManager;
	GMap						m_CustomMap;
	shared_ptr<GCar>			m_pTank[G_MACRO_MAX_TANK];	// 0:플레이어, 그 외에 모두 적

	shared_ptr<GSkyBox >   		m_pSkyBoxObj;

	GPlaneShape					m_pPlane;
	GDirectionLineShape			m_pDirectionLine;
	//GCamera*					m_pCamera;
	GGuridCamera*					m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// Matrix
	//--------------------------------------------------------------------------------------
	//D3DXMATRIX					m_matWorld;
	//D3DXMATRIX					m_World[2];
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
#pragma region OBBOBB
	////OBB
	//bool ChkOBBToRay(G_RAY* pRay, G_BOX* pBox);
	//bool IntersectBox(G_RAY* pRay, G_BOX* pBox);
#pragma endregion	
	void		ColCheck();
	//Default
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
	GuridMain(void);
	~GuridMain(void);
};
