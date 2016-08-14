#include "_StdAfx.h"


bool GCar::init(ID3D11Device* pDevice) {

	if (FAILED(m_pBody.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pBody 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pHead 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/tirebyHand.png")))
		{
			MessageBox(0, _T("m_pTire1 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	if (FAILED(m_pCannon.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pCannon 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	return true;
}
bool GCar::frame(D3DXMATRIX matWorld, float fTime, GBackViewCamera* mainCamera) {

	D3DXMATRIX temp_s, temp_t, temp_r_y, temp_r_z, temp_r_x, temp_r_head;
	D3DXMatrixIdentity(&temp_s);
	D3DXMatrixIdentity(&temp_t);
	D3DXMatrixIdentity(&temp_r_x);
	D3DXMatrixIdentity(&temp_r_y);
	D3DXMatrixIdentity(&temp_r_z);

	D3DXMatrixIdentity(&m_matWorld_body);
	D3DXMatrixIdentity(&m_matWorld_head);

	//D3DXMATRIX temp_rr;
	//D3DXMATRIX temp_rrr;
	//D3DXMatrixIdentity(&temp_rrr);
	//temp_rr = mainCamera->m_mView;
	////temp_rr._41 = 0, temp_rr._42 = 0, temp_rr._43 = 0;
	//D3DXMatrixTranspose(&temp_rr,&temp_rr);
	//temp_rrr._11 = temp_rr._11;  temp_rrr._13 = temp_rr._13;
	//temp_rrr._31 = temp_rr._31; temp_rrr._33 = temp_rr._33;


	matWorld._42 -= m_fHeight;//-0.5f;				// 차 전체 높이 조정


	static float angle = 0.0f;//for 차량 회전 각도
	static float rot = 0.0f, rot_head = 0.0f;// 바퀴 회전 각도

	m_matWorld_body._11 *= m_fBodyXScale;		// 차 몸체 X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;		// 차 몸체 Z Scale

	if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle -= 20.0f*fTime));
	}
	else if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle += 20.0f*fTime));
	}
	else if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
	{
		rot += 100.0f*fTime;// m_Timer.GetSPF();
	}

	else if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
	{
		rot -= 100.0f*fTime;// m_Timer.GetSPF();
	}
	else if (I_Input.KeyCheck(DIK_Q) == KEY_HOLD)
	{
		rot_head += 100.0f*fTime;// m_Timer.GetSPF();
								 //rot -= 100.0f*fTime;// m_Timer.GetSPF();
	}
	else if (I_Input.KeyCheck(DIK_E) == KEY_HOLD)
	{
		//rot -= 100.0f*fTime;// m_Timer.GetSPF();
		rot_head -= 100.0f*fTime;// m_Timer.GetSPF();
	}

	m_matWorld_body *= m_matRotation;
	m_matWorld_body *= matWorld;


	D3DXMatrixRotationY(&temp_r_head, D3DXToRadian(-rot_head));

	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// 차 머리 X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// 차 머리 Z Scale
	m_matWorld_head *= temp_r_head;
	m_matWorld_head *= matWorld;




	D3DXMatrixScaling(&temp_s, m_vTireScale.x, m_vTireScale.y, m_vTireScale.z);
	//D3DXMatrixRotationZ(&temp_r, D3DXToRadian(90.0f));

	D3DXMatrixRotationY(&temp_r_y, D3DXToRadian(-rot));
	D3DXMatrixRotationZ(&temp_r_z, D3DXToRadian(90.0f));

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		D3DXMatrixTranslation(&temp_t, m_vTirePos[i].x, m_vTirePos[i].y, m_vTirePos[i].z);
		m_matWorld_tire[i] = temp_s*temp_r_y*temp_r_z*temp_t*m_matRotation*matWorld;
	}

	if (m_cartype == TANK) {
		D3DXMatrixScaling(&temp_s, 1.0f, 3.0f, 1.0f);
		D3DXMatrixRotationX(&temp_r_x, D3DXToRadian(90.0f));
		D3DXMatrixTranslation(&temp_t, m_vCannonPos.x, m_vCannonPos.y, m_vCannonPos.z);
		m_matWorld_cannon = temp_s*temp_r_x*temp_t*temp_r_head*matWorld;
	}

	return true;
}
bool GCar::render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera) {

	DX::ApplyDSS(pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
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