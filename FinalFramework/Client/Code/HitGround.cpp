#include "stdafx.h"
#include "HitGround.h"

#include "Export_Function.h"
#include "CylinderEffect.h"
#include "MultiCollider.h"

CHitGround::CHitGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{
}

CHitGround::~CHitGround()
{
}

HRESULT CHitGround::Ready_GameObject(const _vec3* pPos, const _float& fRadius, const wstring& wstrCollTag)
{
	FAILED_CHECK_RETURN(Add_Component(fRadius), E_FAIL);

	m_iAttack = 150;
	m_fLifeTime = 0.5f;
	m_fLifeCount = 0.f;

	_vec3 vPos = *pPos;

	m_pTransformCom->Set_Pos(pPos);

	m_wstrCollTag = wstrCollTag;

	return S_OK;
}

_int CHitGround::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	_int iExit = CSkill::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(m_wstrCollTag, this);

	return 0;
}

_int CHitGround::LateUpdate_GameObject(const _float & fTimeDelta)
{
	_int iExit = CSkill::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CHitGround::Render_GameObject(const _float & fTimeDelta)
{
	
}

HRESULT CHitGround::Add_Component(const _float& fRadius)
{
	FAILED_CHECK_RETURN(CSkill::Add_Component(), E_FAIL);

	Engine::CComponent* pComponent = nullptr;

	_matrix matLocal;
	D3DXMatrixIdentity(&matLocal);

	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_TRANSFORM, m_pTransformCom, &matLocal, fRadius, L"Col_HitGround");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pComponent);
	
	m_pColliderCom->Activate();
	return S_OK;
}

CHitGround * CHitGround::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, const _float& fRadius, const wstring& wstrCollTag)
{
	CHitGround* pInstance = new CHitGround(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos, fRadius, wstrCollTag)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CHitGround::Free()
{
	CGameObject::Free();
}
