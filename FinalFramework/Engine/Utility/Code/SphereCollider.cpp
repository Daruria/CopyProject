#include "SphereCollider.h"
#include "Transform.h"
#include "GameObject.h"

#include "Export_Function.h"

USING(Engine)

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCollider(pGraphicDev)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider & rhs)
	:	CCollider(rhs)
	,	m_fRadius(rhs.m_fRadius)
{
}

CSphereCollider::~CSphereCollider()
{
}

HRESULT CSphereCollider::Ready_SphereCollider(CCollider::PARENT_TYPE eParentType, void * pParent, 
	const _matrix * pLocal, const _float& fRadius, const wstring & wstrName)
{
	m_eType = CT_SPHERE;
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
	//case Engine::CCollider::PT_OBJECT:
	//{
	//	CGameObject* pObj = reinterpret_cast<CGameObject*>(m_pParent);
	//	pObj->AddRef();
	//}
	//break;
	//case Engine::CCollider::PT_COLLIDER:
	//{
	//	CCollider* pColl = reinterpret_cast<CCollider*>(m_pParent);
	//	pColl->AddRef();
	//}
	//break;
	}
	memcpy(&m_matLocal, pLocal, sizeof(_matrix));
	m_wstrName = wstrName;
	m_fRadius = fRadius;

#ifdef _DEBUG
	FAILED_CHECK_RETURN(D3DXCreateSphere(m_pGraphicDev, fRadius, 20, 20, &m_SphereMesh, NULL), E_FAIL);

	FAILED_CHECK_RETURN(Ready_ColliderTexture(m_pGraphicDev), E_FAIL);

#endif // _DEBUG


	return S_OK;
}

_int Engine::CSphereCollider::Update_Component(const _float& fTimeDelta)
{
	_int iExit = CCollider::Update_Component(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

#ifdef _DEBUG
	Get_Renderer()->Add_RenderCollider(this);
	m_eColor = m_bActivated ? CC_BLUE : CC_GREEN;
#endif // _DEBUG

	return iExit;
}

void CSphereCollider::Render_Collider()
{
#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_eColor);
	m_SphereMesh->DrawSubset(0);
#endif // _DEBUG
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CCollider::PARENT_TYPE eParentType, void * pParent, 
	const _matrix * pLocal, const _float& fRadius, const wstring & wstrName, const wstring& wstrBoneName)
{
	CSphereCollider* pInst = new CSphereCollider(pGraphicDev);

	if (FAILED(pInst->Ready_SphereCollider(eParentType, pParent, pLocal, fRadius, wstrName)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CComponent* Engine::CSphereCollider::Clone(void)
{
	return new CSphereCollider(*this);
}

void CSphereCollider::Free()
{
#ifdef _DEBUG
	Safe_Release(m_SphereMesh);
#endif // _DEBUG

	CCollider::Free();
}
