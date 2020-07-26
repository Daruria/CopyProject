#include "stdafx.h"
#include "FollowRoute.h"
#include "GameObject.h"

#include "Transform.h"
#include "MyMath.h"

CFollowRoute::CFollowRoute()
{
}

CFollowRoute::~CFollowRoute()
{
}

HRESULT CFollowRoute::Ready_Component(Engine::CTransform* pTransform, Engine::CGameObject* pParent)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pParent = pParent;
	m_pParent->AddRef();

	return S_OK;
}

_int CFollowRoute::Update_Component(const _float& fTimeDelta)
{
	Engine::CTransform* pParentTrans = dynamic_cast<Engine::CTransform*>(m_pParent->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
	NULL_CHECK_RETURN(pParentTrans, -1);

	_vec3 vParentPos, vAngle;
	pParentTrans->Get_Info(Engine::INFO_POS, &vParentPos);
	pParentTrans->Get_Angle(&vAngle);

	m_pTransform->Set_Pos(&vParentPos);
	m_pTransform->Set_Angle(&vAngle);
	m_pTransform->Set_Scale(1.f);

	return 0;
}

CFollowRoute * CFollowRoute::Create(Engine::CTransform* pTransform, Engine::CGameObject* pParent)
{
	CFollowRoute* pInstance = new CFollowRoute;

	if (FAILED(pInstance->Ready_Component(pTransform, pParent)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CFollowRoute::Free()
{
	::Safe_Release(m_pParent);
	::Safe_Release(m_pTransform);
	Engine::CMoveOnRoute::Free();
}
