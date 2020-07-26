#include "stdafx.h"
#include "UndeadShield.h"
#include "ChildTransform.h"
#include "SphereCollider.h"
#include "MultiCollider.h"
#include "BoxCollider.h"
#include "SwordTrail.h"

#include "Export_Function.h"

CUndeadShield::CUndeadShield(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CWeapon(pGraphicDev)
{
}

CUndeadShield::~CUndeadShield()
{
}

HRESULT CUndeadShield::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(Engine::RESOURCE_STAGE, L"Mesh_UndeadShield"), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);
	//m_pTransformCom->Set_AngleY(D3DX_PI);
	m_wstrName = L"Weapon_UndeadShield";

	m_iAttack = 30;

	return S_OK;
}

HRESULT CUndeadShield::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	return S_OK;
}

_int CUndeadShield::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Monster_Attack", this);

	return 0;
}

_int CUndeadShield::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CUndeadShield::Render_GameObject(const _float& fTimeDelta)
{
	if (m_bRender)
		CWeapon::Render_GameObject(fTimeDelta);
}

void CUndeadShield::Attack()
{
	m_pColliderCom->Activate();
}

void CUndeadShield::Stop()
{
	m_pColliderCom->Deactivate();
}

HRESULT CUndeadShield::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags)
{
	FAILED_CHECK_RETURN(CWeapon::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);
	

	pCom = m_pColliderCom = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_TRANSFORM, m_pTransformCom, &matTemp, 1.5f, L"Col_UndeadShield");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	return S_OK;
}

CUndeadShield * CUndeadShield::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUndeadShield* pInst = new CUndeadShield(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CUndeadShield::Free()
{
	CWeapon::Free();
}