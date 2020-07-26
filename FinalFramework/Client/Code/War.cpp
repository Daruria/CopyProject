#include "stdafx.h"
#include "War.h"
#include "WarSword.h"

#include "Export_Function.h"
#include "MeshEffect.h"
#include "Particle.h"
#include "CylinderEffect.h"
#include "RockChips.h"
#include "CircularEffect.h"
#include "Thrust.h"
#include "HitGround.h"
#include "EarthSplitter.h"
#include "ChargeEffect.h"
#include "BrokenWood.h"
#include "PlayerPool.h"
#include "Dummy.h"

CWar::CWar(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCharacter(pGraphicDev)
{
}

CWar::~CWar()
{
}

HRESULT CWar::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(Engine::RESOURCE_STATIC, L"Mesh_War"), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);
	
	m_pTransformCom->Set_Scale(0.01f);
	
	m_eCurState = EWar::IDLE;

	m_wstrName = L"War";

	m_pEquipedWeapon = CWarSword::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipedWeapon, E_FAIL);
	const D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("Bone_War_Weapon_Sword");
	NULL_CHECK_RETURN(pFrame, E_FAIL);
	m_pEquipedWeapon->Set_Parent(&pFrame->CombinedTransformationMatrix, m_pTransformCom);
	m_pEquipedWeapon->Add_TransformFlags(TS_PARENT_NO_SCALE);

	m_iHP = 500;
	m_iHPMax = 500;

	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::HP), &m_iHP);
	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::HPMAX), &m_iHPMax);

	return S_OK;
}

_int CWar::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CCharacter::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = Update_Weapon(fTimeDelta);
	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CWar::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CWar::Render_GameObject(const _float& fTimeDelta)
{
	CCharacter::Render_GameObject(fTimeDelta);
}

void CWar::Hit(_int iAttack, _vec3 * pAtkPos)
{
	CCharacter::Hit(iAttack, pAtkPos);

	switch (m_eCurState.eWar)
	{
	case EWar::ATK_HEAVY_01:
	case EWar::ATK_HEAVY_01b:
	case EWar::ATK_HEAVY_02:
	case EWar::ATK_HEAVY_03:
	case EWar::ATK_LIGHT_01:
	case EWar::ATK_LIGHT_02:
	case EWar::ATK_LIGHT_02b:
	case EWar::ATK_LIGHT_03:
	case EWar::ATK_LIGHT_03_SINGLE:
	case EWar::ATK_LIGHT_04:
	case EWar::ATK_LIGHT_04_SINGLE:
	case EWar::ATK_CHARGE_START:
	case EWar::ATK_CHARGE_LOOP:
	case EWar::ATK_CHARGE_END:
	case EWar::ATK_FLAME_BRAND_START:
	case EWar::ATK_FLAME_BRAND_LOOP:
	case EWar::ATK_FLAME_BRAND_END:
		return;
	}

	_vec3 vPos, vDir, vLook, vRight;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));
	D3DXVec3Normalize(&vRight, m_pTransformCom->Get_Info(Engine::INFO_RIGHT));
	D3DXVec3Normalize(&vDir, &(vPos - *pAtkPos));

	_float fLookDot = D3DXVec3Dot(&vLook, &vDir);
	_float fRightDot = D3DXVec3Dot(&vRight, &vDir);

	UState ePreState = m_eCurState;

	m_eCurState = fabsf(fLookDot) > fabsf(fRightDot) ? (fLookDot > 0.f ? EPlayer::IMPACT_BACK : EPlayer::IMPACT_FRONT) :
														(fRightDot > 0.f ? EPlayer::IMPACT_LEFT : EPlayer::IMPACT_RIGHT);



	m_pEquipedWeapon->Stop();
}

HRESULT CWar::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag)
{
	FAILED_CHECK_RETURN(CCharacter::Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);



	return S_OK;
}

void CWar::Stop_Attack()
{
	m_pEquipedWeapon->Stop();

	m_bEffect = false;
}

void CWar::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	CCharacter::Change_State(SC_INT(m_eCurState.eWar));

	m_ePreState = m_eCurState;

	m_bEffect = false;
	m_bEffectSound = false;

	Engine::Stop_Sound(Engine::CSoundMgr::PLAYER_LOOP);
}

void CWar::Update_State(const _float& fTimeDelta)
{
	switch (m_eCurState.eWar)
	{
	case EWar::IDLE:				Update_Idle(fTimeDelta);				break;
	case EWar::RUN:					Update_Run(fTimeDelta);					break;
	case EWar::JUMP:				Update_Jump(fTimeDelta);				break;
	case EWar::FALL:				Update_Fall(fTimeDelta);				break;
	case EWar::DASH:				Update_Dash(fTimeDelta);				break;
	case EWar::TEST:				Update_Test(fTimeDelta);				break;
	case EWar::IMPACT_FRONT:		
	case EWar::IMPACT_BACK:
	case EWar::IMPACT_LEFT:
	case EWar::IMPACT_RIGHT:		Update_Impact(fTimeDelta);				break;

	case EWar::ATK_HEAVY_01:		Update_AtkHeavy01(fTimeDelta);			break;
	case EWar::ATK_HEAVY_01b:		break;
	case EWar::ATK_HEAVY_02:		Update_AtkHeavy02(fTimeDelta);			break;
	case EWar::ATK_HEAVY_03:		Update_AtkHeavy03(fTimeDelta);			break;
	case EWar::ATK_CHARGE_START:	Update_AtkChargeStart(fTimeDelta);		break;
	case EWar::ATK_CHARGE_LOOP:		Update_AtkChargeLoop(fTimeDelta);		break;
	case EWar::ATK_CHARGE_END:		Update_AtkChargeEnd(fTimeDelta);		break;

	case EWar::ATK_LIGHT_01:		Update_AtkLight01(fTimeDelta); 			break;
	case EWar::ATK_LIGHT_02:		Update_AtkLight02(fTimeDelta);			break;
	case EWar::ATK_LIGHT_02b:		Update_AtkLight02b(fTimeDelta);			break;
	case EWar::ATK_LIGHT_03:		Update_AtkLight03(fTimeDelta);			break;
	case EWar::ATK_LIGHT_03_SINGLE:	Update_AtkLight03Single(fTimeDelta);	break;
	case EWar::ATK_LIGHT_04:		Update_AtkLight04(fTimeDelta);			break;
	case EWar::ATK_LIGHT_04_SINGLE:	Update_AtkLight04Single(fTimeDelta);	break;

	case EWar::ATK_FLAME_BRAND_START:	Update_AtkFlameBrandStart(fTimeDelta);	break;
	case EWar::ATK_FLAME_BRAND_LOOP:	Update_AtkFlameBrandLoop(fTimeDelta);	break;
	case EWar::ATK_FLAME_BRAND_END:		Update_AtkFlameBrandEnd(fTimeDelta);	break;
	}
}

void CWar::KeyInput(const _float& fTimeDelta)
{
	switch (m_eCurState.eWar)
	{
	case EWar::IDLE:
	case EWar::RUN:
		KeyInput_Move(fTimeDelta);
		KeyInput_Jump(fTimeDelta);
		KeyInput_Dash(fTimeDelta);
		KeyInput_Test(fTimeDelta);
		KeyInput_AtkHeavy01(fTimeDelta);
		KeyInput_AtkLight01(fTimeDelta);
		KeyInput_AtkFlameBrandStart(fTimeDelta);
		break;
	case EWar::JUMP:
	case EWar::FALL:
		KeyInput_Move(fTimeDelta);
		break;
	case EWar::DASH:
	case EWar::IMPACT_FRONT:
	case EWar::IMPACT_BACK:
	case EWar::IMPACT_LEFT:
	case EWar::IMPACT_RIGHT:
		if (m_pMeshCom->Get_AnimationProgress() > 0.4)
		{
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkHeavy01(fTimeDelta);
			KeyInput_AtkLight01(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::TEST:
		break;
		
	case EWar::ATK_HEAVY_01:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_AtkChargeStart(fTimeDelta);
			KeyInput_AtkHeavy02(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_HEAVY_01b:
		break;
	case EWar::ATK_HEAVY_02:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_AtkChargeStart(fTimeDelta);
			KeyInput_AtkHeavy03(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_HEAVY_03:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_AtkChargeStart(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_CHARGE_END:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_01:
		if (m_pMeshCom->Get_AnimationProgress() > 0.25)
		{
			KeyInput_AtkLight02(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_02:
		if (m_pMeshCom->Get_AnimationProgress() > 0.35)
		{
			KeyInput_AtkLight03(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_02b:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_03:
		if (m_pMeshCom->Get_AnimationProgress() > 0.38)
		{
			//m_pEquipedWeapon->Stop();
			KeyInput_AtkLight04(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_03_SINGLE:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_04:
		if (m_pMeshCom->Get_AnimationProgress() > 0.4)
		{
			KeyInput_AtkLight01(fTimeDelta);
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_LIGHT_04_SINGLE:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_Move(fTimeDelta);
			KeyInput_Jump(fTimeDelta);
			KeyInput_Dash(fTimeDelta);
			KeyInput_Test(fTimeDelta);
			KeyInput_AtkFlameBrandStart(fTimeDelta);
		}
		break;
	case EWar::ATK_FLAME_BRAND_LOOP:
		KeyInput_AtkFlameBrandEnd(fTimeDelta);
		KeyInput_Move(fTimeDelta);
		break;
	case EWar::ATK_FLAME_BRAND_END:
		if (m_pMeshCom->Get_AnimationProgress() > 0.5)
		{
			KeyInput_Move(fTimeDelta);
		}
		break;
	}
}

_int CWar::Update_Weapon(const _float& fTimeDelta)
{
	if (!m_pEquipedWeapon)
		return 0;

	m_pEquipedWeapon->Update_GameObject(fTimeDelta);

	return 0;
}

void CWar::Update_Dash(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.ePlayer = EPlayer::IDLE;
	}

	if (m_pMeshCom->Get_AnimationProgress() > 0.4)
		m_pColliderCom->Activate();


	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress < (4504 - 4490) / 54.)
	{
		m_fDashTimeCount += fTimeDelta;
		if (m_fDashTimeCount > 0.05f)
		{
			CDummy* pDummy = nullptr;
			FAILED_CHECK_RETURN(CPlayerPool::GetInstance()->Get_Dummy(L"Dummy_War", &pDummy), );
			if (pDummy == nullptr)
				return;

			m_fDashTimeCount = 0.f;

			//pDummy->SetUp_Variables(0.2f, 1.f, 0.f, -4.f);
			pDummy->SetUp_Variables(0.2f, 0.f, 1.f, 0.f, -5.f);

			_vec3 vScale, vAngle, vPos;
			m_pTransformCom->Get_Scale(&vScale);
			m_pTransformCom->Get_Angle(&vAngle);
			m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
			pDummy->Set_Transform(&vScale, &vAngle, &vPos);

			_double dPosition = m_pMeshCom->Get_AnimationPosition();

			pDummy->SWSkinning(m_vecAnimSetIdx[SC_INT(m_eCurState.eWar)], 0.0000005f, dPosition);

			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Dummy", pDummy), );
		}
	}
}

_int CWar::Update_AtkHeavy01(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}
	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_attack_heavy_1.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.3 && dProgress < 0.5)
	{
		

		m_pEquipedWeapon->Attack();
	}
	else if (dProgress > 0.5)
		m_pEquipedWeapon->Stop();

	if (!m_bEffect && (956 - 937) / 53. < dProgress)
	{
		m_bEffect = true;

		_matrix matWeaponWorld;
		_vec3 vPos, vLook, vRot;

		m_pTransformCom->Get_WorldPos(&vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		m_pTransformCom->Get_Angle(&vRot);
		D3DXVec3Normalize(&vLook, &vLook);

		vPos.y += 0.8f;
		vPos += vLook * 2.f;

		vRot.x = D3DX_PI * 0.5f;
		CThrust* pThrust = CThrust::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", &vPos, &vLook, &vRot);
		NULL_CHECK_RETURN(pThrust, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Thrust", pThrust), -1);

		//CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.2f, 1.f, 2.f);
		//NULL_CHECK_RETURN(pEffect, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), -1);
		//pEffect->Set_Pos(&vPos);
		//pEffect->Set_Scale(1.f);
		//pEffect->Set_Speed(0.5f);
		//pEffect->Set_DeltaAlpha(-1.f);
		//pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
		////pEffect->Set_DirUV(&_vec2(1.f, 0.f));
		//pEffect->Set_RotationX(D3DX_PI * 0.5f);
		//pEffect->Set_RotationY(vRot.y);
		//pEffect->Set_LifeTime(0.5f);

		//vPos += vLook * 2.f;
		//pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.5f, 0.5f, 4.f);
		//NULL_CHECK_RETURN(pEffect, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), -1);
		//pEffect->Set_Pos(&vPos);
		//pEffect->Set_Scale(1.f);
		//pEffect->Set_Speed(0.5f);
		//pEffect->Set_DeltaAlpha(-1.f);
		//pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
		////pEffect->Set_DirUV(&_vec2(1.f, 0.f));
		//pEffect->Set_RotationX(D3DX_PI * 0.5f);
		//pEffect->Set_RotationY(vRot.y);
		//pEffect->Set_LifeTime(0.5f);

		//vPos += vLook * 2.f;
		//pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 100, 100, 0.1f, 0.5f, 2.f);
		//NULL_CHECK_RETURN(pEffect, -1);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Effect", pEffect), -1);
		//pEffect->Set_Pos(&vPos);
		//pEffect->Set_Scale(1.f);
		//pEffect->Set_Speed(0.5f);
		//pEffect->Set_DeltaAlpha(-1.f);
		//pEffect->Set_DeltaScale(&_vec3(0.5f, 0.5f, 0.5f));
		////pEffect->Set_DirUV(&_vec2(1.f, 0.f));
		//pEffect->Set_RotationX(D3DX_PI * 0.5f);
		//pEffect->Set_RotationY(vRot.y);
		//pEffect->Set_LifeTime(0.5f);
	}

	return 0;
}

_int CWar::Update_AtkHeavy02(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}	
	
	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_attack_heavy_1b.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.3 && dProgress < 0.6)
	{
	

		m_pEquipedWeapon->Attack();
	}
	else if (dProgress > 0.6)
		m_pEquipedWeapon->Stop();

	if (!m_bEffect && ((1070 - 1043) / 59. < dProgress))
	{
		m_bEffect = true;

		_vec3 vPos, vLook;
		m_pTransformCom->Get_WorldPos(&vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 2.5f;

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", 100, 100, 10.f, 5.f, 2.f);
		NULL_CHECK_RETURN(pTest, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->Set_DeltaScale(&_vec3(0.1f, 0.1f, 0.1f));
		pTest->Set_MaxScale(&_vec3(0.4f, 0.4f, 0.4f));
		pTest->Set_Scale(0.3f);

		//CRockChips*		pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 40, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
		//NULL_CHECK_RETURN(pRockChips, E_FAIL);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRockChips), -1);
		//pRockChips->Set_Pos(&vPos);
		//pRockChips->Set_Speed(100.f);
		//pRockChips->Set_Gravity(5.f);
		//pRockChips->Set_Scale(0.15f);
		//pRockChips->Set_LifeTime(1.f);
		
		for (_int i = 0; i < 15; ++i)
		{
			_vec3 vDir = _vec3(((rand() % 100) - 50.f) / 2.5f, ((rand() % 50)) / 100.f, ((rand() % 100) - 50.f) / 2.5f);
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= 4.f;

			_vec3 vRockPos = vPos + _vec3((rand() % 100 / 100.f) - 0.5f, 0.f, (rand() % 100 / 100.f) - 0.5f ) * 3.f;

			CBrokenWood* pRock = CBrokenWood::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rock_Single_0", L"Fx_Rocks", &vDir, 0.4f);
			NULL_CHECK_RETURN(pRock, -1);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRock), -1);
			pRock->Set_Pos(&vRockPos, m_pNaviCom->Get_Index());
			pRock->Set_Scale(0.002f);
		}


		CHitGround* pHitGround = CHitGround::Create(m_pGraphicDev, &vPos, 3.f, L"Player_Attack");
		NULL_CHECK_RETURN(pHitGround, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_HitGround", pHitGround), -1);
		pHitGround->Set_LifeTime(0.5f);
		pHitGround->Set_Attack(100);

		Engine::Play_Sound(L"char_war_land_explode.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CWar::Update_AtkHeavy03(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_attack_heavy_3.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.3 && dProgress < 0.5)
	{
		m_pEquipedWeapon->Attack();
	}
	else if (dProgress > 0.5)
		m_pEquipedWeapon->Stop();

	if (!m_bEffect && (1132 - 1102) / 88. < dProgress)
	{
		m_bEffect = true;

		_vec3 vPos, vLook;
		m_pTransformCom->Get_WorldPos(&vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 2.f;

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", 100, 100, 10.f, 5.f, 2.f);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->Set_DeltaScale(&_vec3(0.1f, 0.1f, 0.1f));
		pTest->Set_MaxScale(&_vec3(0.5f, 0.5f, 0.5f));
		pTest->Set_Scale(0.4f);

		//CRockChips*		pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 40);
		//NULL_CHECK_RETURN(pRockChips, E_FAIL);
		//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRockChips), -1);
		//pRockChips->Set_Pos(&vPos);
		//pRockChips->Set_Speed(50.f);
		//pRockChips->Set_Gravity(2.f);
		//pRockChips->Set_Scale(0.3f);
		//pRockChips->Set_LifeTime(1.f);
		
		for (_int i = 0; i < 15; ++i)
		{
			_vec3 vDir = _vec3(((rand() % 100) - 50.f) / 2.5f, ((rand() % 50)) / 100.f, ((rand() % 100) - 50.f) / 2.5f);
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= 4.f;

			_vec3 vRockPos = vPos + _vec3((rand() % 100 / 100.f) - 0.5f, 0.f, (rand() % 100 / 100.f) - 0.5f) * 3.f;

			CBrokenWood* pRock = CBrokenWood::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rock_Single_0", L"Fx_Rocks", &vDir, 0.4f);
			NULL_CHECK_RETURN(pRock, -1);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRock), -1);
			pRock->Set_Pos(&vRockPos, m_pNaviCom->Get_Index());
			pRock->Set_Scale(0.002f);
		}

		CHitGround* pHitGround = CHitGround::Create(m_pGraphicDev, &vPos, 4.f, L"Player_Attack");
		NULL_CHECK_RETURN(pHitGround, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_HitGround", pHitGround), -1);
		pHitGround->Set_LifeTime(0.5f);
		pHitGround->Set_Attack(100);

		Engine::Play_Sound(L"char_war_land_explode.ogg", Engine::CSoundMgr::EFFECT);

		Engine::Get_MainCamera()->CameraShake();
		m_pRenderer->On_Blur();

		_matrix matRotY;
		for (_int i = 0; i < 10; ++i)
		{
			wstring wstrBufferTag = rand() % 2 ? L"Mesh_Rock_Single_0" : L"Mesh_Rock_Single_1";

			CMeshEffect* pMeshEffect = CMeshEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferTag, L"Rock_Single");
			NULL_CHECK_RETURN(pMeshEffect, -1);
			Engine::Add_GameObject(L"Layer_GameLogic", L"FX_Rocks", pMeshEffect);

			_float fAngle = i * (2.f * D3DX_PI) / 10;
			D3DXMatrixRotationY(&matRotY, fAngle);
			_vec3 vRockPos = { 0.f, 0.f, 2.f };
			D3DXVec3TransformCoord(&vRockPos, &vRockPos, &matRotY);
			vRockPos += vPos;
			pMeshEffect->Set_Pos(&vRockPos);
			pMeshEffect->Set_RotationY(fAngle);
			pMeshEffect->SetUp_ScaleChange(&_vec3(0.001f, 0.001f, 0.001f), &_vec3(0.01f, 0.01f, 0.01f), 20.f);
			pMeshEffect->Set_LifeTime(2.f);
			pMeshEffect->Set_DissolveStartTime(1.f);
		}
	}


	return 0;
}

_int CWar::Update_AtkChargeStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.eWar = EWar::ATK_CHARGE_LOOP;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_special_charge.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CWar::Update_AtkChargeLoop(const _float & fTimeDelta)
{
	if (!Engine::MousePress(DIM_RB))
	{
		m_eCurState.eWar = EWar::ATK_CHARGE_END;
		m_bEffect = false;
		m_bEffectSound = false;
		Engine::Stop_Sound(Engine::CSoundMgr::PLAYER_LOOP);
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_special_charge_loop.ogg", Engine::CSoundMgr::PLAYER_LOOP);
	}

	m_fChargeTime += fTimeDelta;

	if (m_fChargeTime > 0.25f)
	{
		m_fChargeTime = 0.f;
		_vec3 vPos, vUp;
		m_pEquipedWeapon->Get_Pos(&vPos);
		 
		_matrix matWorld;
		m_pEquipedWeapon->Get_WorldMatrix(&matWorld);
		memcpy(&vUp, matWorld.m[1], sizeof(_vec3));
		D3DXVec3Normalize(&vUp, &vUp);
		_float fRandom = rand() % 100 / 50.f;
		vPos += vUp * -fRandom;

		CChargeEffect* pEffect = CChargeEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Trail2", D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		NULL_CHECK_RETURN(pEffect, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Charging", pEffect), -1);
		pEffect->Set_Pos(&vPos);
		pEffect->Set_LifeTime(1.f);

		_vec3 vCharPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vCharPos);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", 100, 100, 10.f, 5.f, 2.f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		pTest->Set_Pos(&vCharPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(0.5f, 0.5f, 0.5f), 1.f);
	}
	return 0;
}

_int CWar::Update_AtkChargeEnd(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((640 - 635) / 72. < dProgress && dProgress < (647 - 635) / 72.)
		m_pEquipedWeapon->Attack();
	else
		m_pEquipedWeapon->Stop();
	
	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_earthsplitter_1.ogg", Engine::CSoundMgr::EFFECT);
	}

	if (!m_bEffect && ((640 - 635) / 72. < dProgress))
	{
		m_bEffect = true;

		_vec3 vPos, vLook, vAngle;
		m_pTransformCom->Get_WorldPos(&vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		m_pTransformCom->Get_Angle(&vAngle);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 1.f;

		CEarthSplitter* pEarthSplitter = CEarthSplitter::Create(m_pGraphicDev, &vPos, &vLook, &vAngle);
		NULL_CHECK_RETURN(pEarthSplitter, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Skill_EarthSplitter", pEarthSplitter), -1);
		pEarthSplitter->Set_Navi(m_pNaviCom->Get_Index());

	}

	return 0;
}

_int CWar::Update_AtkLight01(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.0 && dProgress < 0.3)
		m_pEquipedWeapon->Attack();
	else if (dProgress > 0.3)
		m_pEquipedWeapon->Stop();

	if ((1312 - 1307) / 64. < dProgress)
	{
		if (!m_bEffect)
		{
			m_bEffect = true;

			_vec3 vPos, vLook, vRot;
			
			m_pTransformCom->Get_WorldPos(&vPos);
			m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos.y += 1.f;
			vPos += vLook * 2.f;

			m_pTransformCom->Get_Angle(&vRot);

		}
	}

	return 0;
}

_int CWar::Update_AtkLight02(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.0 && dProgress < 0.35)
		m_pEquipedWeapon->Attack();
	else if (dProgress > 0.35)
		m_pEquipedWeapon->Stop();

	return 0;
}

_int CWar::Update_AtkLight02b(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.1 && dProgress < 0.8)
		m_pEquipedWeapon->Attack();
	else if (dProgress > 0.8)
		m_pEquipedWeapon->Stop();

	return 0;
}

_int CWar::Update_AtkLight03(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.0 && dProgress < 0.4)
		m_pEquipedWeapon->Attack();
	else if (dProgress > 0.4)
		m_pEquipedWeapon->Stop();

	return 0;
}

_int CWar::Update_AtkLight03Single(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.1 && dProgress < 0.8)
		m_pEquipedWeapon->Attack();
	else if (dProgress > 0.8)
		m_pEquipedWeapon->Stop();

	return 0;
}

_int CWar::Update_AtkLight04(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.0 && dProgress < (1620 - 1584) / 74.)
		m_pEquipedWeapon->Attack();
	else
	{
		m_pEquipedWeapon->Stop();
	}
	
	if (dProgress > (1613 - 1584) / 74.)
	{
		if (!m_bEffect)
		{
			m_bEffect = true;
			_vec3 vPos, vLook;
			m_pTransformCom->Get_WorldPos(&vPos);
			m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vPos += vLook * 2.5f;

			CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", 100, 100, 10.f, 5.f, 2.f);
			NULL_CHECK_RETURN(pTest, E_FAIL);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
			pTest->Set_Pos(&vPos);
			pTest->Set_DeltaAlpha(-1.f);
			pTest->Set_LifeTime(1.f);
			pTest->Set_DeltaScale(&_vec3(0.05f, 0.05f, 0.05f));
			pTest->Set_MaxScale(&_vec3(0.35f, 0.35f, 0.35f));
			pTest->Set_Scale(0.3f);

			//CRockChips*		pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 40);
			//NULL_CHECK_RETURN(pRockChips, E_FAIL);
			//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRockChips), -1);
			//pRockChips->Set_Pos(&vPos);
			//pRockChips->Set_Speed(50.f);
			//pRockChips->Set_Gravity(2.f);
			//pRockChips->Set_Scale(0.3f);
			//pRockChips->Set_LifeTime(1.f);

			for (_int i = 0; i < 15; ++i)
			{
				_vec3 vDir = _vec3(((rand() % 100) - 50.f) / 2.5f, ((rand() % 50)) / 100.f, ((rand() % 100) - 50.f) / 2.5f);
				D3DXVec3Normalize(&vDir, &vDir);
				vDir *= 4.f;

				_vec3 vRockPos = vPos + _vec3((rand() % 100 / 100.f) - 0.5f, 0.f, (rand() % 100 / 100.f) - 0.5f) * 3.f;

				CBrokenWood* pRock = CBrokenWood::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Rock_Single_0", L"Fx_Rocks", &vDir, 0.4f);
				NULL_CHECK_RETURN(pRock, -1);
				FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_RockChips", pRock), -1);
				pRock->Set_Pos(&vRockPos, m_pNaviCom->Get_Index());
				pRock->Set_Scale(0.002f);
			}

			CHitGround* pHitGround = CHitGround::Create(m_pGraphicDev, &vPos, 3.f, L"Player_Attack");
			NULL_CHECK_RETURN(pHitGround, -1);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_HitGround", pHitGround), -1);
			pHitGround->Set_LifeTime(0.5f);
			pHitGround->Set_Attack(100);

			Engine::Play_Sound(L"char_war_land_explode.ogg", Engine::CSoundMgr::EFFECT);
		}

	}
	return 0;
}

_int CWar::Update_AtkLight04Single(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.1 && dProgress < 0.8)
		m_pEquipedWeapon->Attack();
	else if (dProgress > 0.8)
		m_pEquipedWeapon->Stop();

	return 0;
}

_int CWar::Update_AtkFlameBrandStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Attack();
		m_eCurState.eWar = EWar::ATK_FLAME_BRAND_LOOP;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_flamebrand_start_01.ogg", Engine::CSoundMgr::EFFECT);
	}


	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (0.5 < dProgress)
	{
		m_pEquipedWeapon->Attack();
	}
	return 0;
}

_int CWar::Update_AtkFlameBrandLoop(const _float & fTimeDelta)
{
	m_pEquipedWeapon->Attack();


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_flamebrand_level1_loop.ogg", Engine::CSoundMgr::PLAYER_LOOP);
	}

	return 0;
}

_int CWar::Update_AtkFlameBrandEnd(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pEquipedWeapon->Stop();
		m_eCurState.eWar = EWar::IDLE;
		m_bEffect = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"char_war_flamebrand_end_level1.ogg", Engine::CSoundMgr::EFFECT);
	}

	m_pEquipedWeapon->Attack();

	return 0;
}

void CWar::KeyInput_AtkHeavy01(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_RB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_HEAVY_01;
		m_bEffect = false;
		m_bEffectSound = false;
	}

}

void CWar::KeyInput_AtkHeavy02(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_RB))
	{
		Turn_ClickPos(fTimeDelta);

		m_eCurState.eWar = EWar::ATK_HEAVY_02;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkHeavy03(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_RB))
	{
		Turn_ClickPos(fTimeDelta);

		m_eCurState.eWar = EWar::ATK_HEAVY_03;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkChargeStart(const _float & fTimeDelta)
{
	if (Engine::Get_MouseAccTime(DIM_RB) > 0.5f)
	{
		Turn_ClickPos(fTimeDelta);

		m_eCurState.eWar = EWar::ATK_CHARGE_START;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkLight01(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_01;
		m_bEffect = false;
		
		Engine::Play_Sound(L"char_war_attack_1_01.ogg", Engine::CSoundMgr::EFFECT);
	}
}

void CWar::KeyInput_AtkLight02(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_02;
		m_bEffect = false;

		Engine::Play_Sound(L"char_war_attack_2_01.ogg", Engine::CSoundMgr::EFFECT);
	}
}

void CWar::KeyInput_AtkLight02b(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_02b;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkLight03(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_03;
		m_bEffect = false;

		Engine::Play_Sound(L"char_war_attack_3_01.ogg", Engine::CSoundMgr::EFFECT);
	}
}

void CWar::KeyInput_AtkLight03Single(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_03_SINGLE;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkLight04(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_04;
		m_bEffect = false;

		Engine::Play_Sound(L"char_war_attack_4_b_01.ogg", Engine::CSoundMgr::EFFECT);
	}
}

void CWar::KeyInput_AtkLight04Single(const _float & fTimeDelta)
{
	if (Engine::MouseDown(DIM_LB))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_LIGHT_04_SINGLE;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkFlameBrandStart(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_Q))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_FLAME_BRAND_START;
		m_bEffect = false;
	}
}

void CWar::KeyInput_AtkFlameBrandEnd(const _float & fTimeDelta)
{
	if (Engine::KeyDown(DIK_Q))
	{
		Turn_ClickPos(fTimeDelta);
		m_eCurState.eWar = EWar::ATK_FLAME_BRAND_END;
		m_bEffect = false;
	}
}

HRESULT CWar::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_WSTATE(FINISH));
	m_vecAnimSetType.resize(SC_WSTATE(FINISH));
	m_vecAnimItpl.resize(SC_WSTATE(FINISH));

	Set_AnimVariable(SC_WSTATE(IDLE), 178, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_WSTATE(RUN), 261, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_WSTATE(JUMP), 190, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(FALL), 190, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(DASH), 106, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(TEST), m_uiTestAniIndex, AT_REPEAT, 0.05);

	Set_AnimVariable(SC_WSTATE(IMPACT_FRONT), 186, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(IMPACT_BACK), 187, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(IMPACT_LEFT), 188, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(IMPACT_RIGHT), 189, AT_ONCE, 0.05);

	Set_AnimVariable(SC_WSTATE(ATK_HEAVY_01), 31, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_HEAVY_01b), 32, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_HEAVY_02), 33, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_HEAVY_03), 34, AT_ONCE, 0.05);

	Set_AnimVariable(SC_WSTATE(ATK_CHARGE_START),	24, AT_ONCE,	0.05);
	Set_AnimVariable(SC_WSTATE(ATK_CHARGE_LOOP),	23, AT_REPEAT,	0.05);
	Set_AnimVariable(SC_WSTATE(ATK_CHARGE_END),		25, AT_ONCE,	0.05);

	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_01), 38, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_02), 39, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_02b), 40, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_03), 41, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_03_SINGLE), 42, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_04), 43, AT_ONCE, 0.05);
	Set_AnimVariable(SC_WSTATE(ATK_LIGHT_04_SINGLE), 44, AT_ONCE, 0.05);

	Set_AnimVariable(SC_WSTATE(ATK_FLAME_BRAND_START),	30, AT_ONCE, 0.0000005);
	Set_AnimVariable(SC_WSTATE(ATK_FLAME_BRAND_LOOP),	28, AT_REPEAT, 0.00000005);
	Set_AnimVariable(SC_WSTATE(ATK_FLAME_BRAND_END),	29, AT_ONCE, 0.05);


	return S_OK;
}

void CWar::Turn_ClickPos(const _float & fTimeDelta)
{
	_vec2 vMousePos;
	CMyMath::ClientMousePos(g_hWnd, &vMousePos);

	_vec3 vPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook));

	_float fY = m_pNaviCom->Get_Height(&vPos);
	D3DXPLANE Plane = { 0.f, 1.f, 0.f, -fY };

	_vec3 vPickingPos;
	CMyMath::PickingOnPlane(&vPickingPos, &vMousePos, WINCX, WINCY, Engine::Get_MainCamera()->Get_Projection(), Engine::Get_MainCamera()->Get_View(), &Plane);

	D3DXVec3Normalize(&vDir, &(vPickingPos - vPos));

	_float fAngle = CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

	m_pTransformCom->Rotation(ROT_Y, fAngle);
}

CWar * CWar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWar* pInst = new CWar(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CWar::Free()
{
	::Safe_Release(m_pEquipedWeapon);
	CCharacter::Free();
}
