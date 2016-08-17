#include "_StdAfx.h"


bool GCar::init(ID3D11Device* pDevice) {

	if (FAILED(m_pBody.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pBody ����"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pHead ����"), _T("Fatal error"), MB_OK);
		return 0;
	}

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/tirebyHand.png")))
		{
			MessageBox(0, _T("m_pTire1 ����"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	if (FAILED(m_pCannon.Create(pDevice, L"../../data/shader/Box.hlsl", L"../../data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pCannon ����"), _T("Fatal error"), MB_OK);
		return 0;
	}

	return true;
}
bool GCar::frame( float fTime, GGuridCamera* mainCamera) {
	D3DXMATRIX matWorld;

	D3DXMATRIX mat_s, mat_t, mat_r_tire_right, mat_r_tire_left, mat_r_z, mat_r_x;
	D3DXMatrixIdentity(&mat_s);
	D3DXMatrixIdentity(&mat_t);
	D3DXMatrixIdentity(&mat_r_x);
	D3DXMatrixIdentity(&mat_r_tire_right);
	D3DXMatrixIdentity(&mat_r_tire_left);
	D3DXMatrixIdentity(&mat_r_z);

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





	static float angle = 0.0f;//for ���� ȸ�� ����
	static float rot_right = 0.0f, rot_left = 0.0f, rot_head = 0.0f;// ���� ȸ�� ����

	m_matWorld_body._11 *= m_fBodyXScale;		// �� ��ü X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;		// �� ��ü Z Scale

	if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle -= 20.0f*fTime));
		rot_right -= 100.0f*fTime;// m_Timer.GetSPF();
		rot_left += 100.0f*fTime;// m_Timer.GetSPF();
	}
	else if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle += 20.0f*fTime));
		rot_right += 100.0f*fTime;// m_Timer.GetSPF();
		rot_left -= 100.0f*fTime;// m_Timer.GetSPF();
	}
	else if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
	{
		//m_matWorld_body._41 += m_vLook.x* 20.0f*fTime;
		//m_matWorld_body._43 += m_vLook.z* 20.0f*fTime;;
		rot_right += 100.0f*fTime;// m_Timer.GetSPF();
		rot_left += 100.0f*fTime;// m_Timer.GetSPF();
	}

	else if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
	{
		rot_right -= 100.0f*fTime;// m_Timer.GetSPF();
		rot_left -= 100.0f*fTime;// m_Timer.GetSPF();
	}
	else if (I_Input.KeyCheck(DIK_Q) == KEY_HOLD)
	{
		//mainCamera->m_vObjectVector[2].x;
		//mainCamera->m_vObjectVector[2].y;
		//mainCamera->m_vObjectVector[2].z;

		rot_head += 100.0f*fTime;// m_Timer.GetSPF();
	}
	else if (I_Input.KeyCheck(DIK_E) == KEY_HOLD)
	{
		//rot -= 100.0f*fTime;// m_Timer.GetSPF();
		rot_head -= 100.0f*fTime;// m_Timer.GetSPF();
	}

	m_vLook.x = m_matRotation._31;
	m_vLook.y = m_matRotation._32;
	m_vLook.z = m_matRotation._33;

	mainCamera->m_vObjectVector[2] = m_vLook;


	mainCamera->Update(fTime);						//ī�޶� ������Ʈ
	matWorld = *mainCamera->GetWorldMatrix();
	matWorld._42 = m_fHeight;//-0.5f;				// �� ��ü ���� ����


	m_matWorld_body *= m_matRotation;
	m_matWorld_body *= matWorld;

	


	D3DXMatrixRotationY(&m_matHeadRotation, D3DXToRadian(-rot_head));

	m_matHeadRotation *= m_matRotation;

	m_vHeadLook.x = m_matHeadRotation._31;
	m_vHeadLook.y = m_matHeadRotation._32;
	m_vHeadLook.z = m_matHeadRotation._33;


	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// �� �Ӹ� X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// �� �Ӹ� Z Scale
	m_matWorld_head *= m_matHeadRotation;
	m_matWorld_head *= matWorld;

	//matWorld2._41 = ;
	m_matWorld_head._42 = m_fHeadHeight;
	//matWorld2._43 += 30.0f;


	D3DXMatrixScaling(&mat_s, m_vTireScale.x, m_vTireScale.y, m_vTireScale.z);
	//D3DXMatrixRotationZ(&temp_r, D3DXToRadian(90.0f));

	D3DXMatrixRotationY(&mat_r_tire_right, D3DXToRadian(-rot_right));
	D3DXMatrixRotationY(&mat_r_tire_left, D3DXToRadian(-rot_left));
	D3DXMatrixRotationZ(&mat_r_z, D3DXToRadian(90.0f));

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		D3DXMatrixTranslation(&mat_t, m_vTirePos[i].x, m_vTirePos[i].y, m_vTirePos[i].z);
		if(i<3){
			m_matWorld_tire[i] = mat_s*mat_r_tire_left*mat_r_z*mat_t*m_matRotation*matWorld;
		}
		else {
			m_matWorld_tire[i] = mat_s*mat_r_tire_right*mat_r_z*mat_t*m_matRotation*matWorld;
		}
	}

	if (m_cartype == TANK) {
		D3DXMatrixScaling(&mat_s, 1.0f, 3.0f, 1.0f);
		D3DXMatrixRotationX(&mat_r_x, D3DXToRadian(90.0f));
		D3DXMatrixTranslation(&mat_t, m_vCannonPos.x, m_vCannonPos.y, m_vCannonPos.z);
		m_matWorld_cannon = mat_s*mat_r_x*mat_t*m_matHeadRotation*matWorld;
	}


	//m_vLook.x = m_matWorld_body._31;
	//m_vLook.y = m_matWorld_body._32;
	//m_vLook.z = m_matWorld_body._33;

	return true;
}
bool GCar::render(ID3D11DeviceContext*    pImmediateContext, GBackViewCamera*			pMainCamera) {

	DX::ApplyDSS(pImmediateContext, DX::GDxState::g_pDSSDepthEnable);
	m_pBody.SetMatrix(&m_matWorld_body, &pMainCamera->m_matView, &pMainCamera->m_matProj);
	m_pBody.Render(pImmediateContext);



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