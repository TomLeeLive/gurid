#include "_StdAfx.h"


GShell::GShell(GCar* car, float fFireTime, bool bPlayer) {
	m_bPlayer = bPlayer;
	m_vLook = car->m_vHeadLook;
	D3DXMATRIX matWorld = car->m_matWorld_cannon;
	m_matRotation = car->m_matHeadRotation;
	//m_matWorld = matWorld;
	m_vPos.x = matWorld._41; m_vPos.y = matWorld._42; m_vPos.z = matWorld._43;


	m_vPos += G_MACRO_CANNON_LENGTH*m_vLook;


	D3DXMatrixIdentity(&m_mat_s);
	D3DXMatrixIdentity(&m_mat_r_x);
	D3DXMatrixScaling(&m_mat_s, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationX(&m_mat_r_x, D3DXToRadian(90.0f));
	m_fSpeed = 50.0f;
	m_fEndTime = 5.0f;
	m_fFireTime = fFireTime;
	Create();
}

GShell::GShell()
{
}


GShell::~GShell()
{
}
