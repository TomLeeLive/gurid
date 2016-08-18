#pragma once
class GShell : public GBoundingBoxOBB , public GCylinder
{
public:
	bool  m_bPlayer;			//플레이어 발사 총알인지? if 플레이어 --> true
	//CBox  m_Box;				//바운딩 박스
	float m_fSpeed;				//포탄 스피드
	float m_fFireTime;			//포탄이 생성된 후 경과시간.
	float m_fEndTime;			//포탄이 소멸되는 시간.
	D3DXVECTOR3 m_vLook;		//Look
	D3DXVECTOR3 m_vPos;			//Position
	D3DXMATRIX  m_matRotation;	//회전행렬

	D3DXMATRIX m_mat_s, m_mat_r_x;
	//D3DXVECTOR3 m_vCannonScale;

	bool BoundingBoxUpdate(D3DXMATRIX matWorld) {
		initBox(this, D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1));
		moveBox(this, matWorld);

		return true;
	}

	bool frame() {

	}
	bool render() {

	}
	bool Create() {

		if (FAILED(GCylinder::Create(g_pd3dDevice, L"data/shader/Box.hlsl", L"data/bullet.png")))
		{
			MessageBox(0, _T("GShell 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	GShell();
	GShell(GCar* car, float fFireTime, bool bPlayer);
	virtual ~GShell();
};

