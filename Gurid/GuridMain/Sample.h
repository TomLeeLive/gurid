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
#define G_MACRO_TANK_TIRES 8

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
		FL = 0, //앞 왼쪽
		FR = 1, //앞 오른쪽
		RL = 2, //뒤 왼쪽
		RR = 3  //뒤 오른쪽
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
	GCylinder					m_pTire[4];
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

			m_vTirePos[FL] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[FR] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[RL] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[RR] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
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

			m_vTirePos[FL] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[FR] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[RL] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[RR] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
			m_vTireScale = D3DXVECTOR3(3.0f, 0.5f, 3.0f);
		}
	};
	GCar() {};
	~GCar() {};
};

bool GCar::init(ID3D11Device* pDevice) {

	if (FAILED(m_pBody.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox2 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}


	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/checker_with_numbers.bmp")))
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


class Sample : public GBASISLib_0
{
public:
	GCar*						m_pCar[CARTYPE_LAST];// 0: 승용차, 1 : 트럭, 2 : 지프차

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
