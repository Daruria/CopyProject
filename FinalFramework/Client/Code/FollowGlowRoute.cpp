#include "stdafx.h"
#include "FollowGlowRoute.h"
#include "GameObject.h"

#include "Transform.h"
#include "MyMath.h"
#include "Export_Function.h"

CFollowGlowRoute::CFollowGlowRoute()
{
}

CFollowGlowRoute::~CFollowGlowRoute()
{
}

HRESULT CFollowGlowRoute::Ready_Component(Engine::CTransform* pTransform, Engine::CGameObject* pParent)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pParent = pParent;
	m_pParent->AddRef();

	return S_OK;
}

_int CFollowGlowRoute::Update_Component(const _float& fTimeDelta)
{
	if (m_pParent->IsDead())
	{
		m_bIsDead = true;
		return 0;
	}

	Engine::CTransform* pParentTrans = dynamic_cast<Engine::CTransform*>(m_pParent->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
	NULL_CHECK_RETURN(pParentTrans, -1);

	_vec3 vParentPos, vAngle;
	pParentTrans->Get_Info(Engine::INFO_POS, &vParentPos);
	//pParentTrans->Get_Angle(&vAngle);

	Engine::Get_MainCamera()->Get_Angle(&vAngle);

	m_pTransform->Set_Pos(&vParentPos);
	m_pTransform->Set_Angle(&vAngle);
	
	m_fScalingTime += fTimeDelta * m_fSpeed;
	m_pTransform->Set_Scale(m_fScale + m_fAmp * sinf(m_fScalingTime));

	if (m_fScalingTime > D3DX_PI * 2.f)
		m_fScalingTime -= D3DX_PI * 2.f;

	return 0;
}

void CFollowGlowRoute::Set_Scaling(const _float & fMinScale, const _float & fMaxScale, const _float & fSpeed)
{
	m_fScale = (fMaxScale + fMinScale) * 0.5f;

	m_fAmp = fMaxScale - fMinScale;
	m_fSpeed = fSpeed;
}

CFollowGlowRoute * CFollowGlowRoute::Create(Engine::CTransform* pTransform, Engine::CGameObject* pParent)
{
	CFollowGlowRoute* pInstance = new CFollowGlowRoute;

	if (FAILED(pInstance->Ready_Component(pTransform, pParent)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CFollowGlowRoute::Free()
{
	::Safe_Release(m_pParent);
	::Safe_Release(m_pTransform);
	Engine::CMoveOnRoute::Free();
}
