#include "stdafx.h"
#include "UndeadThrowRoute.h"

#include "Transform.h"
#include "MyMath.h"

CUndeadThrowRoute::CUndeadThrowRoute()
{
}

CUndeadThrowRoute::~CUndeadThrowRoute()
{
}

HRESULT CUndeadThrowRoute::Ready_Component(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pCenterPos, const _vec3* pMoveDir, const _float& fSpeed, const _float& fAngularSpeed)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();

	memcpy(&m_vCenterPos, pCenterPos, sizeof(_vec3));

	m_vOutDir = *pStartPos - *pCenterPos;
	
	m_fRadius = D3DXVec3Length(&m_vOutDir);
	D3DXVec3Normalize(&m_vOutDir, &m_vOutDir);

	m_fSpeed = fSpeed;
	m_fAngularSpeed = fAngularSpeed;

	_vec3 vLook;

	D3DXVec3Cross(&vLook, &m_vAxis, &m_vOutDir);

	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vLookAngle = {};
	_vec3 vLookArray[2];
	vLookArray[0] = vLookArray[1] = vLook;

	for (_int i = 0; i < 2; ++i)
	{
		(*((_float*)&vLookArray[i] + i)) = 0.f;
		D3DXVec3Normalize(&vLookArray[i], &vLookArray[i]);
	}

	vLookAngle.x = Engine::CMyMath::AngleBetween2Vectors(&AXIS_Z, &vLookArray[0], &AXIS_X);
	vLookAngle.y = Engine::CMyMath::AngleBetween2Vectors(&AXIS_Z, &vLookArray[1], &AXIS_Y);
	vLookAngle.z = 0.f;
	m_pTransform->Set_Angle(&vLookAngle);

	return S_OK;
}

_int CUndeadThrowRoute::Update_Component(const _float& fTimeDelta)
{
	m_fRadius -= m_fSpeed * fTimeDelta;
	if (m_fRadius < 0.f)
		m_fRadius = 0.f;
	m_fAngle += m_fAngularSpeed * fTimeDelta;

	_matrix matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &m_vAxis, m_fAngle);

	_vec3 vRotPos;
	D3DXVec3TransformCoord(&vRotPos, &(m_vOutDir * m_fRadius), &matRotAxis);
	
	m_pTransform->Set_Pos(&(vRotPos + m_vCenterPos));

	return 0;
}

CUndeadThrowRoute * CUndeadThrowRoute::Create(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pCenterPos, const _vec3* pMoveDir, const _float& fSpeed, const _float& fAngularSpeed)
{
	CUndeadThrowRoute* pInstance = new CUndeadThrowRoute;

	if (FAILED(pInstance->Ready_Component(pTransform, pStartPos, pCenterPos, pMoveDir, fSpeed, fAngularSpeed)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CUndeadThrowRoute::Free()
{
	::Safe_Release(m_pTransform);
	Engine::CMoveOnRoute::Free();
}
