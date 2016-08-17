#pragma once

class GGuridCamera : public GBackViewCamera
{
public:
	float				fMoveSpeed;
	D3DXMATRIX			Update(float fElapseTime );
	//void				MoveLook(float fValue);
	//void				MovementTarget(float fValue, D3DXVECTOR3 vDirection);

public:
	GGuridCamera(void) {
		fMoveSpeed = 1.0f;
		m_vLerpUpVector = D3DXVECTOR3(0, 1, 0);
		SetOffsetDistance(0.0f, 5.0f, 15.0f);
		m_vModelCenter = D3DXVECTOR3(0, 0, 0);

		m_nRotateModelButtonMask = MOUSE_RIGHT_BUTTON;
		m_nZoomButtonMask = MOUSE_WHEEL;
		m_nRotateCameraButtonMask = MOUSE_LEFT_BUTTON;
	};
	virtual ~GGuridCamera(void) {};
};
