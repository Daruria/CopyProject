#include "stdafx.h"
#include "ChargeTrailRoute.h"

#include "Transform.h"
#include "MyMath.h"

CChargeTrailRoute::CChargeTrailRoute()
{
}

CChargeTrailRoute::~CChargeTrailRoute()
{
}

HRESULT CChargeTrailRoute::Ready_Component(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pDestPos, const _float& fSpeed, const _float& fAngularSpeed)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();

	memcpy(&m_vDestPos, pDestPos, sizeof(_vec3));

	m_vDir = *pStartPos - *pDestPos;
	//_vec3 vAxis = { rand() % 100 / 100.f - 0.5f, rand() % 100 / 100.f - 0.5f, rand() % 100 / 100.f - 0.5f };

	_float fRand1 = (rand() % 100) / 100.f - 0.5f;
	_float fRand2 = (rand() % 100) / 100.f - 0.5f;
	_vec3 vRandomAxis = fRand1 * AXIS_Y + fRand2 * AXIS_Z;

	D3DXVec3Cross(&m_vAxis, &vRandomAxis, &m_vDir);
	
	m_fRadius = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_fSpeed = fSpeed;
	m_fAngularSpeed = fAngularSpeed;

	_vec3 vLook;

	D3DXVec3Cross(&vLook, &m_vAxis, &m_vDir);

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

_int CChargeTrailRoute::Update_Component(const _float& fTimeDelta)
{
	m_fRadius -= m_fSpeed * fTimeDelta;
	if (m_fRadius < 0.5f)
		m_fRadius = 0.5f;
	m_fAngle += m_fAngularSpeed * fTimeDelta;

	_matrix matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &m_vAxis, m_fAngle);

	_vec3 vRotPos;
	D3DXVec3TransformCoord(&vRotPos, &(m_vDir * m_fRadius), &matRotAxis);
	
	m_pTransform->Set_Pos(&(vRotPos + m_vDestPos));

	return 0;
}

CChargeTrailRoute * CChargeTrailRoute::Create(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pDestPos, const _float& fSpeed, const _float& fAngularSpeed)
{
	CChargeTrailRoute* pInstance = new CChargeTrailRoute;

	if (FAILED(pInstance->Ready_Component(pTransform, pStartPos, pDestPos, fSpeed, fAngularSpeed)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CChargeTrailRoute::Free()
{
	::Safe_Release(m_pTransform);
	Engine::CMoveOnRoute::Free();
}
