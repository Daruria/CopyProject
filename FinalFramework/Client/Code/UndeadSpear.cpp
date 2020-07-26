#include "stdafx.h"
#include "UndeadSpear.h"
#include "ChildTransform.h"
#include "SphereCollider.h"
#include "MultiCollider.h"
#include "BoxCollider.h"
#include "SwordTrail.h"

#include "Export_Function.h"

CUndeadSpear::CUndeadSpear(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CWeapon(pGraphicDev)
{
}

CUndeadSpear::~CUndeadSpear()
{
}

HRESULT CUndeadSpear::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(Engine::RESOURCE_STAGE, L"Mesh_UndeadSpear"), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);
	//m_pTransformCom->Set_AngleY();
	m_wstrName = L"Weapon_UndeadSpear";

	m_pSwordTrail = CSwordTrail::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_SwordTrail");
	NULL_CHECK_RETURN(m_pSwordTrail, E_FAIL);

	m_iAttack = 30;

	return S_OK;
}

HRESULT CUndeadSpear::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	return S_OK;
}

_int CUndeadSpear::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	if (m_bTrail)
	{
		_matrix matWorld;
		_vec3	vTopPos, vBottomPos, vLook;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		memcpy(&vTopPos, matWorld.m[3], sizeof(_vec3));
		memcpy(&vBottomPos, matWorld.m[3], sizeof(_vec3));
		memcpy(&vLook, matWorld.m[2], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);
		vTopPos += vLook * 3.f;

		m_pSwordTrail->Add_Vertex(&vTopPos, &vBottomPos);
		m_pSwordTrail->Update_GameObject(fTimeDelta);
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Monster_Attack", this);

	return 0;
}

_int CUndeadSpear::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CUndeadSpear::Render_GameObject(const _float& fTimeDelta)
{
	if (m_bRender)
		CWeapon::Render_GameObject(fTimeDelta);
}

void CUndeadSpear::Attack()
{
	m_pColliderCom->Activate();
	m_bTrail = true;
}

void CUndeadSpear::Stop()
{
	m_pColliderCom->Deactivate();
	if (m_bTrail)
		m_pSwordTrail->Clear_Vertex();
	m_bTrail = false;
}

HRESULT CUndeadSpear::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags)
{
	FAILED_CHECK_RETURN(CWeapon::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);
	//matTemp._42 = 0.f;

	Engine::CMultiCollider* pMultiCol = nullptr;
	pCom = m_pColliderCom = pMultiCol = CMultiCollider::Create(m_pGraphicDev, CCollider::PT_TRANSFORM, m_pTransformCom, &matTemp, nullptr, L"Col_UndeadSpear");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	matTemp._43 = 2.f;
	CCollider* pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_UndeadSpearSphere0");
	pMultiCol->Add_Collider(pColl);

	matTemp._43 = 0.f;
	pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_UndeadSpearSphere1");
	pMultiCol->Add_Collider(pColl);

	//matTemp._42 = 2.f;
	//pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_UndeadSpearSphere2");
	//pMultiCol->Add_Collider(pColl);

	//pCom = CBoxCollider::Create(m_pGraphicDev, CCollider::PT_OBJECT, this, &matTemp, &_vec3(0.2f, 1.8f, 0.4f), L"Col_UndeadSpear");
	//NULL_CHECK_RETURN(pCom, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	return S_OK;
}

CUndeadSpear * CUndeadSpear::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUndeadSpear* pInst = new CUndeadSpear(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CUndeadSpear::Free()
{
	::Safe_Release(m_pSwordTrail);
	CWeapon::Free();
}