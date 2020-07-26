#include "stdafx.h"
#include "ThrowSpear.h"

#include "Transform.h"
#include "CylinderEffect.h"

#include "Export_Function.h"
#include "SparkEffect.h"
#include "RockChips.h"
#include "SphereCollider.h"
#include "HitGround.h"

CThrowSpear::CThrowSpear(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CThrowSpear::~CThrowSpear()
{
}

HRESULT CThrowSpear::Ready_GameObject(const _vec3* pStartPos, const _vec3* pMoveDir, const _vec3* pAngle, const _float& fSpeed, const _float& fHeight)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(pStartPos);
	
	m_vMoveDir = *pMoveDir;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
	
	m_vAngle = *pAngle;
	m_vAngle.x += D3DX_PI * 0.5f;


	m_fSpeed = fSpeed;
	m_fHeight = fHeight;

	return S_OK;
}

_int CThrowSpear::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fLifeCnt += fTimeDelta;

	if (m_fLifeCnt > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	m_pTransformCom->Move_Pos(m_vMoveDir * m_fSpeed * fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//if (m_fFxItvalTime + 0.1f < m_fLifeCnt)
	{
		m_fFxItvalTime = m_fLifeCnt;

		CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 50, 20, 0.5f, 1.f, 1.f);
		NULL_CHECK_RETURN(pEffect, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Throw", pEffect), -1);
		pEffect->Set_Pos(&vPos);
		pEffect->Set_Rotation(&m_vAngle);
		pEffect->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(3.f, 3.f, 3.f), 2.f);
		pEffect->Set_DeltaAlpha(-2.f);
		pEffect->Set_LifeTime(0.5f);
	}

	if (m_fHeight > vPos.y)
	{
		m_bIsDead = true;

		for (_int i = 0; i < 4; ++i)
		{
			_vec3 vRandMove = { (rand() % 100) / 50.f - 1.f, 0.f, (rand() % 100) / 50.f - 1.f };

			CSparkEffect* pSpark = CSparkEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Spark",
				D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2((rand() % 2) * 0.5f, (rand() % 2) * 0.5f), &_vec2(0.5f, 0.5f));
			NULL_CHECK_RETURN(pSpark, -1);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Spark", pSpark), -1);
			pSpark->Set_Scale(2.f);
			pSpark->Set_Pos(&(vPos + vRandMove));
			pSpark->Set_LifeTime(2.f);
			pSpark->Set_Speed(3.f);
		}

		CRockChips*		pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 40);
		NULL_CHECK_RETURN(pRockChips, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRockChips), -1);
		pRockChips->Set_Pos(&vPos);
		pRockChips->Set_Speed(50.f);
		pRockChips->Set_Gravity(2.f);
		pRockChips->Set_Scale(0.3f);
		pRockChips->Set_LifeTime(1.f);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", 100, 100, 10.f, 5.f, 2.f);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->Set_DeltaScale(&_vec3(0.05f, 0.05f, 0.05f));
		pTest->Set_MaxScale(&_vec3(0.35f, 0.35f, 0.35f));
		pTest->Set_Scale(0.3f);

		CHitGround* pHitGround = CHitGround::Create(m_pGraphicDev, &vPos, 3.f, L"Monster_Attack");
		NULL_CHECK_RETURN(pHitGround, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_HitGround", pHitGround), -1);
		pHitGround->Set_LifeTime(0.5f);
		pHitGround->Set_Attack(20);
		return 0;
	}

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (0x80000000 & iExit)
		return -1;

	return 0;
}

_int CThrowSpear::LateUpdate_GameObject(const _float & fTimeDelta)
{
	_int iExit = Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (0x80000000 & iExit)
		return -1;

	return 0;
}

void CThrowSpear::Render_GameObject(const _float & fTimeDelta)
{
	return ;
}

HRESULT CThrowSpear::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	_matrix matTemp;
	D3DXMatrixIdentity(&matTemp);
	//matTemp._42 = -1.2f;

	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_TRANSFORM, m_pTransformCom, &matTemp, 3.f, L"Col_UndeadSpear");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pComponent);


	return S_OK;
}

CThrowSpear * CThrowSpear::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartPos, const _vec3* pMoveDir, const _vec3* pAngle, const _float& fSpeed, const _float& fHeight)
{
	CThrowSpear* pInstance = new CThrowSpear(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pStartPos, pMoveDir, pAngle, fSpeed, fHeight)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CThrowSpear::Free()
{
	Engine::CGameObject::Free();
}
