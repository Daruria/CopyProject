#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	ZeroMemory(&m_vUp, sizeof(_vec3));
}

Engine::CCamera::~CCamera()
{

}

HRESULT Engine::CCamera::Ready_Camera()
{
	m_vEye	= { 0.f, 0.f, 0.f };
	m_vAt	= { 0.f, 0.f, 1.f };
	m_vUp	= { 0.f, 1.f, 0.f };

	m_fFovY = D3DXToRadian(60.f);
	m_fAspect = 1.f;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW,	&m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_matPreView = m_matView;

	ZeroMemory(&m_tFrustum, sizeof(FRUSTUM));

	return S_OK;
}

_int Engine::CCamera::Update_Camera(const _float& fTimeDelta)
{
	m_matPreView = m_matView;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	Update_Frustum();

	return 0;
}

void CCamera::Update_Frustum()
{
	m_tFrustum.vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_tFrustum.vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_tFrustum.vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_tFrustum.vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_tFrustum.vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_tFrustum.vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_tFrustum.vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_tFrustum.vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	_matrix matProj, matView;

	D3DXMatrixInverse(&matProj, NULL, &m_matProj);
	D3DXMatrixInverse(&matView, NULL, &m_matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_tFrustum.vPoint[i], &m_tFrustum.vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_tFrustum.vPoint[i], &m_tFrustum.vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_tFrustum.Plane[0], &m_tFrustum.vPoint[1], &m_tFrustum.vPoint[5], &m_tFrustum.vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_tFrustum.Plane[1], &m_tFrustum.vPoint[4], &m_tFrustum.vPoint[0], &m_tFrustum.vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_tFrustum.Plane[2], &m_tFrustum.vPoint[4], &m_tFrustum.vPoint[5], &m_tFrustum.vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_tFrustum.Plane[3], &m_tFrustum.vPoint[3], &m_tFrustum.vPoint[2], &m_tFrustum.vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_tFrustum.Plane[4], &m_tFrustum.vPoint[7], &m_tFrustum.vPoint[6], &m_tFrustum.vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_tFrustum.Plane[5], &m_tFrustum.vPoint[0], &m_tFrustum.vPoint[1], &m_tFrustum.vPoint[2]);

}

void CCamera::Get_Right(_vec3 * pRight) const
{
	_vec3 vLook = m_vAt - m_vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(pRight, &m_vUp, &vLook);
	D3DXVec3Normalize(pRight, pRight);
}

void CCamera::Get_Look(_vec3 * pLook) const
{
	*pLook = m_vAt - m_vEye;
	D3DXVec3Normalize(pLook, pLook);
}

const _vec3 * CCamera::Get_Angle(_vec3 * pAngle)
{
	_vec3 vAngle;



	return nullptr;
}

void CCamera::Set_Angle(const _vec3 * pAngle)
{
	 
}

void CCamera::Set_Angle(const _float & fx, const _float & fy, const _float & fz)
{
}

void CCamera::Set_AngleX(const _float & fx)
{
}

void CCamera::Set_AngleY(const _float & fy)
{
}

void CCamera::Set_AngleZ(const _float & fz)
{
}

_bool CCamera::Is_InFrustum(const _vec3 * pPos)
{
	_float	fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_tFrustum.Plane[i], pPos);

		if (fDistance > 0.f)
			return false;
	}

	return true;
}

_bool CCamera::Is_InFrustum(const _vec3 * pPos, const _float & fRadius)
{
	_float	fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_tFrustum.Plane[i], pPos);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

void Engine::CCamera::Free(void)
{
	Safe_Release(m_pGraphicDev);
}