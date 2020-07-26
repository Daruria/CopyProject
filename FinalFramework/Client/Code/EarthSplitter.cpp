#include "stdafx.h"
#include "EarthSplitter.h"

#include "Export_Function.h"
#include "CylinderEffect.h"
#include "MultiCollider.h"
#include "RockChips.h"
#include "MeshEffect.h"
#include "BrokenWood.h"

CEarthSplitter::CEarthSplitter(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSkill(pGraphicDev)
{
}

CEarthSplitter::~CEarthSplitter()
{
}

HRESULT CEarthSplitter::Ready_GameObject(const _vec3* pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR& dwColor)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_iAttack = 300;
	m_fLifeTime = 0.5f;
	m_fLifeCount = 0.f;

	_vec3 vPos = *pPos;

	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Angle(pRot);


	return S_OK;
}

_int CEarthSplitter::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	if (m_fIntervalTime + 0.05f < m_fLifeCount)
	{
		m_fIntervalTime = m_fLifeCount;
		Add_Effect();
	}

	_vec3 vLook;
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
	m_pTransformCom->Move_Pos(vLook * m_fSpeed * fTimeDelta);



	_int iExit = CSkill::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	Engine::Add_GameObject_To_CollisionList(L"Player_Attack", this);

	return 0;
}

_int CEarthSplitter::LateUpdate_GameObject(const _float & fTimeDelta)
{
	_int iExit = CSkill::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CEarthSplitter::Render_GameObject(const _float & fTimeDelta)
{
	
}

HRESULT CEarthSplitter::Add_Component()
{
	FAILED_CHECK_RETURN(CSkill::Add_Component(), E_FAIL);

	Engine::CComponent* pComponent = nullptr;

	_matrix matLocal;
	D3DXMatrixIdentity(&matLocal);

	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_TRANSFORM, m_pTransformCom, &matLocal, 2.f, L"Col_EarthSplitter");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pComponent);

	m_pColliderCom->Activate();
	return S_OK;
}

void CEarthSplitter::Add_Effect()
{
	_vec3 vPos, vAngle;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	m_pTransformCom->Get_Angle(&vAngle);

	CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.2f, 1.f, 2.f);
	NULL_CHECK_RETURN(pEffect, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), );
	pEffect->Set_Pos(&vPos);
	pEffect->Set_Scale(1.f);
	pEffect->Set_Speed(0.5f);
	pEffect->Set_DeltaAlpha(-1.f);
	pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
	pEffect->Set_MaxScale(&_vec3(1.25f, 1.25f, 1.25f));
	//pEffect->Set_DirUV(&_vec2(1.f, 0.f));
	pEffect->Set_Rotation(&vAngle);
	pEffect->Set_LifeTime(0.5f);


	Add_RockMeshEffect(&vPos, &vAngle, D3DX_PI * 0.5f);
	Add_RockMeshEffect(&vPos, &vAngle, D3DX_PI * -0.5f);


	//CRockChips*		pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 10);
	//NULL_CHECK_RETURN(pRockChips, );
	//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRockChips), );
	//pRockChips->Set_Pos(&vPos);
	//pRockChips->Set_Speed(50.f);
	//pRockChips->Set_Gravity(2.f);
	//pRockChips->Set_Scale(0.3f);
	//pRockChips->Set_LifeTime(1.f);

	for (_int i = 0; i < 3; ++i)
	{
		_vec3 vDir = _vec3(((rand() % 100) - 50.f) / 2.5f, ((rand() % 50)) / 100.f, ((rand() % 100) - 50.f) / 2.5f);
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= 4.f;

		_vec3 vRockPos = vPos + _vec3((rand() % 100 / 100.f) - 0.5f, 0.f, (rand() % 100 / 100.f) - 0.5f) * 3.f;

		CBrokenWood* pRock = CBrokenWood::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rock_Single_0", L"Fx_Rocks", &vDir, 0.4f);
		NULL_CHECK_RETURN(pRock, );
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRock), );
		pRock->Set_Pos(&vRockPos, m_iNavi);
		pRock->Set_Scale(0.002f);
	}
}

void CEarthSplitter::Add_RockMeshEffect(const _vec3* pPos, const _vec3* pAngle, const _float & fTurnAngle)
{
	_matrix matRotY;
	wstring wstrBufferTag = rand() % 2 ? L"Mesh_Rock_Single_0" : L"Mesh_Rock_Single_1";

	CMeshEffect* pMeshEffect = CMeshEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Rock_Single");
	NULL_CHECK_RETURN(pMeshEffect, );
	Engine::Add_GameObject(L"Layer_GameLogic", L"FX_Rocks", pMeshEffect);

	_float fAngle = pAngle->y + fTurnAngle;
	D3DXMatrixRotationY(&matRotY, fAngle);
	_vec3 vRockPos = { 0.f, 0.f, 1.f };
	D3DXVec3TransformCoord(&vRockPos, &vRockPos, &matRotY);
	vRockPos += *pPos;
	pMeshEffect->Set_Pos(&vRockPos);
	pMeshEffect->Set_RotationY(fAngle);
	pMeshEffect->SetUp_ScaleChange(&_vec3(0.001f, 0.001f, 0.001f), &_vec3(0.01f, 0.01f, 0.01f), 20.f);
	pMeshEffect->Set_LifeTime(2.f);
	pMeshEffect->Set_DissolveStartTime(1.f);
}

CEarthSplitter * CEarthSplitter::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, const _vec3* pLook, const _vec3* pRot, const D3DXCOLOR & dwColor)
{
	CEarthSplitter* pInstance = new CEarthSplitter(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPos, pLook, pRot, dwColor)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CEarthSplitter::Free()
{
	CSkill::Free();
}
