#include "stdafx.h"
#include "MapObject.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Texture.h"

#include "Export_Function.h"
#include "Weapon.h"
#include "HealingObject.h"
#include "EmptyEffect.h"
#include "FollowGlowRoute.h"
#include "BrokenWood.h"
#include "UVEffect.h"

CMapObject::CMapObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CStaticObject(pGraphicDev)
{
}

CMapObject::~CMapObject()
{
}


HRESULT CMapObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

HRESULT CMapObject::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f);


	m_vShakingDir = { 0.1f, 0.f, 0.f };

	return S_OK;
}

HRESULT CMapObject::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CStaticObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	Engine::CCollider* pCol = nullptr;
	_matrix matCol;
	D3DXMatrixIdentity(&matCol);
	matCol._42 += 1.f;
	wstring wstrColName = L"Col_" + m_wstrName;
	//pCom = pCol = CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, 1.f, wstrColName);
	pCom = pCol = CBoxCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, &_vec3(2.f, 2.f, 2.f), wstrColName);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	pCol->Activate();

	pCom = m_pDissolveTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_Dissolve_Bits"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_DissolveTexture", pCom);


	pCom = m_pNaviCom = DC_ENG(CNavigationMesh, Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pCom);


	return S_OK;
}

_int CMapObject::Update_GameObject(const _float& fTimeDelta)
{
	//m_fDissolveTime += fTimeDelta * 0.1f;

	if (m_bIsDead)
		return 0;

	if (m_bShake)
	{
		m_fShakingTime += fTimeDelta;

		if (m_fShakingTime > 1.f)
		{
			m_bShake = false;
			m_fShakingTime = 0.f;
		}

		m_pTransformCom->Set_Pos(m_vOriginPos + m_vShakingDir * sinf(m_fShakingTime * 40.f));
	}


	_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	Engine::Add_GameObject_To_CollisionList(L"MapObject", this);

	return 0;
}

_int CMapObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CMapObject::Render_GameObject(const _float& fTimeDelta)
{
	if (m_vecShader.empty())
		return;

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	_vec3 vCenter, vScale;
	D3DXVec3TransformCoord(&vCenter, &m_vCenter, &matWorld);

	m_pTransformCom->Get_Scale(&vScale);

	_float fScale = max(vScale.x, max(vScale.y, vScale.z));
	_float fRadius = m_fRadius * fScale;

	if (!Get_MainCamera()->Is_InFrustum(&vCenter, fRadius))
		return;

	LPD3DXEFFECT	pEffect = m_vecShader[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	//m_pMeshCom->Render_Meshes(pEffect);

	//pEffect->SetFloat()
	//m_fDissolveTime += fTimeDelta;
	//pEffect->SetFloat("g_fTime", m_fDissolveTime);
	//m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

	//m_pMeshCom->Render_Meshes_WithPass(pEffect, 2);
	m_pMeshCom->Render_Meshes_WithPass(pEffect, 3);

	pEffect->End();

	::Safe_Release(pEffect);
}

void CMapObject::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
	m_vOriginPos = *pPos;
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pPos));
}

void CMapObject::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
	m_vOriginPos = { fX, fY, fZ };
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&m_vOriginPos));
}

void CMapObject::Collision_Enter(CGameObject * pOpponent)
{
	CWeapon*	pWeapon = dynamic_cast<CWeapon*>(pOpponent);
	
	if (pWeapon)
	{
		m_iHP -= 1;

		m_bShake = true;

		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

		_matrix matRotY;

		_float fAngle = (rand() % 180) / D3DX_PI;
		for (_int i = 0; i < 10; ++i)
		{
			D3DXMatrixRotationY(&matRotY, fAngle);
			_vec3 vRot = { 1.f, 0.f, 0.f };
			D3DXVec3TransformCoord(&vRot, &vRot, &matRotY);
			Add_Dust(&(vPos + vRot));
			fAngle += D3DX_PI / 5.f;
		}


		if (m_iHP <= 0)
		{
			m_iHP = 0;

			m_bIsDead = true;

			_int iRand = rand() % 5 + 10;

			for (_int i = 0; i < iRand; ++i)
				Add_HealingObject(&vPos);

			CBrokenWood* pWood = nullptr;
			wstring wstrWoodTag[6] = { L"Mesh_BrokenWood", L"Mesh_BrokenWood1", L"Mesh_BrokenWood2", L"Mesh_BrokenWood3", L"Mesh_BrokenWood4", L"Mesh_BrokenWood5" };
			for (_int i = 0; i < 15; ++i)
			{
				_vec3 vWoodPos = m_vOriginPos + _vec3(((rand() % 100) - 50.f) / 20.f, ((rand() % 100) - 50.f) / 20.f, ((rand() % 100) - 50.f) / 20.f);
				_vec3 vDir = _vec3(((rand() % 100) - 50.f) / 5.f, ((rand() % 100) - 50.f) / 5.f, ((rand() % 100) - 50.f) / 5.f);
				D3DXVec3Normalize(&vDir, &vDir);
				vDir *= (_float)(rand() % 10) / 5.f;

				pWood = CBrokenWood::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrWoodTag[rand() % 6], L"BrokenWood", &vDir);
				NULL_CHECK(pWood);
				pWood->Set_Pos(&vWoodPos, m_pNaviCom->Get_Index());
				pWood->Set_Scale(0.002f);

				FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"BrokenWood", pWood), );
			}

		}
	}
}

void CMapObject::Collision_Stay(CGameObject * pOpponent)
{
}

void CMapObject::Collision_Leave(CGameObject * pOpponent)
{
}

void CMapObject::Add_HealingObject(const _vec3 * pPos)
{
	_vec3 vPos = *pPos;
	vPos += _vec3(((rand() % 100) / 100.f) - 0.5f, ((rand() % 100) / 100.f) - 0.5f, ((rand() % 100) / 100.f) - 0.5f);

	CHealingObject*	pHealing = CHealingObject::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"DisCrystal", L"HealingCrystal");
	NULL_CHECK_RETURN(pHealing, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_HealingCrystal", pHealing), );
	pHealing->Set_Pos(&vPos);
	pHealing->Set_Scale(0.002f);

	_vec3 vSpreadDir = { (rand() % 100) / 100.f - 0.5f, (rand() % 100 / 100.f),  (rand() % 100) / 100.f - 0.5f };
	pHealing->Set_Variables(20.f, -40.f, D3DXVec3Normalize(&vSpreadDir, &vSpreadDir));

	CEmptyEffect* pEffect = CEmptyEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Radial_Mask", D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
	NULL_CHECK_RETURN(pEffect, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Glow", pEffect), );
	pEffect->Set_Pos(&vPos);
	pEffect->Set_LifeTime(100.f);
	pEffect->Set_UV(&_vec2(0.f, 0.f), &_vec2(1.f, 1.f));
	pEffect->Set_RenderID_Pass(Engine::RENDER_EFFECT, 2);


	Engine::CTransform*	pEffectTrans = dynamic_cast<Engine::CTransform*>(pEffect->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
	CFollowGlowRoute* pRoute = CFollowGlowRoute::Create(pEffectTrans, pHealing);
	NULL_CHECK_RETURN(pRoute, );
	pEffect->Add_MoveOnRoute(pRoute);

	pRoute->Set_Scaling(0.5f, 2.f, 2.f);
}

void CMapObject::Add_Dust(const _vec3 * pPos)
{
	CUVEffect* pDust = CUVEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	NULL_CHECK_RETURN(pDust, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Dust", pDust), );
	pDust->Set_UV(&_vec2((rand() % 2) * 0.5f, (rand() % 2) * 0.5f), &_vec2(0.5f, 0.5f));
	pDust->Set_Pos(pPos);
	pDust->SetUp_ScaleChange(&_vec3(3.f, 3.f, 3.f), &_vec3(3.2f, 3.2f, 3.2f), 1.f);
	pDust->Set_LifeTime(1.f);
	pDust->Set_DeltaAlpha(-1.f);
	pDust->On_BillBoard();
	pDust->Set_RenderID_Pass(Engine::RENDER_ALPHA, 7);

}

CMapObject * CMapObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CMapObject* pInst = new CMapObject(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);
 
	return pInst;
}

void CMapObject::Free()
{
	CStaticObject::Free();
}
