#pragma once
#include "GBASISStd.h"

struct GLightInfo
{
	D3DXVECTOR3				m_vPosition;	// ����Ʈ�� ��ġ
	D3DXVECTOR3				m_vDirection;	// ����Ʈ�� ����
	float					m_fRange;		// ������ ��ȿ �Ÿ�
	float					m_fFalloff;		// ���� �ܰ� �ܺ� ���� �ֵ�( Luminance )
	float					m_fTheta;		// ���� ���� ���� ����		
	float					m_fPhi;			// �ܺ� ���� ���� ����
	D3DXVECTOR4				m_DiffuseColor;
};

class GLightObj : public GLightInfo
{
public:
	D3DXMATRIX				m_matInitWorld;
	D3DXMATRIX				m_matWorld;
	D3DXVECTOR3				m_vInitPosition;// �ʱ� ����Ʈ�� ��ġ��
	D3DXVECTOR3				m_vScale;		// ����Ʈ ������Ʈ�� ũ�⺯ȯ ��
	D3DXVECTOR3				m_vRotation;    // ����Ʈ ������Ʈ�� ȸ��(xyz)
	D3DXVECTOR3				m_vOffset;		// ���� ���̿��� �߰��� ��ġ(x,y,z)

	void					SetValue(D3DXVECTOR3* pvPos = NULL,
		D3DXVECTOR3* pvDir = NULL,
		float* pfRange = NULL,
		D3DXVECTOR4* pvColor = NULL,
		D3DXVECTOR3* pvScale = NULL,
		D3DXVECTOR3* pvRotation = NULL,
		float fTheta = D3DX_PI / 4.0f,
		float fPhi = 5.0f);
public:
	bool					Init();
	bool					Frame(float fElapaseTime = 0.0f, float fSecondPerFrame = 0.0f);
	bool					Render();
	bool					Release();
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	GLightObj(void);
	~GLightObj(void);
};
