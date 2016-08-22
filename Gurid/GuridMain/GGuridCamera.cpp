#include "_StdAfx.h"


//void GGuridCamera::MoveLook(float fValue)
//{
//	MovementTarget(fValue, m_vObjectVector[2]);
//}
//void GGuridCamera::MovementTarget(float fValue, D3DXVECTOR3 vDirection)
//{
//	m_vTargetPos += vDirection * fValue *2.0f;
//	m_vCameraDestination = m_vTargetPos + (vDirection * GetDecideDirection(fValue*2.0f));
//	m_vCameraDestination.y += m_vTargetPos.y + m_vOffsetDistance.y;
//}

D3DXMATRIX GGuridCamera::Update(float fElapseTime)
{
	if(g_pMain->m_GameSingle.m_nWavePhase != WAVE_ST_GAMEOVER){

		if (g_InputData.bWKey) 	MoveLook(fElapseTime * 2.0f * m_fSpeed);
		if (g_InputData.bSKey)		MoveLook(-fElapseTime * 2.0f * m_fSpeed);
		if (g_InputData.bDKey) {

			//m_fPrevRadius = m_fRadius;
			//m_fRadius += fElapseTime * 2.0f * m_fRadius * 0.1f / 120.0f;

			//MoveSide(fElapseTime * 2.0f); 

		}
		if (g_InputData.bAKey) {

			//m_fPrevRadius = m_fRadius;
			//m_fRadius += (-fElapseTime * 2.0f) * m_fRadius * 0.1f / 120.0f;
			//MoveSide(-fElapseTime * 2.0f);

		}
	}
	//if( g_InputData.bQKey )		MoveUp(   fElapseTime * 2.0f);
	//if( g_InputData.bEKey )		MoveUp(  -fElapseTime * 2.0f);

	// Change the radius from the camera to the model based on wheel scrolling
	if (m_nMouseWheelDelta && m_nZoomButtonMask == MOUSE_WHEEL)
	{
		m_fPrevRadius = m_fRadius;
		m_fRadius += m_nMouseWheelDelta * m_fRadius * 0.1f / 120.0f;
	}
	m_fRadius = __min(m_fMaxRadius, m_fRadius);
	m_fRadius = __max(m_fMinRadius, m_fRadius);


	//// Get the inverse of the arcball's rotation matrix
	D3DXMATRIX mCameraRot;
	D3DXMatrixInverse(&mCameraRot, NULL, m_ViewArcBall.GetRotationMatrix());

	//// Transform vectors based on camera's rotation matrix
	D3DXVECTOR3 vWorldUp, vWorldAhead;
	D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);


	// 카메라 회전이 적용되면 목표 위치를 갱신한다.
	if (m_ViewArcBall.m_bDrag)
	{
		m_vCameraDestination = m_vCameraPos = m_vTargetPos - vWorldAhead * m_fRadius;
	}
	// 카메라의 앵글을 정방향으로 보간한다.
	D3DXVec3Lerp(&m_vLerpUpVector, &vWorldUp, &vLocalUp, min(fElapseTime*3.0f, 1.0f));

	// 휠의 이동변위값을 사용하여 목표위치를 갱신한다.
	if (m_nMouseWheelDelta)
	{
		D3DXVECTOR3 vTmp;
		D3DXVec3Normalize(&vTmp, &(m_vCameraPos - m_vTargetPos));
		m_vCameraDestination += (vTmp * (m_fRadius - m_fPrevRadius));
	}

	// 천천히 따라오는 카메라.	
	D3DXVec3Lerp(&m_vCameraPos,
		&m_vCameraPos,
		&m_vCameraDestination, min(fElapseTime, 1.0f));

	if (!m_ViewArcBall.m_bDrag)
	{
		SetViewMatrix(m_vCameraPos, m_vTargetPos, m_vLerpUpVector);
	}
	//  좌우 앞뒤만 움직인다.
	//else
	//{
	//	// Update the view matrix
	//	D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vTargetPos, &vWorldUp);
	//}
	D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vTargetPos, &vWorldUp);

	D3DXMATRIX mInvView;
	D3DXMatrixInverse(&mInvView, NULL, &m_matView);
	mInvView._41 = mInvView._42 = mInvView._43 = 0;

	D3DXMATRIX mModelLastRotInv;
	D3DXMatrixInverse(&mModelLastRotInv, NULL, &m_mModelLastRot);

	// Accumulate the delta of the arcball's rotation in view space.
	// Note that per-frame delta rotations could be problematic over long periods of time.
	D3DXMATRIX mModelRot;
	mModelRot = *m_WorldArcBall.GetRotationMatrix();
	m_mModelRot *= m_matView * mModelLastRotInv * mModelRot * mInvView;

	m_mCameraRotLast = mCameraRot;
	m_mModelLastRot = mModelRot;

	// Translate the rotation matrix to the same position as the lookAt position
	m_mModelRot._41 = m_vTargetPos.x;
	m_mModelRot._42 = m_vTargetPos.y;
	m_mModelRot._43 = m_vTargetPos.z;

	// Translate world matrix so its at the center of the model
	D3DXMATRIX mTrans;
	D3DXMatrixTranslation(&mTrans, -m_vModelCenter.x, -m_vModelCenter.y, -m_vModelCenter.z);
	m_matWorld = mTrans * m_mModelRot;

	m_vObjectVector[0].x = m_matWorld._11;
	m_vObjectVector[0].y = m_matWorld._21;
	m_vObjectVector[0].z = m_matWorld._31;
	m_vObjectVector[1].x = m_matWorld._12;
	m_vObjectVector[1].y = m_matWorld._22;
	m_vObjectVector[1].z = m_matWorld._32;
	m_vObjectVector[2].x = m_matWorld._13;
	m_vObjectVector[2].y = m_matWorld._23;
	m_vObjectVector[2].z = m_matWorld._33;

	// 마우스 휠값 초기화
	m_nMouseWheelDelta = 0;
	m_fPrevRadius = m_fRadius;
	return UpdateVector();
}
