#include "stdafx.h"
#include "MolochSword.h"
#include "ChildTransform.h"
#include "SphereCollider.h"
#include "MultiCollider.h"
#include "BoxCollider.h"
#include "SwordTrail.h"

#include "Export_Function.h"
#include "FireEffect.h"

CMolochSword::CMolochSword(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CWeapon(pGraphicDev)
{
}

CMolochSword::~CMolochSword()
{
}

HRESULT CMolochSword::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(Engine::RESOURCE_STAGE, L"Mesh_MolochSword"), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);
	m_pTransformCom->Set_AngleY(D3DX_PI);
	m_wstrName = L"Weapon_MolochSword";

	m_pSwordTrail = CSwordTrail::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_SwordTrail");
	NULL_CHECK_RETURN(m_pSwordTrail, E_FAIL);

	//m_pCrystal = Engine::CStaticObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_MolochSwordCrystal", L"CrystalSword");


	m_iAttack = 30;

	return S_OK;
}

HRESULT CMolochSword::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	return S_OK;
}

_int CMolochSword::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	if (m_bCrystalOn)
	{
		_matrix matWorld;
		_vec3	vTopPos, vBottomPos, vLook;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		memcpy(&vTopPos, matWorld.m[3], sizeof(_vec3));
		memcpy(&vBottomPos, matWorld.m[3], sizeof(_vec3));
		memcpy(&vLook, matWorld.m[1], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);
		vTopPos += -vLook * m_fSwordLength;

		_vec3 vPos = vBottomPos + ((rand() % 100) / 100.f) * (-vLook * m_fSwordLength);
		vPos += {rand() % 100 / 100.f - 0.5f, rand() % 100 / 100.f - 0.5f, rand() % 100 / 100.f - 0.5f};

		CFireEffect* pFire = CFireEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", L"Texture_FireNoise", L"Texture_FireAlpha");
		NULL_CHECK_RETURN(pFire, -1);
		Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Fire", pFire);
		pFire->Set_Scale(2.f);
		pFire->Set_Pos(&vPos);
		pFire->Set_LifeTime(0.5f);
		pFire->Set_DeltaAlpha(-2.f);
		_vec2 vUVStart = { rand() % 4 * 0.25f, rand() % 4 * 0.25f };
		_vec2 vUVSize = { 0.25f, 0.25f };
		pFire->Set_UV(&vUVStart, &vUVSize);
	}

	if (m_bTrail)
	{
		_matrix matWorld;
		_vec3	vTopPos, vBottomPos, vLook;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		memcpy(&vTopPos, matWorld.m[3], sizeof(_vec3));
		memcpy(&vBottomPos, matWorld.m[3], sizeof(_vec3));
		memcpy(&vLook, matWorld.m[1], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);
		vTopPos += -vLook * m_fSwordLength;

		m_pSwordTrail->Add_Vertex(&vTopPos, &vBottomPos);
		m_pSwordTrail->Update_GameObject(fTimeDelta);
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Monster_Attack", this);

	return 0;
}

_int CMolochSword::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CWeapon::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CMolochSword::Render_GameObject(const _float& fTimeDelta)
{
	//CWeapon::Render_GameObject(fTimeDelta);

	LPD3DXEFFECT	pEffect = m_vecShader[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	if (m_bDissolve)
	{
		m_fDissolveTime += fTimeDelta;
		pEffect->SetFloat("g_fTime", m_fDissolveTime);
		pEffect->SetVector("g_vDissolveColor", &_vec4(1.f, 0.f, 0.f, 1.f));
		m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

		m_pMeshCom->Render_Meshes_WithPass(pEffect, 2);
	}
	else
		m_pMeshCom->Render_Meshes(pEffect);
	if (m_bCrystalOn && m_pCrystalMeshCom)
	{
		if (m_bDissolve)
		{
			m_fDissolveTime += fTimeDelta;
			pEffect->SetFloat("g_fTime", m_fDissolveTime);
			pEffect->SetVector("g_vDissolveColor", &_vec4(1.f, 0.f, 0.f, 1.f));
			m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

			m_pCrystalMeshCom->Render_Meshes_WithPass(pEffect, 2);
		}
		else	
			m_pCrystalMeshCom->Render_Meshes(pEffect);
	}
	pEffect->End();

	::Safe_Release(pEffect);
}

void CMolochSword::Attack()
{
	m_pColliderCom->Activate();
	m_bTrail = true;
}

void CMolochSword::Stop()
{
	m_pColliderCom->Deactivate();
	if (m_bTrail)
		m_pSwordTrail->Clear_Vertex();
	m_bTrail = false;
}

void CMolochSword::CrystalOn()
{
	m_bCrystalOn = true;
	m_fSwordLength = 6.f;

	Engine::CMultiCollider* pMultiCol = dynamic_cast<Engine::CMultiCollider*>(m_pColliderCom);

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);

	matTemp._42 = -2.f;
	CCollider* pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_MolochSwordSphere2");
	pMultiCol->Add_Collider(pColl);

	matTemp._42 = -4.f;
	pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_MolochSwordSphere3");
	pMultiCol->Add_Collider(pColl);

}

HRESULT CMolochSword::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags)
{
	FAILED_CHECK_RETURN(CWeapon::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	pCom = m_pCrystalMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(wContainerIdx, L"Mesh_MolochSwordCrystal"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_CrystalMesh", pCom);

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);
	matTemp._42 = -2.f;

	Engine::CMultiCollider* pMultiCol = nullptr;
	pCom = m_pColliderCom = pMultiCol = CMultiCollider::Create(m_pGraphicDev, CCollider::PT_TRANSFORM, m_pTransformCom, &matTemp, nullptr, L"Col_MolochSword");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	matTemp._42 = 2.f;
	CCollider* pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_MolochSwordSphere0");
	pMultiCol->Add_Collider(pColl);

	matTemp._42 = 0.f;
	pColl = CSphereCollider::Create(m_pGraphicDev, CCollider::PT_COLLIDER, pMultiCol, &matTemp, 1.f, L"Col_MolochSwordSphere1");
	pMultiCol->Add_Collider(pColl);


	return S_OK;
}

CMolochSword * CMolochSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMolochSword* pInst = new CMolochSword(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CMolochSword::Free()
{
	::Safe_Release(m_pSwordTrail);
	CWeapon::Free();
}