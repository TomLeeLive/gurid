#include "_StdAfx.h"



bool GCar::init(ID3D11Device* pDevice) {

	if (FAILED(m_pBody.Create(pDevice, L"data/shader/Box.hlsl", L"data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pBody 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(m_pHead.Create(pDevice, L"data/shader/Box.hlsl", L"data/bodybyHand.png")))
	{
		MessageBox(0, _T("m_pHead 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		if (FAILED(m_pTire[i].Create(pDevice, L"data/shader/Box.hlsl", L"data/tirebyHand.png")))
		{
			MessageBox(0, _T("m_pTire1 실패"), _T("Fatal error"), MB_OK);
			return 0;
		}
	}

	if (FAILED(m_pCannon.Create(pDevice, L"data/shader/Box.hlsl", L"data/cannon.png")))
	{
		MessageBox(0, _T("m_pCannon 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	return true;
}
bool GCar::frame_player(float fTime, GGuridCamera* mainCamera) {
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

	static float fSpeed = G_MACRO_CAR_SPEED;//차량 이동 스피드
	static float angle = 0.0f;//for 차량 회전 각도
	static float rot_right = 0.0f, rot_left = 0.0f, rot_head = 0.0f;// 바퀴 회전 각도

	//float 누적발생에 대한 오차를 줄이기 위해..
	if (angle > 360.0f || angle < -360.0f) {
		angle = 0.0f; D3DXMatrixIdentity(&m_matRotation);; m_vLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	if (rot_right > 360.0f || rot_right < -360.0f)
		rot_right = 0.0f;
	if (rot_left > 360.0f || rot_left < -360.0f)
		rot_left = 0.0f;
	if (rot_head > 360.0f || rot_head < -360.0f) {
		rot_head = 0.0f; D3DXMatrixIdentity(&m_matHeadRotation); m_vHeadLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_matWorld_body._11 *= m_fBodyXScale;		// 차 몸체 X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;		// 차 몸체 Z Scale

	if (g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER) {

		if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
		{
			if (mainCamera->m_fSpeed == G_MACRO_CAR_MAX_SPEED) { g_pMain->m_pSound.Play(SMD_TANKFASTMOVE, true, true); }
			else { g_pMain->m_pSound.Play(SND_TANKMOVE, true, true); }

			D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle -= 20.0f*fTime*fSpeed));
			rot_right += 100.0f*fTime;// m_Timer.GetSPF();
			rot_left -= 100.0f*fTime;// m_Timer.GetSPF();
		}
		else if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
		{
			if (mainCamera->m_fSpeed == G_MACRO_CAR_MAX_SPEED) { g_pMain->m_pSound.Play(SMD_TANKFASTMOVE, true, true); }
			else { g_pMain->m_pSound.Play(SND_TANKMOVE, true, true); }

			D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle += 20.0f*fTime*fSpeed));
			rot_right -= 100.0f*fTime;// m_Timer.GetSPF();
			rot_left += 100.0f*fTime;// m_Timer.GetSPF();
		}
		else if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
		{
			if (mainCamera->m_fSpeed == G_MACRO_CAR_MAX_SPEED) { g_pMain->m_pSound.Play(SMD_TANKFASTMOVE, true, true); }
			else { g_pMain->m_pSound.Play(SND_TANKMOVE, true, true); }
			//m_matWorld_body._41 += m_vLook.x* 20.0f*fTime;
			//m_matWorld_body._43 += m_vLook.z* 20.0f*fTime;;
			rot_right -= 100.0f*fTime;// m_Timer.GetSPF();
			rot_left -= 100.0f*fTime;// m_Timer.GetSPF();
		}
		else if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
		{
			if (mainCamera->m_fSpeed == G_MACRO_CAR_MAX_SPEED) { g_pMain->m_pSound.Play(SMD_TANKFASTMOVE, true, true); }
			else { g_pMain->m_pSound.Play(SND_TANKMOVE, true, true); }
			rot_right += 100.0f*fTime;// m_Timer.GetSPF();
			rot_left += 100.0f*fTime;// m_Timer.GetSPF();
		}
		else {
			g_pMain->m_pSound.Stop(SMD_TANKFASTMOVE);
			g_pMain->m_pSound.Stop(SND_TANKMOVE);
		}
		if (I_Input.KeyCheck(DIK_Q) == KEY_HOLD)
		{
			g_pMain->m_pSound.Play(SND_TANKHEADROT, true, true);
			rot_head += 100.0f*fTime;// m_Timer.GetSPF();
		}
		else if (I_Input.KeyCheck(DIK_E) == KEY_HOLD)
		{
			g_pMain->m_pSound.Play(SND_TANKHEADROT, true, true);
			rot_head -= 100.0f*fTime;// m_Timer.GetSPF();
		}
		else {
			g_pMain->m_pSound.Stop(SND_TANKHEADROT);
		}

		if (I_Input.KeyCheck(DIK_LSHIFT) == KEY_HOLD)
		{
			mainCamera->m_fSpeed = fSpeed = G_MACRO_CAR_MAX_SPEED;
		}
		else {
			mainCamera->m_fSpeed = fSpeed = G_MACRO_CAR_SPEED;
		}
	}

	m_vLook.x = m_matRotation._31;
	m_vLook.y = m_matRotation._32;
	m_vLook.z = m_matRotation._33;

	mainCamera->m_vObjectVector[2] = m_vLook;


	mainCamera->Update(fTime);						//카메라 업데이트
	matWorld = *mainCamera->GetWorldMatrix();
	matWorld._42 = m_fHeight;//-0.5f;				// 차 전체 높이 조정


	m_matWorld_body *= m_matRotation;
	m_matWorld_body *= matWorld;




	D3DXMatrixRotationY(&m_matHeadRotation, D3DXToRadian(-rot_head));

	m_matHeadRotation *= m_matRotation;

	m_vHeadLook.x = m_matHeadRotation._31;
	m_vHeadLook.y = m_matHeadRotation._32;
	m_vHeadLook.z = m_matHeadRotation._33;


	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// 차 머리 X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// 차 머리 Z Scale
	m_matWorld_head *= m_matHeadRotation;
	m_matWorld_head *= matWorld;

	//matWorld2._41 = ;
	m_matWorld_head._42 = m_fHeadHeight;
	//matWorld2._43 += 30.0f;


	D3DXMatrixScaling(&mat_s, m_vTireScale.x, m_vTireScale.y, m_vTireScale.z);
	//D3DXMatrixRotationZ(&temp_r, D3DXToRadian(90.0f));

	D3DXMatrixRotationY(&mat_r_tire_right, D3DXToRadian(rot_right*fSpeed));
	D3DXMatrixRotationY(&mat_r_tire_left, D3DXToRadian(rot_left*fSpeed));
	D3DXMatrixRotationZ(&mat_r_z, D3DXToRadian(90.0f));

	for (int i = 0; i < G_MACRO_TIRES; i++) {
		D3DXMatrixTranslation(&mat_t, m_vTirePos[i].x, m_vTirePos[i].y, m_vTirePos[i].z);
		if (i<3) {
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
bool GCar::frame_enemy(float fTime, GGuridCamera* cam,D3DXVECTOR3 vPlayerPos){

	vPlayerPos.y = 0;
	D3DXVECTOR3 vLook_toPlayer;
	D3DXMATRIX mat_r_toPlayer;
	D3DXMatrixIdentity(&mat_r_toPlayer);



	//D3DXMATRIX matWorld;

	D3DXMATRIX mat_s, mat_t, mat_r_tire_right, mat_r_tire_left, mat_r_z, mat_r_x;
	D3DXMatrixIdentity(&mat_s);
	D3DXMatrixIdentity(&mat_t);
	D3DXMatrixIdentity(&mat_r_x);
	D3DXMatrixIdentity(&mat_r_tire_right);
	D3DXMatrixIdentity(&mat_r_tire_left);
	D3DXMatrixIdentity(&mat_r_z);

	D3DXMatrixIdentity(&m_matWorld_body);
	D3DXMatrixIdentity(&m_matWorld_head);


	static float angle = 0.0f;//for 차량 회전 각도
	static float rot_right = 0.0f, rot_left = 0.0f, rot_head = 0.0f;// 바퀴 회전 각도

	//float 누적발생에 대한 오차를 줄이기 위해..
	if (angle > 360.0f || angle < -360.0f) {
		angle = 0.0f; D3DXMatrixIdentity(&m_matRotation);; m_vLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	if (rot_right > 360.0f || rot_right < -360.0f)
		rot_right = 0.0f;
	if (rot_left > 360.0f || rot_left < -360.0f)
		rot_left = 0.0f;
	if (rot_head > 360.0f || rot_head < -360.0f) {
		rot_head = 0.0f; D3DXMatrixIdentity(&m_matHeadRotation); m_vHeadLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_matWorld_body._11 *= m_fBodyXScale;		// 차 몸체 X Scale.
	m_matWorld_body._33 *= m_fBodyZScale;		// 차 몸체 Z Scale

	//바퀴 회전을 위해
	if (g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_READY && g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER) {
		rot_right += 100.0f*fTime;
		rot_left += 100.0f*fTime;
	}
/*
	if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle -= 20.0f*fTime));
		rot_right -= 100.0f*fTime;
		rot_left += 100.0f*fTime;
	}
	else if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
	{
		D3DXMatrixRotationY(&m_matRotation, D3DXToRadian(angle += 20.0f*fTime));
		rot_right += 100.0f*fTime;
		rot_left -= 100.0f*fTime;
	}
	else if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
	{
		rot_right += 100.0f*fTime;
		rot_left += 100.0f*fTime;
	}

	else if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
	{
		rot_right -= 100.0f*fTime;
		rot_left -= 100.0f*fTime;
	}
	else if (I_Input.KeyCheck(DIK_Q) == KEY_HOLD)
	{
		rot_head += 100.0f*fTime;
	}
	else if (I_Input.KeyCheck(DIK_E) == KEY_HOLD)
	{
		rot_head -= 100.0f*fTime;
	}
*/
	m_vLook.x = m_matRotation._31;
	m_vLook.y = m_matRotation._32;
	m_vLook.z = m_matRotation._33;

	//mainCamera->m_vObjectVector[2] = m_vLook;
	//mainCamera->Update(fTime);				

	//matWorld = m_matWorld;//*mainCamera->GetWorldMatrix();
	m_matWorld._42 = m_fHeight;//-0.5f;				// 차 전체 높이 조정


	//플레이어 방향으로 회전을 하기 위해서..
	D3DXVECTOR3 vPos = D3DXVECTOR3(m_matWorld._41, 0.0f, m_matWorld._43);
	vLook_toPlayer = vPlayerPos - vPos;
	D3DXVec3Normalize(&vLook_toPlayer, &vLook_toPlayer);
	D3DXVECTOR3 vRight, vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&vRight, &vUp, &vLook_toPlayer);
	D3DXVec3Cross(&vUp, &vLook_toPlayer, &vRight);

	mat_r_toPlayer._11 = vRight.x;		    mat_r_toPlayer._12 = vRight.y;			mat_r_toPlayer._13 = vRight.z;
	mat_r_toPlayer._21 = vUp.x;				mat_r_toPlayer._22 = vUp.y;				mat_r_toPlayer._23 = vUp.z;
	mat_r_toPlayer._31 = vLook_toPlayer.x;  mat_r_toPlayer._32 = vLook_toPlayer.y;  mat_r_toPlayer._33 = vLook_toPlayer.z;
	m_matWorld_body *= mat_r_toPlayer;


	//이동
	if (g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_READY && g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER) {
		D3DXVECTOR3 temp = D3DXVECTOR3(m_matWorld._41, m_matWorld._42, m_matWorld._43);//D3DXVECTOR3((*_F)->m_matWorld._41, (*_F)->m_matWorld._42, (*_F)->m_matWorld._43);
		temp = temp + m_fSpeed*fTime*vLook_toPlayer;
		m_matWorld._41 = temp.x; m_matWorld._42 = temp.y; m_matWorld._43 = temp.z;
	}
	//m_matWorld_body *= m_matRotation;
	m_matWorld_body *= m_matWorld;

	


	D3DXMatrixRotationY(&m_matHeadRotation, D3DXToRadian(-rot_head));

	m_matHeadRotation *= mat_r_toPlayer;

	m_vHeadLook.x = m_matHeadRotation._31;
	m_vHeadLook.y = m_matHeadRotation._32;
	m_vHeadLook.z = m_matHeadRotation._33;


	m_matWorld_head._11 *= m_fHeadXScale;// 3.0f;		// 차 머리 X Scale
	m_matWorld_head._33 *= m_fHeadZScale;// 3.0f;		// 차 머리 Z Scale
	m_matWorld_head *= m_matHeadRotation;
	m_matWorld_head *= m_matWorld;

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
		if (i<3) {
			m_matWorld_tire[i] = mat_s*mat_r_tire_left*mat_r_z*mat_t*mat_r_toPlayer*m_matWorld;
		}
		else {
			m_matWorld_tire[i] = mat_s*mat_r_tire_right*mat_r_z*mat_t*mat_r_toPlayer*m_matWorld;
		}
	}

	if (m_cartype == TANK) {
		D3DXMatrixScaling(&mat_s, 1.0f, 3.0f, 1.0f);
		D3DXMatrixRotationX(&mat_r_x, D3DXToRadian(90.0f));
		D3DXMatrixTranslation(&mat_t, m_vCannonPos.x, m_vCannonPos.y, m_vCannonPos.z);
		m_matWorld_cannon = mat_s*mat_r_x*mat_t*m_matHeadRotation*m_matWorld;
	}
	
		


	return true;
}
bool GCar::frame( float fTime, GGuridCamera* mainCamera,D3DXVECTOR3 vPlayerPos) {
	
	if (m_bPlayer == true) {
		frame_player(fTime, mainCamera);
	}
	else {
		frame_enemy(fTime, mainCamera, vPlayerPos);
	}

	initBox(this, D3DXVECTOR3(-7, -7, -7), D3DXVECTOR3(7, 7, 7));
	moveBox(this, m_matWorld_head);

	return true;
}
bool GCar::render(ID3D11DeviceContext*    pImmediateContext, GGuridCamera*			pMainCamera) {

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