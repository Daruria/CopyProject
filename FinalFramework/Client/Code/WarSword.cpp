#include "stdafx.h"
#include "WarSword.h"
#include "ChildTransform.h"
#include "SphereCollider.h"
#include "MultiCollider.h"
#include "BoxCollider.h"
#include "SwordTrail.h"

#include "Export_Function.h"
#include "Particle.h"

CWarSword::CWarSword(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CWeapon(pGraphicDev)
{
}

CWarSword::~CWarSword()
{
}

HRESULT CWarSword::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(Engine::RESOURCE_STATIC, L"Mesh_WarSword"), E_FAIL);

	m_pSwordTrail = CSwordTrail::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_SwordTrail");
	NULL_CHECK_RETURN(m_pSwordTrail, E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);
	m_pTransformCom->Set_AngleY(D3DX_PI * 0.5f);
	m_wstrName = L"Weapon_WarSword";

	m_iAttack = 100;

	return S_OK;
}

HRESULT CWarSword::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	return S_OK;
}

_int CWarSword::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::Update_GameObject(fTimeDelta);
	
	if (iExit & 0x80000000)
		return -1;

	if (m_bTrail)
	{
		_vec3 vSwordTop, vSwordBottom, vUp;
		_matrix matWorld, matSphereTest0, matSphereTest1;
		m_pTransformCom->Get_WorldPos(&vSwordTop);
		m_pTransformCom->Get_WorldPos(&vSwordBottom);
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		memcpy(&vUp, matWorld.m[1], sizeof(_vec3));
		D3DXVec3Normalize(&vUp, &vUp);
		vSwordTop += vUp * -2.f;
		vSwordBottom += vUp * -0.3f;

		m_pSwordTrail->Add_Vertex(&vSwordTop, &vSwordBottom);
		m_pSwordTrail->Update_GameObject(fTimeDelta);
	}

	Engine::Add_GameObject_To_CollisionList(L"Player_Attack", this);

	return 0;
}

_int CWarSword::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CWarSword::Render_GameObject(const _float& fTimeDelta)
{
	CWeapon::Render_GameObject(fTimeDelta);
	



}

void CWarSword::Attack()
{
	m_pColliderCom->Activate();
	m_bTrail = true;
}

void CWarSword::Stop()
{
	m_pColliderCom->Deactivate();
	if (m_bTrail)
		m_pSwordTrail->Clear_Vertex();
	m_bTrail = false;
	
}

void CWarSword::Collision_Enter(CGameObject* pOpponent)
{
	_vec3 vPos, vOppoPos;
	m_pTransformCom->Get_WorldPos(&vPos);
	dynamic_cast<Engine::CTransform*>(pOpponent->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"))->Get_WorldPos(&vOppoPos);
	vPos = (vPos + vOppoPos) * 0.5f;

	//CParticle* pEffect = CParticle::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_EffectDot", 50);
	//NULL_CHECK(pEffect);
	//Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Particle", pEffect);
	//pEffect->Set_Scale(0.1f);
	//pEffect->Set_Speed(30.f);
	//pEffect->Set_Pos(&vPos);
	Engine::Play_Sound(L"imp_sword_01.ogg", Engine::CSoundMgr::EFFECT);
}

void CWarSword::Collision_Stay(CGameObject* pOpponent)
{
	
}

void CWarSword::Collision_Leave(CGameObject* pOpponent)
{
	
}

HRESULT CWarSword::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags)
{
	FAILED_CHECK_RETURN(CWeapon::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);
	matTemp._42 = -1.2f;

	CMultiCollider* pMultiCol;
	pCom = m_pColliderCom = pMultiCol = CMultiCollider::Create(m_pGraphicDev, CCollider::PT_TRANSFORM, m_pTransformCom, &matTemp, nullptr, L"Col_WarSword");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	matTemp._42 = -0.6f;
	CCollider* pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 0.5f, L"Col_WarSwordSphere0");
	pMultiCol->Add_Collider(pColl);

	matTemp._42 = 0.f;
	pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 0.5f, L"Col_WarSwordSphere1");
	pMultiCol->Add_Collider(pColl);

	matTemp._42 = 0.6f;
	pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 0.5f, L"Col_WarSwordSphere2");
	pMultiCol->Add_Collider(pColl);

	//pCom = CBoxCollider::Create(m_pGraphicDev, CCollider::PT_OBJECT, this, &matTemp, &_vec3(0.2f, 1.8f, 0.4f), L"Col_WarSword");
	//NULL_CHECK_RETURN(pCom, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	return S_OK;
}

CWarSword * CWarSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWarSword* pInst = new CWarSword(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CWarSword::Free()
{

	::Safe_Release(m_pSwordTrail);
	CWeapon::Free();
}