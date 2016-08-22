#pragma once



enum cartypes {
	SEDAN = 0,	//����
	TRUCK = 1,	//Ʈ��
	JEEP = 2,	//����
	TANK = 3,	//��ũ
	CARTYPE_LAST//���� ��
};

class GCar : public GBoundingBoxOBB
{
public:
	enum tirepos {
		L1 = 0, //�� ����
		L2 = 1, //�� ������
		L3 = 2, //�� ����
		R1 = 3, //�� ����
		R2 = 4, //�� ������
		R3 = 5, //�� ����

	};
	
	D3DXMATRIX	m_matRotation;	// ���� ����(ȸ��)
	D3DXVECTOR3 m_vLook;		// ���� ����(look)
	//D3DXVECTOR3 m_vRight;		// ���� ����(Right)
	//D3DXVECTOR3 m_vUp;			// ���� ����(Up)

	D3DXMATRIX	m_matHeadRotation;	// �Ӹ��� ����(ȸ��)
	D3DXVECTOR3 m_vHeadLook;		// ���� ����(look)
	//D3DXVECTOR3 m_vHeadRight;		// ���� ����(Right)
	//D3DXVECTOR3 m_vHeadUp;			// ���� ����(Up)

	float m_fSpeed;
	float m_fFireTime;			//��ź �߻��� ��Ÿ��(���� �߻������ �ð�)	
	float m_fCoolTime;			//������ ��ź �߻�� �ð�
	int	  m_iHP;				//�� ü��
	int   m_iBoost;				//�� �ν�Ʈ
	int	  m_iShell;				//�߻� ���� źȯ ��
	bool  m_bPlayer;			//�÷��̾� ����?
	UINT  m_cartype;			//�� Ÿ��
	float m_fHeight;			// �� ��ü ���� ����
	float m_fHeadHeight;			// �� �Ӹ� ���� ����
	float m_fBodyXScale;		// �� ��ü X Scale.
	float m_fBodyZScale;		// �� ��ü Z Scale
	float m_fHeadXScale;		// �� �Ӹ� X Scale
	float m_fHeadZScale;		// �� �Ӹ� Z Scale
	D3DXVECTOR3	m_vTireScale;    // Ÿ�̾� ������
	D3DXVECTOR3 m_vTirePos[G_MACRO_TIRES];	// Ÿ�̾� ��ġ
	D3DXVECTOR3 m_vCannonScale;	//��ũ ���� ������
	D3DXVECTOR3 m_vCannonPos;	//��ũ ���� ��ġ

	D3DXMATRIX m_matWorld;		//����ũ ������ ���� �������

	D3DXMATRIX	m_matWorld_body;
	D3DXMATRIX	m_matWorld_head;
	D3DXMATRIX  m_matWorld_tire[G_MACRO_TIRES];
	D3DXMATRIX	m_matWorld_cannon;

	GBoxShape					m_pBody;			//�� �ٵ�
	GBoxShape					m_pHead;			//�� ���
	GCylinder					m_pTire[G_MACRO_TIRES];
	GCylinder					m_pCannon;			//��ũ ����

	bool frame_player(float fTime, GGuridCamera* mainCamera);
	bool frame_enemy(float fTime, GGuridCamera* mainCamera, D3DXVECTOR3 vPlayerPos);

	virtual bool init(ID3D11Device* pDevice);
	virtual bool frame( float fTime, GGuridCamera* mainCamera, D3DXVECTOR3 vPlayerPos);
	virtual bool render(ID3D11DeviceContext*    pImmediateContext, GGuridCamera*			pMainCamera);
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
	void Create(cartypes type) {
		m_fSpeed	= 3.0f;
		m_fFireTime = 0.0f;
		m_fCoolTime = 0.5f;
		m_iHP		= 100;
		m_iBoost	= 100;
		m_iShell    = 10;

		//D3DXMatrixIdentity(&m_matRotation);
		//D3DXMatrixIdentity(&m_matHeadRotation);
		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixIdentity(&m_matWorld_body);
		D3DXMatrixIdentity(&m_matWorld_head);
		for (int i = 0; i < G_MACRO_TIRES; i++)
			D3DXMatrixIdentity(&m_matWorld_tire[G_MACRO_TIRES]);

		D3DXMatrixIdentity(&m_matWorld_cannon);

		if(m_bPlayer== false){
			int iX = (rand() % 200) -100;
			int iZ = (rand() % 200) -100;

			if (iX >= 0)
				iX+= 300;
			if (iX < 0)
				iX -= 300;

			if (iZ >= 0)
				iZ += 300;
			if (iZ < 0)
				iZ -= 300;

			D3DXMatrixTranslation(&m_matWorld, float(iX), 0.0f, float(iZ));
		}

		if (type == TANK) {

			D3DXMatrixIdentity(&m_matRotation);
			m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//m_matRotation._31 = m_vLook.x; m_matRotation._32 = m_vLook.y; m_matRotation._33 = m_vLook.z;

			D3DXMatrixIdentity(&m_matHeadRotation);
			m_vHeadLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			//m_matHeadRotation._31 = m_vHeadLook.x; m_matHeadRotation._32 = m_vHeadLook.y; m_matHeadRotation._33 = m_vHeadLook.z;


			m_cartype = TANK;
			m_fHeight = 3.5f;			// �� ��ü ���� ����
			m_fHeadHeight = 5.5f;
			m_fBodyXScale = 3.0f;		// �� ��ü X Scale.
			m_fBodyZScale = 5.0f;		// �� ��ü Z Scale
			m_fHeadXScale = 2.5f;		// �� �Ӹ� X Scale
			m_fHeadZScale = 2.5f;		// �� �Ӹ� Z Scale

			m_vTirePos[L1] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[L2] = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			m_vTirePos[L3] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[R1] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[R2] = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			m_vTirePos[R3] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
			m_vTireScale = D3DXVECTOR3(3.0f, 0.5f, 3.0f);

			m_vCannonScale = D3DXVECTOR3(1.0f, 3.0f, 1.0f);
			m_vCannonPos = D3DXVECTOR3(0.0f, 2.0f, 3.0f);
		}
		else {
			m_cartype = SEDAN;
			m_fHeight = 0.5f;			// �� ��ü ���� ����
			m_fBodyXScale = 3.0f;		// �� ��ü X Scale.
			m_fBodyZScale = 5.0f;		// �� ��ü Z Scale
			m_fHeadXScale = 3.0f;		// �� �Ӹ� X Scale
			m_fHeadZScale = 3.0f;		// �� �Ӹ� Z Scale

			m_vTirePos[L1] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[L2] = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			m_vTirePos[L3] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[R1] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[R2] = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			m_vTirePos[R3] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
			m_vTireScale = D3DXVECTOR3(3.0f, 0.5f, 3.0f);
		}
	}
	GCar(cartypes type, bool player = false) {
		m_bPlayer = player;
		Create(type);
	};
	GCar() {};
	~GCar() {};
};