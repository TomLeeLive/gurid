#pragma once
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

#define G_MACRO_TIRES 4

enum cartypes {
	SEDAN = 0,	//세단
	TRUCK = 1,	//트럭
	JEEP = 2	//지프
};

class GCar {
public:
	enum tirepos {
		FL = 0, //앞 왼쪽
		FR = 1, //앞 오른쪽
		RL = 2, //뒤 왼쪽
		RR = 3  //뒤 오른쪽
	};

	float m_fHeight;			// 차 전체 높이 조정
	float m_fBodyXScale;		// 차 몸체 X Scale.
	float m_fBodyZScale;		// 차 몸체 Z Scale
	float m_fHeadXScale;		// 차 머리 X Scale
	float m_fHeadZScale;		// 차 머리 Z Scale
	D3DXVECTOR3 m_vTirePos[G_MACRO_TIRES];	// 타이어 위치

	D3DXMATRIX	m_matWorld_body;
	D3DXMATRIX	m_matWorld_head;
	D3DXMATRIX  m_matWorld_tire[G_MACRO_TIRES];

	GBoxShape					m_pBody;			//차 바디
	GBoxShape					m_pHead;			//차 헤드
	GCylinder					m_pTire[4];

	virtual bool init(ID3D11Device* pDevice);
	virtual bool frame(D3DXMATRIX matWorld, float fTime);
	virtual bool render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera);
	virtual bool release();
	GCar(cartypes type) {

		if (type == SEDAN){
			m_fHeight = 0.5f;			// 차 전체 높이 조정
			m_fBodyXScale = 3.0f;		// 차 몸체 X Scale.
			m_fBodyZScale =  5.0f;		// 차 몸체 Z Scale
			m_fHeadXScale = 3.0f;		// 차 머리 X Scale
			m_fHeadZScale = 3.0f;		// 차 머리 Z Scale

			m_vTirePos[FL] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[FR] = D3DXVECTOR3(2.0f, -2.0f, 3.0f );
			m_vTirePos[RL] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[RR] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
		}
	};
	GCar() {};
	~GCar() {};
};

bool GCar::init(ID3D11Device* pDevice) {

	if (FAILED(m_pBody.Create(pDevice, L"data/shader/Box.hlsl", L"_bak/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"data/shader/Box.hlsl", L"_bak/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox2 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}


	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"data/shader/Box.hlsl", L"_bak/tire21.jpg")))
		{
			MessageBox(0, _T("m_pTire1 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	return true;
}
bool GCar::frame(D3DXMATRIX matWorld, float fTime) {

	D3DXMATRIX temp_s, temp_r, temp_t, temp_r_y, temp_r_z;
	D3DXMatrixIdentity(&temp_s);
	D3DXMatrixIdentity(&temp_r);
	D3DXMatrixIdentity(&temp_t);

	matWorld._42 -= m_fHeight;//-0.5f;				// 차 전체 높이 조정
	m_matWorld_body = matWorld;
	m_matWorld_body._11 *= m_fBodyXScale;//3.0f;		// 차 몸체 X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;// 5.0f;		// 차 몸체 Z Scale


	m_matWorld_head = matWorld;
	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// 차 머리 X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// 차 머리 Z Scale


	static float rot = 0.0f;
	rot += 100.0f*fTime;// m_Timer.GetSPF();

	D3DXMatrixScaling(&temp_s, 3.0f, 0.5f, 3.0f);
	//D3DXMatrixRotationZ(&temp_r, D3DXToRadian(90.0f));

	D3DXMatrixRotationY(&temp_r_y, D3DXToRadian(-rot));
	D3DXMatrixRotationZ(&temp_r_z, D3DXToRadian(90.0f));

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		D3DXMatrixTranslation(&temp_t, m_vTirePos[i].x, m_vTirePos[i].y, m_vTirePos[i].z);
		m_matWorld_tire[i] = temp_s*temp_r_y*temp_r_z*temp_t*matWorld;
	}

	return true;
}
bool GCar::render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera){

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

	return true;
}
bool GCar::release() {
	return true;
}


class GuridMain : public GBASISLib_0
{
public:
	GCar*						m_pCar[3];// 0: 승용차, 1 : 트럭, 2 : 지프차

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
	int			WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();		
public:
	GuridMain(void);
	~GuridMain(void);
};
extern GuridMain* g_pMain;