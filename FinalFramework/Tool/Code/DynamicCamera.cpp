#include "stdafx.h"
#include "DynamicCamera.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
	ZeroMemory(&m_vAngle, sizeof(_vec3));
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_Camera()
{
	Reset_Camera();
	Update_EyeAtUp();
	Update_RightLook();
		
	return S_OK;
}

_int CDynamicCamera::Update_Camera(const _float& fTimeDelta)
{
	Update_EyeAtUp();
	Update_RightLook();
	_int iExit = CCamera::Update_Camera(0.f);

	return iExit;
}

HRESULT CDynamicCamera::Reset_Camera()
{
	m_fFovY = D3DXToRadian(60.f);
	m_fAspect = (_float)WINCX / (_float)WINCY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	m_vEye = { 0.f, 5.f, -5.f };
	m_vAngle = { D3DXToRadian(45.f), 0.f, 0.f };

	return S_OK;
} 

void CDynamicCamera::MiddleClick(CAM_INPUT eState, const _vec2* pClickedPos)
{
	switch (eState)
	{
	case CDynamicCamera::CAM_INPUT_ROT:
		m_vOldData = m_vAngle;
		break;
	case CDynamicCamera::CAM_INPUT_MOVE:
		m_vOldData = m_vAt;
		break;
	}
	memcpy(&m_vClickPos, pClickedPos, sizeof(_vec2));
	m_eState = eState;
}

void CDynamicCamera::MiddleClick_Update(const _vec2* pMousePos)
{
	if (pMousePos)
	{
		_vec2 vDelta = *pMousePos - m_vClickPos;
		switch (m_eState)
		{
		case CDynamicCamera::CAM_INPUT_ROT:
			m_vAngle = m_vOldData;
			m_vAngle.x += D3DXToRadian(vDelta.y * m_fSpeed * 2.f);
			m_vAngle.y += D3DXToRadian(vDelta.x * m_fSpeed * 2.f);
			if (m_vAngle.x > D3DX_PI * 0.5f)
				m_vAngle.x = D3DX_PI * 0.5f;
			else if (m_vAngle.x < -D3DX_PI * 0.5f)
				m_vAngle.x = -D3DX_PI * 0.5f;
			break;
		case CDynamicCamera::CAM_INPUT_MOVE:
			m_vAt = m_vOldData;
			m_vAt -= m_vRight * m_fSpeed * vDelta.x * 0.01f * m_fDistance;
			m_vAt += m_vUp * m_fSpeed * vDelta.y * 0.01f * m_fDistance;
			break;
		}
		Update_EyeAtUp();
	}
}

void CDynamicCamera::MiddleClickReset()
{
	m_eState = CAM_INPUT_MOVE;
}

void CDynamicCamera::ZoomInOut(const _float & fDelta)
{
	m_fDistance -= fDelta * m_fDistance * 0.5f;
	if (m_fDistance < 0.1f)
		m_fDistance = 0.1f;
}

void CDynamicCamera::Update_EyeAtUp()
{
	D3DXMatrixRotationX(&m_matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&m_matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&m_matRotZ, m_vAngle.z);

	m_matRotAll = m_matRotX * m_matRotY * m_matRotZ;

	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &m_matRotAll);

	_vec3 vAtDir;
	D3DXVec3TransformNormal(&vAtDir, &AXIS_Z, &m_matRotAll);
	m_vEye = m_vAt - vAtDir * m_fDistance;
}

void CDynamicCamera::Update_RightLook()
{
	D3DXVec3Normalize(&m_vLook, &(m_vAt - m_vEye));
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicCamera* pInst = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInst->Ready_Camera()))
		Safe_Release(pInst);

	return pInst;
}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}

