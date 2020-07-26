#include "MultiCollider.h"
#include "Transform.h"
#include "GameObject.h"

#include "Export_Function.h"

USING(Engine)

CMultiCollider::CMultiCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCollider(pGraphicDev)
{
}

CMultiCollider::~CMultiCollider()
{
}

_int Engine::CMultiCollider::Update_Component(const _float& fTimeDelta)
{
	_int iExit = CCollider::Update_Component(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	for (auto& pColl : m_ChildList)
	{
		iExit = pColl->Update_Component(fTimeDelta);
		if (iExit & 0x80000000)
			return -1;
	}

#ifdef _DEBUG
	Get_Renderer()->Add_RenderCollider(this);
#endif // _DEBUG

	return iExit;
}

void Engine::CMultiCollider::Render_Collider()
{
#ifdef _DEBUG
	for (auto& pColl : m_ChildList)
		pColl->Render_Collider();
#endif // _DEBUG
}

HRESULT CMultiCollider::Ready_MultiCollider(CCollider::PARENT_TYPE eParentType, void * pParent, const _matrix * pLocal, 
	list<CCollider*>* pColliderList, const wstring & wstrName)
{
	m_eType = CT_MULTI;
	m_eParentType = eParentType;
	m_pParent = pParent;

	switch (eParentType)
	{
	case Engine::CCollider::PT_TRANSFORM:
	{
		CTransform* pTransform = reinterpret_cast<CTransform*>(m_pParent);
		pTransform->AddRef();
	}
	break;
	case Engine::CCollider::PT_OBJECT:
	{
		CGameObject* pObj = reinterpret_cast<CGameObject*>(m_pParent);
		pObj->AddRef();
	}
	break;
	case Engine::CCollider::PT_COLLIDER:
	{
		CCollider* pColl = reinterpret_cast<CCollider*>(m_pParent);
		pColl->AddRef();
	}
	break;
	}
	memcpy(&m_matLocal, pLocal, sizeof(_matrix));
	m_wstrName = wstrName;

	if (pColliderList)
	{
		for (auto& pColl : *pColliderList)
			m_ChildList.push_back(pColl);
	}


	return S_OK;
}

HRESULT CMultiCollider::Add_Collider(CCollider * pColl)
{
	m_ChildList.push_back(pColl);

	return S_OK;
}


CMultiCollider * CMultiCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CCollider::PARENT_TYPE eParentType, void * pParent,
	const _matrix * pLocal, list<CCollider*>* pColliderList, const wstring & wstrName)
{
	CMultiCollider* pInst = new CMultiCollider(pGraphicDev);

	if (FAILED(pInst->Ready_MultiCollider(eParentType, pParent, pLocal, pColliderList, wstrName)))
		Safe_Release(pInst);

	return pInst;
}

void CMultiCollider::Free()
{
	for_each(m_ChildList.begin(), m_ChildList.end(), CDeleteObj());
	m_ChildList.clear();
	CCollider::Free();
}
