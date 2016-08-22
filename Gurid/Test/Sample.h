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
	SEDAN = 0,	//����
	TRUCK = 1,	//Ʈ��
	JEEP = 2	//����
};

class GCar {
public:
	enum tirepos {
		FL = 0, //�� ����
		FR = 1, //�� ������
		RL = 2, //�� ����
		RR = 3  //�� ������
	};

	float m_fHeight;			// �� ��ü ���� ����
	float m_fBodyXScale;		// �� ��ü X Scale.
	float m_fBodyZScale;		// �� ��ü Z Scale
	float m_fHeadXScale;		// �� �Ӹ� X Scale
	float m_fHeadZScale;		// �� �Ӹ� Z Scale
	D3DXVECTOR3 m_vTirePos[G_MACRO_TIRES];	// Ÿ�̾� ��ġ

	D3DXMATRIX	m_matWorld_body;
	D3DXMATRIX	m_matWorld_head;
	D3DXMATRIX  m_matWorld_tire[G_MACRO_TIRES];

	GBoxShape					m_pBody;			//�� �ٵ�
	GBoxShape					m_pHead;			//�� ���
	GCylinder					m_pTire[4];

	virtual bool init(ID3D11Device* pDevice);
	virtual bool frame(D3DXMATRIX matWorld, float fTime);
	virtual bool render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera);
	virtual bool release();
	GCar(cartypes type) {

		if (type == SEDAN){
			m_fHeight = 0.5f;			// �� ��ü ���� ����
			m_fBodyXScale = 3.0f;		// �� ��ü X Scale.
			m_fBodyZScale =  5.0f;		// �� ��ü Z Scale
			m_fHeadXScale = 3.0f;		// �� �Ӹ� X Scale
			m_fHeadZScale = 3.0f;		// �� �Ӹ� Z Scale

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
		MessageBox(0, _T("m_pBox ����"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"data/shader/Box.hlsl", L"_bak/checker_with_numbers.bmp")))
	{
		MessageBox(0, _T("m_pBox2 ����"), _T("Fatal error"), MB_OK);
		return 0;
	}


	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"data/shader/Box.hlsl", L"_bak/tire21.jpg")))
		{
			MessageBox(0, _T("m_pTire1 ����"), _T("Fatal error"), MB_OK);
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

	matWorld._42 -= m_fHeight;//-0.5f;				// �� ��ü ���� ����
	m_matWorld_body = matWorld;
	m_matWorld_body._11 *= m_fBodyXScale;//3.0f;		// �� ��ü X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;// 5.0f;		// �� ��ü Z Scale


	m_matWorld_head = matWorld;
	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// �� �Ӹ� X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// �� �Ӹ� Z Scale


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
	GCar*						m_pCar[3];// 0: �¿���, 1 : Ʈ��, 2 : ������

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
public:	
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		DrawDebug();
	int			WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();		
public:
	GuridMain(void);
	~GuridMain(void);
};
extern GuridMain* g_pMain;