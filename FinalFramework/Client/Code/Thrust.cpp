#include "stdafx.h"
#include "Thrust.h"

#include "Export_Function.h"
#include "CylinderEffect.h"
#include "MultiCollider.h"

CThrust::CThrust(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{
}

CThrust::~CThrust()
{
}

HRESULT CThrust::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _vec3* pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR& dwColor)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	m_iAttack = 150;
	m_fLifeTime = 0.5f;
	m_fLifeCount = 0.f;

	_vec3 vPos = *pPos;

	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Angle(pRot);

	CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.2f, 1.f, 2.f, Engine::RENDER_DISTORTION, 5);
	NULL_CHECK_RETURN(pEffect, -1);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), -1);
	pEffect->Set_Pos(&vPos);
	pEffect->Set_Scale(1.f);
	pEffect->Set_Speed(0.5f);
	pEffect->Set_DeltaAlpha(-1.f);
	pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
	pEffect->Set_MaxScale(&_vec3(1.25f, 1.25f, 1.25f));
	//pEffect->Set_DirUV(&_vec2(1.f, 0.f));
	pEffect->Set_Rotation(pRot);
	pEffect->Set_LifeTime(0.5f);


	vPos += *pLook * 2.f;
	pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.5f, 0.5f, 4.f, Engine::RENDER_DISTORTION, 5);
	NULL_CHECK_RETURN(pEffect, -1);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), -1);
	pEffect->Set_Pos(&vPos);
	pEffect->Set_Scale(1.f);
	pEffect->Set_Speed(0.5f);
	pEffect->Set_DeltaAlpha(-1.f);
	pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
	pEffect->Set_MaxScale(&_vec3(1.25f, 1.25f, 1.25f));
	//pEffect->Set_DirUV(&_vec2(1.f, 0.f));
	pEffect->Set_Rotation(pRot);
	pEffect->Set_LifeTime(0.5f);

	vPos += *pLook * 2.f;
	pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.1f, 0.5f, 2.f, Engine::RENDER_DISTORTION, 5);
	NULL_CHECK_RETURN(pEffect, -1);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), -1);
	pEffect->Set_Pos(&vPos);
	pEffect->Set_Scale(1.f);
	pEffect->Set_Speed(0.5f);
	pEffect->Set_DeltaAlpha(-1.f);
	pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
	pEffect->Set_MaxScale(&_vec3(1.25f, 1.25f, 1.25f));
	//pEffect->Set_DirUV(&_vec2(1.f, 0.f));
	pEffect->Set_Rotation(pRot);
	pEffect->Set_LifeTime(0.5f);

	return S_OK;
}

_int CThrust::Update_GameObject(const _float & fTimeDelta)
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

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Player_Attack", this);

	return 0;
}

_int CThrust::LateUpdate_GameObject(const _float & fTimeDelta)
{
	_int iExit = CSkill::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CThrust::Render_GameObject(const _float & fTimeDelta)
{
	
}

HRESULT CThrust::Add_Component(const _ushort & wContainerIdx, const wstring & wstrTextureTag)
{
	FAILED_CHECK_RETURN(CSkill::Add_Component(), E_FAIL);

	Engine::CComponent* pComponent = nullptr;

	_matrix matLocal;
	D3DXMatrixIdentity(&matLocal);

	Engine::CMultiCollider* pMultiCol = nullptr;

	pComponent = m_pColliderCom = pMultiCol = Engine::CMultiCollider::Create(m_pGraphicDev, Engine::CCollider::PT_TRANSFORM, m_pTransformCom, &matLocal, nullptr, L"Col_Thrust");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pComponent);

	Engine::CCollider* pChildCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_COLLIDER, pMultiCol, &matLocal, 1.f, L"Col_Sphere0");
	pMultiCol->Add_Collider(pChildCol);

	matLocal._42 += 2.f;
	pChildCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_COLLIDER, pMultiCol, &matLocal, 1.f, L"Col_Sphere1");
	pMultiCol->Add_Collider(pChildCol);

	matLocal._42 += 2.f;
	pChildCol = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_COLLIDER, pMultiCol, &matLocal, 1.f, L"Col_Sphere2");
	pMultiCol->Add_Collider(pChildCol);

	m_pColliderCom->Activate();
	return S_OK;
}

CThrust * CThrust::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _vec3 * pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR & dwColor)
{
	CThrust* pInstance = new CThrust(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wContainerIdx, wstrTextureTag, pPos, pLook, pRot, dwColor)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CThrust::Free()
{
	CGameObject::Free();
}
