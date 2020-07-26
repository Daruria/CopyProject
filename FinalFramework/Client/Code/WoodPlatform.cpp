#include "stdafx.h"
#include "WoodPlatform.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "Export_Function.h"
#include "Weapon.h"
#include "BrokenWood.h"
#include "NavigationMesh.h"

CWoodPlatform::CWoodPlatform(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CMapObject(pGraphicDev)
{
}

CWoodPlatform::~CWoodPlatform()
{
}


HRESULT CWoodPlatform::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

HRESULT CWoodPlatform::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	m_pTransformCom->Set_Scale(0.005f);

	m_vShakingDir = { 0.1f, 0.f, 0.f };

	m_vOriginPos = { 0.f, 0.f, 0.f };

	return S_OK;
}

HRESULT CWoodPlatform::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CStaticObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	Engine::CCollider* pCol = nullptr;
	_matrix matCol;
	D3DXMatrixIdentity(&matCol);
	matCol._42 += 1.f;
	wstring wstrColName = L"Col_" + m_wstrName;
	//pCom = pCol = CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, 1.f, wstrColName);
	pCom = pCol = CBoxCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, &_vec3(5.f, 5.f, 5.f), wstrColName);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	pCol->Activate();

	pCom = m_pNaviCom = DC_ENG(CNavigationMesh, Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pCom);

	return S_OK;
}

_int CWoodPlatform::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_pNaviCom->Change_CellOption(m_pNaviCom->Get_Index(), Engine::CCell::NO_MOVE);

	if (m_bShaking)
	{
		m_fShakingTime += fTimeDelta;

		if (m_fShakingTime > 1.f)
			m_bShaking = false;

		m_pTransformCom->Set_Pos(m_vOriginPos + m_vShakingDir * sinf(m_fShakingTime * 40.f));
		
	}

	if (Engine::KeyDown(DIK_N))
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		cout << "WoodPlatform" << endl;
		cout << "X : " << vPos.x << "\tY : " << vPos.y << "\tZ : " <<vPos.z<< endl;
	}

	_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	Engine::Add_GameObject_To_CollisionList(L"WoodPlatform", this);

	return 0;
}

_int CWoodPlatform::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CWoodPlatform::Render_GameObject(const _float& fTimeDelta)
{
	CStaticObject::Render_GameObject(fTimeDelta);
}

void CWoodPlatform::Collision_Enter(CGameObject* pOpponent)
{
	++m_iHitCount;

	if (m_iHitCount > 2)
	{
		m_bIsDead = true;

		m_pNaviCom->Change_CellOption(m_pNaviCom->Get_Index(), Engine::CCell::NO_OPTION);

		CBrokenWood* pWood = nullptr;
		wstring wstrWoodTag[6] = { L"Mesh_BrokenWood", L"Mesh_BrokenWood1", L"Mesh_BrokenWood2", L"Mesh_BrokenWood3", L"Mesh_BrokenWood4", L"Mesh_BrokenWood5" };
		for (_int i = 0; i < 30; ++i)
		{
			_vec3 vWoodPos = m_vOriginPos + _vec3(((rand() % 100) - 50.f) / 20.f, ((rand() % 100) - 50.f) / 20.f, ((rand() % 100) - 50.f) / 20.f);
			_vec3 vDir = _vec3(((rand() % 100) - 50.f) / 5.f, ((rand() % 100) - 50.f) / 5.f, ((rand() % 100) - 50.f) / 5.f);
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= (_float)(rand() % 10);
			
			pWood = CBrokenWood::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrWoodTag[rand() % 6], L"BrokenWood", &vDir);
			NULL_CHECK(pWood);
			pWood->Set_Pos(&vWoodPos, m_pNaviCom->Get_Index());
			
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"BrokenWood", pWood), );
		}


		Engine::Play_Sound(L"char_war_land_explode.ogg", Engine::CSoundMgr::EFFECT);
	}

	m_bShaking = true;
	m_fShakingTime = 0.f;
}

void CWoodPlatform::Collision_Stay(CGameObject* pOpponent)
{
	
}

void CWoodPlatform::Collision_Leave(CGameObject* pOpponent)
{
	
}

void CWoodPlatform::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
	m_vOriginPos = *pPos;
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pPos));
}

void CWoodPlatform::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
	m_vOriginPos = { fX, fY, fZ };
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&m_vOriginPos));
}

void CWoodPlatform::Set_Angle(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Angle(fX, fY, fZ);
}

CWoodPlatform * CWoodPlatform::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CWoodPlatform* pInst = new CWoodPlatform(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);
 
	return pInst;
}

void CWoodPlatform::Free()
{
	CStaticObject::Free();
}
