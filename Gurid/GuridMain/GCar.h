#pragma once

#define G_MACRO_TIRES 6
#define G_MACRO_TANK_TIRES 6

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
		L1 = 0, //앞 왼쪽
		L2 = 1, //앞 오른쪽
		L3 = 2, //뒤 왼쪽
		R1 = 3, //앞 왼쪽
		R2 = 4, //앞 오른쪽
		R3 = 5, //뒤 왼쪽

	};

	D3DXMATRIX	m_matRotation;	// 차의 방향(회전)
	//D3DXVECTOR3 m_vRight;		// 차의 방향(Right)
	//D3DXVECTOR3 m_vUp;			// 차의 방향(Up)
	D3DXVECTOR3 m_vLook;		// 차의 방향(look)

	D3DXMATRIX	m_matHeadRotation;	// 머리의 방향(회전)
	//D3DXVECTOR3 m_vHeadRight;		// 차의 방향(Right)
	//D3DXVECTOR3 m_vHeadUp;			// 차의 방향(Up)
	D3DXVECTOR3 m_vHeadLook;		// 차의 방향(look)

	bool  m_bPlayer;			//플레이어 인지?
	UINT  m_cartype;			//차 타입
	float m_fHeight;			// 차 전체 높이 조정
	float m_fHeadHeight;			// 차 머리 높이 조정
	float m_fBodyXScale;		// 차 몸체 X Scale.
	float m_fBodyZScale;		// 차 몸체 Z Scale
	float m_fHeadXScale;		// 차 머리 X Scale
	float m_fHeadZScale;		// 차 머리 Z Scale
	D3DXVECTOR3	m_vTireScale;    // 타이어 스케일
	D3DXVECTOR3 m_vTirePos[G_MACRO_TIRES];	// 타이어 위치
	D3DXVECTOR3 m_vCannonScale;	//탱크 주포 스케일
	D3DXVECTOR3 m_vCannonPos;	//탱크 주포 위치

	D3DXMATRIX m_matWorld;		//적탱크 구현를 위한 월드행렬

	D3DXMATRIX	m_matWorld_body;
	D3DXMATRIX	m_matWorld_head;
	D3DXMATRIX  m_matWorld_tire[G_MACRO_TIRES];
	D3DXMATRIX	m_matWorld_cannon;

	GBoxShape					m_pBody;			//차 바디
	GBoxShape					m_pHead;			//차 헤드
	GCylinder					m_pTire[G_MACRO_TIRES];
	GCylinder					m_pCannon;			//탱크 주포

	bool frame_player(float fTime, GGuridCamera* mainCamera);
	bool frame_enemy(float fTime, GGuridCamera* mainCamera);

	virtual bool init(ID3D11Device* pDevice);
	virtual bool frame( float fTime, GGuridCamera* mainCamera);
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
	void Create(cartypes type) {
		D3DXMatrixIdentity(&m_matWorld);
		if (type == TANK) {

			D3DXMatrixIdentity(&m_matRotation);
			m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			m_matRotation._13 = m_vLook.x; m_matRotation._23 = m_vLook.y; m_matRotation._33 = m_vLook.z;
			//m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			//m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//m_matRotation._11 = m_vRight.x; m_matRotation._12 = m_vUp.x;
			//m_matRotation._21 = m_vRight.y; m_matRotation._22 = m_vUp.y;
			//m_matRotation._31 = m_vRight.z; m_matRotation._32 = m_vUp.z;

			D3DXMatrixIdentity(&m_matHeadRotation);
			m_vHeadLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);



			m_cartype = TANK;
			m_fHeight = 3.5f;			// 차 전체 높이 조정
			m_fHeadHeight = 5.5f;
			m_fBodyXScale = 3.0f;		// 차 몸체 X Scale.
			m_fBodyZScale = 5.0f;		// 차 몸체 Z Scale
			m_fHeadXScale = 2.5f;		// 차 머리 X Scale
			m_fHeadZScale = 2.5f;		// 차 머리 Z Scale

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
			m_fHeight = 0.5f;			// 차 전체 높이 조정
			m_fBodyXScale = 3.0f;		// 차 몸체 X Scale.
			m_fBodyZScale = 5.0f;		// 차 몸체 Z Scale
			m_fHeadXScale = 3.0f;		// 차 머리 X Scale
			m_fHeadZScale = 3.0f;		// 차 머리 Z Scale

			m_vTirePos[L1] = D3DXVECTOR3(-2.0f, -2.0f, 3.0f);
			m_vTirePos[L2] = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			m_vTirePos[L3] = D3DXVECTOR3(-2.0f, -2.0f, -3.0f);
			m_vTirePos[R1] = D3DXVECTOR3(2.0f, -2.0f, 3.0f);
			m_vTirePos[R2] = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			m_vTirePos[R3] = D3DXVECTOR3(2.0f, -2.0f, -3.0f);
			m_vTireScale = D3DXVECTOR3(3.0f, 0.5f, 3.0f);
		}
	}
	GCar(cartypes type) {
		m_bPlayer = false;
		Create(type);
	};
	GCar(cartypes type, bool player) {
		m_bPlayer = player;
		Create(type);
	};
	GCar() {};
	~GCar() {};
};