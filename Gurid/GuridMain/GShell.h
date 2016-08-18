#pragma once
class GShell : public GBoundingBoxOBB , public GCylinder
{
public:
	bool  m_bPlayer;			//�÷��̾� �߻� �Ѿ�����? if �÷��̾� --> true
	//CBox  m_Box;				//�ٿ�� �ڽ�
	float m_fSpeed;				//��ź ���ǵ�
	float m_fFireTime;			//��ź�� ������ �� ����ð�.
	float m_fEndTime;			//��ź�� �Ҹ�Ǵ� �ð�.
	D3DXVECTOR3 m_vLook;		//Look
	D3DXVECTOR3 m_vPos;			//Position
	D3DXMATRIX  m_matRotation;	//ȸ�����

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
			MessageBox(0, _T("GShell ����"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	GShell();
	GShell(GCar* car, float fFireTime, bool bPlayer);
	virtual ~GShell();
};

