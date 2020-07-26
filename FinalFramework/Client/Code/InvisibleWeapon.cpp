#include "stdafx.h"
#include "InvisibleWeapon.h"
#include "ChildTransform.h"
#include "SphereCollider.h"
#include "MultiCollider.h"
#include "BoxCollider.h"

#include "Export_Function.h"

CInvisibleWeapon::CInvisibleWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CWeapon(pGraphicDev)
{
}

CInvisibleWeapon::~CInvisibleWeapon()
{
}

HRESULT CInvisibleWeapon::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(1.f), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);
	//m_pTransformCom->Set_AngleY(D3DX_PI);
	m_wstrName = L"Weapon_InvisibleWeapon";

	return S_OK;
}

HRESULT CInvisibleWeapon::Ready_GameObject(const _float & fRadius)
{
	FAILED_CHECK_RETURN(Add_Component(fRadius), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);
	//m_pTransformCom->Set_AngleY(D3DX_PI);
	m_wstrName = L"Weapon_InvisibleWeapon";

	return S_OK;
}


_int CInvisibleWeapon::Update_GameObject(const _float& fTimeDelta)
{
	//_int iExit = CWeapon::Update_GameObject(fTimeDelta);

	//if (iExit & 0x80000000)
	//	return -1;

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Monster_Attack", this);

	return 0;
}

_int CInvisibleWeapon::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CInvisibleWeapon::Render_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	//CWeapon::Render_GameObject(fTimeDelta);
}

void CInvisibleWeapon::Attack()
{
	m_pColliderCom->Activate();
}

void CInvisibleWeapon::Stop()
{
	m_pColliderCom->Deactivate();
}

HRESULT CInvisibleWeapon::Add_Component(const _float& fRadius, const _ulong& dwFlags)
{
	Engine::CComponent* pCom = nullptr;

	pCom = m_pTransformCom = CTransform::Create(dwFlags);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pCom);

	pCom = m_pRenderer = Get_Renderer();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_pRenderer->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pCom);

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);

	pCom = m_pColliderCom = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_TRANSFORM, m_pTransformCom, &matTemp, fRadius, L"Col_InvisibleWeapon");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	return S_OK;
}

CInvisibleWeapon * CInvisibleWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fRadius)
{
	CInvisibleWeapon* pInst = new CInvisibleWeapon(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(fRadius)))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CInvisibleWeapon::Free()
{
	CWeapon::Free();
}