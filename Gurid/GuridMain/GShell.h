#pragma once
class GShell : public GCylinder
{
public:
	float m_fSpeed;				//포탄 스피드
	float m_fFireTime;			//포탄이 생성된 후 경과시간.
	float m_fEndTime;			//포탄이 소멸되는 시간.
	D3DXVECTOR3 m_vLook;		//Look
	D3DXVECTOR3 m_vPos;			//Position
	D3DXMATRIX  m_matRotation;	//회전행렬

	D3DXMATRIX m_mat_s, m_mat_r_x;
	D3DXVECTOR3 m_vCannonScale;

	bool frame() {

	}
	bool render() {

	}
	bool Create() {

		if (FAILED(GCylinder::Create(g_pd3dDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
		{
			MessageBox(0, _T("GShell 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	GShell();
	//GShell(D3DXVECTOR3 vLook) {
	//	D3DXMatrixScaling(&m_mat_s, 1.0f, 3.0f, 1.0f);
	//	D3DXMatrixRotationX(&m_mat_r_x, D3DXToRadian(90.0f));
	//	m_fSpeed = 50.0f;
	//	m_vLook = vLook;
	//	Create();
	//}


	//GShell(D3DVECTOR vLook, D3DXMATRIX matWorld) {
	//	//m_matWorld = matWorld;
	//	m_vPos.x = matWorld._41; m_vPos.y = matWorld._42; m_vPos.z = matWorld._43;
	//	m_vLook = vLook;

	//	D3DXVECTOR3 temp = m_vPos;
	//	temp = temp + 5.0f*m_vLook;
	//	m_vPos = temp;


	//	D3DXMatrixIdentity(&m_mat_s);
	//	D3DXMatrixIdentity(&m_mat_r_x);
	//	D3DXMatrixScaling(&m_mat_s, 1.0f, 1.0f, 1.0f);
	//	D3DXMatrixRotationX(&m_mat_r_x, D3DXToRadian(90.0f));
	//	m_fSpeed = 50.0f;
	//	
	//	Create();
	//}

	GShell(GCar* car, float fFireTime) {
		m_vLook = car->m_vHeadLook;
		D3DXMATRIX matWorld = car->m_matWorld_cannon;
		m_matRotation = car->m_matHeadRotation;
		//m_matWorld = matWorld;
		m_vPos.x = matWorld._41; m_vPos.y = matWorld._42; m_vPos.z = matWorld._43;

		D3DXVECTOR3 temp = m_vPos;
		temp = temp + 5.0f*m_vLook;
		m_vPos = temp;


		D3DXMatrixIdentity(&m_mat_s);
		D3DXMatrixIdentity(&m_mat_r_x);
		D3DXMatrixScaling(&m_mat_s, 1.0f, 1.0f, 1.0f);
		D3DXMatrixRotationX(&m_mat_r_x, D3DXToRadian(90.0f));
		m_fSpeed = 50.0f;
		m_fEndTime = 5.0f;
		m_fFireTime = fFireTime;
		Create();
	}
	virtual ~GShell();
};

