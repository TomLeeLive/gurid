#pragma once
#include "GSprite.h"
#include "GSpriteMgr.h"

#include "GMap.h"
#include "GDirectWrite.h"
#include "GShape.h"

struct G_RAY
{
	float			fExtent;
	D3DXVECTOR3		vOrigin;
	D3DXVECTOR3		vDirection;
};

class GGameSingle
{
public:
	WAVE_STATE		m_nWavePhase;					//Wave ����
	WAVE_STATE		m_nWaveBeforePhase;				//Wave ���� ����
	//HP, �ν���, �Ѿ� ������-----------------------------------------------------------------------
	GPlaneShape					m_pBar[3];
	D3DXMATRIX					m_mPlaneWorld[3];
	D3DXMATRIX					m_mPlaneView;
	D3DXMATRIX					m_mPlaneProj;
	//�ؽ�Ʈ----------------------------------------------------------------------------------------
	GDirectWrite				m_Font;
	ComPtr<IDXGISurface1>		m_pBackBuffer = NULL;
	int							m_iScore;					//���� ����
	int							m_fPlayTime;				//�÷��� Ÿ��(�����ð�) ��¿�
	//----------------------------------------------------------------------------------------------
	GWaveManager				m_WaveManager;				//wave manager
	GCarManager					m_TankManager;
	GShellManager				m_ShellManager;
	GMap						m_CustomMap;
	//shared_ptr<GCar>			m_pTank[G_MACRO_MAX_TANK];	// 0:�÷��̾�, �� �ܿ� ��� ��

	shared_ptr<GSkyBox >   		m_pSkyBoxObj;

	GPlaneShape					m_pPlane;
	GDirectionLineShape			m_pDirectionLine;
	//GCamera*					m_pCamera;
	GGuridCamera*					m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// Matrix
	//--------------------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------------------
	// ����Ʈ
	//-------------------------------------------------------------------------------------
	GPlaneShape             m_BigPlane;
	//shared_ptr<GModelViewCamera > m_pMainCamera;
	GSprite*				m_pSprite[2];
	D3DXMATRIX				m_matPlaneWorld;
	D3DXMATRIX				m_matWorld;
	ComPtr<ID3D11BlendState> m_pBS[4];
	HRESULT SetBlendState();

public:
	G_RAY		m_Ray;
	G_BOX		m_Box[3];
	GBoxShape	m_Geom[3];
	GShape*		m_pLine;
	//	D3DXMATRIX	m_matWorld;
	D3DXVECTOR3 m_vIntersection;
	D3DXVECTOR3 m_vDxR;

public:

	void		ColCheck();
	void		ShellManagerFrame();

	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();


	bool		RenderScene(GSprite* pPlane);

	bool		DrawDebug();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();

	GGameSingle();
	virtual ~GGameSingle();
};

