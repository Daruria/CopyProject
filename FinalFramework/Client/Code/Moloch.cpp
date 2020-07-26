#include "stdafx.h"
#include "Moloch.h"
#include "SphereCollider.h"
#include "DynamicMesh.h"
#include "Transform.h"

#include "Export_Function.h"
#include "Observer.h"
#include "MolochSword.h"
#include "CutSceneCam.h"
#include "BossSubject.h"
#include "BossUI.h"
#include "MeshEffect.h"
#include "SphereEffect.h"
#include "RockChips.h"
#include "CylinderEffect.h"
#include "HitGround.h"
#include "UVEffect.h"

CMoloch::CMoloch(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CMoloch::~CMoloch()
{
}

HRESULT CMoloch::Ready_GameObject()
{
	return S_OK;
}

HRESULT CMoloch::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pSpawnPos)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);

	m_pTransformCom->Set_Pos(pSpawnPos);
	memcpy(&m_vSpawnPos, pSpawnPos, sizeof(_vec3));

	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(0, 0.05);
	m_eCurState = EMoloch::INTRO;

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	m_pEquipWeapon = CMolochSword::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipWeapon, E_FAIL);

	BONE_LINKER*	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Moloch_Weapon_Blade")->CombinedTransformationMatrix;
	m_pEquipWeapon->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pEquipWeapon->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_fSpeed = 10.f;
	m_iHP = 5000;
	m_iHPMax = 5000;
	m_bFullMode = false;

	m_iGeyserCount = 0;

	Engine::Add_Subject(L"Boss", CBossSubject::Create());
	Engine::Update_Subject(L"Boss", SC_INT(EBossData::TRANSFORM), m_pTransformCom);
	_ulong dwIndex = m_pNaviCom->Get_Index();
	Engine::Update_Subject(L"Boss", SC_INT(EBossData::NAVI_INDEX), &dwIndex);
	
	Engine::Update_Subject(L"Boss", SC_INT(EBossData::HP),		&m_iHP);
	Engine::Update_Subject(L"Boss", SC_INT(EBossData::HPMAX),	&m_iHPMax);

	return S_OK;
}

_int CMoloch::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Update_State(fTimeDelta);
	Change_State();

	if (Engine::KeyDown(DIK_G))
		m_iHP /= 2;

	_int iExit = CMonster::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = Update_Weapon(fTimeDelta);
	if (iExit & 0x80000000)
		return -1;

	if (m_eCurState != EMoloch::DEATH01)
		Gravity(fTimeDelta);
	//On_NaviMesh();

	Engine::Add_GameObject_To_CollisionList(L"Monster_Body", this);

	return 0;
}

_int CMoloch::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CMonster::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CMoloch::Render_GameObject(const _float& fTimeDelta)
{
	CMonster::Render_GameObject(fTimeDelta);
}

HRESULT CMoloch::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	_ulong dwFlag = TS_RENDEROFFSET;
	FAILED_CHECK_RETURN(CMonster::Add_Component(wContainerIdx, wstrBufferTag, dwFlag), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matColl;
	D3DXMatrixIdentity(&matColl);
	//matColl._42 = 0.5f;
	pCom = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_BONE, m_pTransformCom, &matColl, 3.f, L"Col_Monster");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);

	m_pColliderCom->Activate();

	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));

	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("null_Spine")->CombinedTransformationMatrix;
	m_pColliderCom->Set_Bone(&pLinker->matLink);
	m_LinkList.push_back(pLinker);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DX_PI));

	return S_OK;
}

void CMoloch::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[SC_INT(m_eCurState)], m_vecAnimItpl[SC_INT(m_eCurState)]);
	m_eCurAnimType = m_vecAnimSetType[SC_INT(m_eCurState)];

	if (m_eCurState != EMoloch::ATTACK02 && m_eCurState != EMoloch::ATTACK03)
		m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
	Attack_Stop();

	m_ePreState = m_eCurState;
}

_int CMoloch::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CMoloch::EMoloch::IDLE:				iExit = Update_Idle(fTimeDelta);			break;
	case CMoloch::EMoloch::RUN:					iExit = Update_Run(fTimeDelta);				break;
	case CMoloch::EMoloch::FALL:				iExit = Update_Fall(fTimeDelta);			break;
	case CMoloch::EMoloch::IMPACT:				iExit = Update_Impact(fTimeDelta);			break;
	case CMoloch::EMoloch::DEATH01:				iExit = Update_Death(fTimeDelta);			break;
	case CMoloch::EMoloch::INTRO:				iExit = Update_Intro(fTimeDelta);			break;

	case CMoloch::EMoloch::ATTACK_180_L:		iExit = Update_Attack180L(fTimeDelta);		break;
	case CMoloch::EMoloch::ATTACK_180_R:		iExit = Update_Attack180R(fTimeDelta);		break;

	case CMoloch::EMoloch::BLADE_EXTEND:		iExit = Update_BladeExtend(fTimeDelta);		break;
	case CMoloch::EMoloch::CRYSTAL_ERUPTION:	iExit = Update_CrystalEruption(fTimeDelta);	break;
	case CMoloch::EMoloch::CRYSTAL_ERUPTION2:	iExit = Update_CrystalEruption2(fTimeDelta);break;
	case CMoloch::EMoloch::TREMOR_PULSE:		iExit = Update_TremorPulse(fTimeDelta);		break;

	case CMoloch::EMoloch::ATTACK01:			iExit = Update_Attack01(fTimeDelta);		break;
	case CMoloch::EMoloch::ATTACK02:			iExit = Update_Attack02(fTimeDelta);		break;
	case CMoloch::EMoloch::ATTACK03:			iExit = Update_Attack03(fTimeDelta);		break;

	case CMoloch::EMoloch::DASH_START:			iExit = Update_DashStart(fTimeDelta);		break;
	case CMoloch::EMoloch::DASH_HOLD:			iExit = Update_DashHold(fTimeDelta);		break;
	case CMoloch::EMoloch::DASH_STRIKE:			iExit = Update_DashStrike(fTimeDelta);		break;

	case CMoloch::EMoloch::FULL_DASH_START:		iExit = Update_FullDashStart(fTimeDelta);	break;
	case CMoloch::EMoloch::FULL_DASH:			iExit = Update_FullDash(fTimeDelta);		break;
	case CMoloch::EMoloch::FULL_DASH_STRIKE:	iExit = Update_FullDashStrike(fTimeDelta);	break;
	case CMoloch::EMoloch::FULL_DASH_STRIKE2:	iExit = Update_FullDashStrike2(fTimeDelta);	break;

	case CMoloch::EMoloch::FULL_GEYSER_START:	iExit = Update_FullGeyserStart(fTimeDelta);	break;
	case CMoloch::EMoloch::FULL_GEYSER:			iExit = Update_FullGeyser(fTimeDelta);		break;
	case CMoloch::EMoloch::FULL_GEYSER2:		iExit = Update_FullGeyser2(fTimeDelta);		break;

	case CMoloch::EMoloch::FULL_SWING1:			iExit = Update_FullSwing1(fTimeDelta);		break;
	case CMoloch::EMoloch::FULL_SWING2:			iExit = Update_FullSwing2(fTimeDelta);		break;
	case CMoloch::EMoloch::FULL_SWING3:			iExit = Update_FullSwing3(fTimeDelta);		break;

	case CMoloch::EMoloch::FULL_ATK_180_R:		iExit = Update_FullAtk180R(fTimeDelta);		break;
	case CMoloch::EMoloch::FULL_IDLE:			iExit = Update_FullIdle(fTimeDelta);		break;
	case CMoloch::EMoloch::FULL_RUN:			iExit = Update_FullRun(fTimeDelta);			break;
	case CMoloch::EMoloch::EEND:
		break;
	}


	return iExit;
}

void CMoloch::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;
	Engine::Update_Subject(L"Boss", SC_INT(EBossData::HP), &m_iHP);
	//m_pEquipWeapon->Stop();

	m_fRim = 1.f;
	Add_DamageFont(iAttack);
	Add_BloodParticle(pAtkPos);

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_eCurState = EMoloch::DEATH01;
		m_pColliderCom->Deactivate();
	}
	//else
	//{
	//	if (m_eCurState == EMoloch::IMPACT)
	//	{
	//		CMonster::Change_State(SC_INT(EMoloch::IDLE));
	//		CMonster::Change_State(SC_INT(EMoloch::IMPACT));
	//	}
	//	m_eCurState = EMoloch::IMPACT;
	//}
}

void CMoloch::On_NaviMesh()
{
	if (m_eCurState == EMoloch::DEATH01)
		return;

	switch (m_pNaviCom->Get_Option())
	{
	case Engine::CCell::NO_OPTION:
	{
		_float fY = m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS));
		m_pTransformCom->Set_PosY(fY);
		m_fGravity = 0.f;
	}
		break;

	case Engine::CCell::FALL:
	{
		if (m_eCurState == EMoloch::IDLE)
			m_eCurState = EMoloch::FALL;
	}
		break;
	}
}

void CMoloch::PlayAnimation(const _float & fTimeDelta)
{
	_vec3 vChangedPos = { 0.f, 0.f, 0.f }, vPos, vTargetPos;
	switch (m_eCurAnimType)
	{
	case AT_REPEAT:
		m_pMeshCom->Play_Animation(fTimeDelta, &vChangedPos);
		break;
	case AT_ONCE:
		if (!m_pMeshCom->Is_AnimationSetEnd())
			m_pMeshCom->Play_Animation(fTimeDelta, &vChangedPos);
		else
			m_pMeshCom->Play_Animation(0.f, &vChangedPos);
		break;
	}

	if (m_pMeshCom->Is_InItpProgress())
		return;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	if (vChangedPos.x || vChangedPos.y || vChangedPos.z)
	{
		D3DXVec3TransformNormal(&vChangedPos, &vChangedPos, m_pTransformCom->Get_RenderMatrix());
		
		if (m_eCurState == EMoloch::DASH_START || m_eCurState == EMoloch::FULL_DASH_START)
		{
			if (D3DXVec3Length(&m_vDashStartPos) < D3DXVec3Length(&vChangedPos))
				m_vDashStartPos = vChangedPos;
			
			m_vAnimDiffer = m_vOldPos - (m_vDashStartPos - vChangedPos) - vPos;

		}
		else
			m_vAnimDiffer = m_vOldPos + vChangedPos - vPos;
	}

	Update_Link();

}

void CMoloch::Decision_Position()
{
	_vec3 vTargetDisplacement = m_vAnimDiffer + m_vDisplacement;
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNavigationMesh(&vPos, &vTargetDisplacement, nullptr));


	On_NaviMesh();
	ZeroMemory(&m_vDisplacement, sizeof(_vec3));
	ZeroMemory(&m_vAnimDiffer, sizeof(_vec3));

}

HRESULT CMoloch::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_INT(EMoloch::EEND));
	m_vecAnimSetType.resize(SC_INT(EMoloch::EEND));
	m_vecAnimItpl.resize(SC_INT(EMoloch::EEND));

	Set_AnimVariable(SC_INT(EMoloch::IDLE),					45, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::RUN),					56, AT_REPEAT, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::ATTACK01),				19, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::ATTACK02),				20, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::ATTACK03),				21, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::DEATH01),				26, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FALL),					28, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::IMPACT),				25, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::INTRO),				46, AT_ONCE, 0.00000005);

	Set_AnimVariable(SC_INT(EMoloch::ATTACK_180_L),			0, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::ATTACK_180_R),			1, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::BLADE_EXTEND),			22, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::CRYSTAL_ERUPTION),		2, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::CRYSTAL_ERUPTION2),	3, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::TREMOR_PULSE),			18, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::DASH_START),			5, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::DASH_HOLD),			4, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::DASH_STRIKE),			6, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::FULL_DASH_START),		9, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_DASH),			8, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_DASH_STRIKE),		10, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_DASH_STRIKE2),	11, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::FULL_GEYSER_START),	14, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_GEYSER),			12, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_GEYSER2),			13, AT_ONCE, 0.05);
	
	Set_AnimVariable(SC_INT(EMoloch::FULL_SWING1),			15, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_SWING2),			16, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_SWING3),			17, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_ATK_180_R),		7, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EMoloch::FULL_IDLE),			24, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EMoloch::FULL_RUN),				37, AT_REPEAT, 0.05);

	return S_OK;
}

_int CMoloch::Update_Weapon(const _float & fTimeDelta)
{
	if (!m_pEquipWeapon)
		return 0;

	m_pEquipWeapon->Update_GameObject(fTimeDelta);
	return 0;
}

_int CMoloch::Update_Idle(const _float & fTimeDelta)
{
	m_fPatternDelay += fTimeDelta;
	if (m_fPatternDelay < 0.f)
	{
		return 0;
	}

	m_fPatternDelay = 0.f;
	if (!m_bAtk_Start)
	{
		++m_iPattern;

		if (m_iPattern > 3)
			m_iPattern = 0;

		m_bAtk_Start = true;
	}

	_vec3 vPos, vPlayerPos, vDir, vLook;
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	memcpy(&vPos,	matWorld.m[3], sizeof(_vec3));
	memcpy(&vLook,	matWorld.m[2], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));
	D3DXVec3Normalize(&vDir, &vDir);

	if (m_iHP  <= m_iHPMax / 2)
	{
		m_eCurState = EMoloch::BLADE_EXTEND;
		//m_bFullMode = true;
		m_iPattern = 0;
		m_bAtk_Start = false;
		return 0;
	}

	switch (m_iPattern)
	{
	case 0:	//	Attack
	{ 
		if (fDistance < 5.f)
		{
			_float fDotLook = D3DXVec3Dot(&vDir, &vLook);

			if (fDotLook < 0.f)
			{
				_vec3 vRight;
				memcpy(&vRight, matWorld.m[0], sizeof(_vec3));
				D3DXVec3Normalize(&vRight, &vRight);
				_float fDotRight = D3DXVec3Dot(&vDir, &vRight);
				m_eCurState = fDotRight < 0.f ? EMoloch::ATTACK_180_L : EMoloch::ATTACK_180_R;
				return 0;
			}
		}
		
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EMoloch::RUN;
	}
		break;

	case 1:	//	Dash
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EMoloch::DASH_START;
	}
		break;

	case 2:
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = rand() % 2 ? EMoloch::CRYSTAL_ERUPTION : EMoloch::CRYSTAL_ERUPTION2;
	}
	break;
	case 3:
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EMoloch::TREMOR_PULSE;
	}
	break;
	}

	return 0;
}

_int CMoloch::Update_Run(const _float & fTimeDelta)
{
	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (!m_bFootStepSound &&
		(((2971 - 2960) / 61. < dProgress && dProgress < (2982 - 2960) / 61.) ||
		((3002 - 2960) / 61. < dProgress && dProgress < (3012 - 2960) / 61.)))
	{
		m_bFootStepSound = true;

		Engine::Play_Sound(L"en_moloch_foot_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	if (m_bFootStepSound &&
		(((2987 - 2960) / 61. < dProgress && dProgress < (2998 - 2960) / 61.) ||
		((3016 - 2960) / 61. < dProgress)))
	{
		m_bFootStepSound = false;

		Engine::Play_Sound(L"en_moloch_foot_02.ogg", Engine::CSoundMgr::EFFECT);
	}

	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));

	D3DXVec3Normalize(&vDir, &vDir);

	m_vDisplacement += vDir * m_fSpeed * fTimeDelta;

	_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

	m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

	if (m_bAtk_Start)
	{
		if (m_iPattern == 0)
		{
			if (fDistance < 8.f)
				m_eCurState = EMoloch::ATTACK01;
		}
	}

	return 0;
}

_int CMoloch::Update_Fall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y < -20.f)
		m_eCurState = EMoloch::DEATH01;

	return 0;
}

_int CMoloch::Update_Death(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bDissolve = true;
		m_pEquipWeapon->Start_Dissolve();

		if (m_fDissolveTime > 2.f)
		{
			m_bIsDead = true;
			Engine::Die_GameObject(L"Layer_UI", L"UI_Boss");

			m_pNaviCom->Change_CellOption(156, Engine::CCell::NO_OPTION);
			m_pNaviCom->Change_CellOption(492, Engine::CCell::NO_OPTION);
			m_pNaviCom->Change_CellOption(501, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(502, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(499, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(500, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(494, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(495, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(496, Engine::CCell::FALL);
			m_pNaviCom->Change_CellOption(497, Engine::CCell::FALL);
		}

		return 0;
	}


	return 0;
}

_int CMoloch::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::IDLE;
		m_bEffectSound = false;
		return 0;
	}

	return 0;
}

_int CMoloch::Update_Intro(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::IDLE;
		m_bEffectSound = false;

		FAILED_CHECK_RETURN(Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_QUATER), -1);

		CBossUI* pUI = CBossUI::Create(m_pGraphicDev, L"UI_Boss");
		NULL_CHECK_RETURN(pUI, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_UI", L"UI_Boss", pUI), -1);

		return 0;
	}


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_intro.ogg", Engine::CSoundMgr::EFFECT);
	}

	if (!m_bCutScene)
	{
		m_bCutScene = true;
		FAILED_CHECK_RETURN(Engine::Change_MainCamera(Engine::CCameraMgr::MAIN_CAM_CUT_SCENE), -1);

		m_pNaviCom->Change_CellOption(156, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(492, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(501, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(502, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(499, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(500, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(494, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(495, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(496, Engine::CCell::NO_MOVE);
		m_pNaviCom->Change_CellOption(497, Engine::CCell::NO_MOVE);
	}

	_matrix matRender;
	m_pTransformCom->Get_RenderMatrix(&matRender);
	_vec3 vCamPos, vPelvis, vDir;

	const D3DXFRAME_DERIVED* pCamFrame		= m_pMeshCom->Get_FrameByName("Bone_Moloch_Camera");
	const D3DXFRAME_DERIVED* pPelvisFrame	= m_pMeshCom->Get_FrameByName("Bone_Moloch_Pelvis");

	memcpy(&vCamPos, pCamFrame->CombinedTransformationMatrix.m[3], sizeof(_vec3));
	memcpy(&vPelvis, pPelvisFrame->CombinedTransformationMatrix.m[3], sizeof(_vec3));

	D3DXVec3TransformCoord(&vCamPos, &vCamPos, &matRender);
	D3DXVec3TransformCoord(&vPelvis, &vPelvis, &matRender);

	vDir = -(vPelvis - vCamPos);
	_float fDist = D3DXVec3Length(&vDir);
	_vec3 vRotX, vRotY;
	D3DXVec3Normalize(&vRotX, &_vec3(0.f, vDir.y, vDir.z));
	D3DXVec3Normalize(&vRotY, &_vec3(vDir.x, 0.f, vDir.z));

	_float fRotX = Engine::CMyMath::AngleBetween2Vectors(&AXIS_Z, &vRotX, &AXIS_X);
	_float fRotY = Engine::CMyMath::AngleBetween2Vectors(&AXIS_Z, &vRotY, &AXIS_Y);

	CCutSceneCam* pCam = dynamic_cast<CCutSceneCam*>(Engine::Get_MainCamera());
	NULL_CHECK_RETURN(pCam, -1);

	pCam->Set_At(&vPelvis);
	pCam->Set_Angle(fRotX, fRotY, 0.f);
	pCam->Set_Distance(-fDist);

	return 0;
}

_int CMoloch::Update_Attack180L(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::IDLE;
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DX_PI);
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_180.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (15 / 71. < dProgress && dProgress < 30 / 71.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

_int CMoloch::Update_Attack180R(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::IDLE;
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DX_PI);
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}
	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_180.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((86 - 71) / 61. < dProgress && dProgress < (95 - 71) / 61.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

_int CMoloch::Update_BladeExtend(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		m_iPattern = 0;
		if (!m_bFullMode)
			m_pEquipWeapon->CrystalOn();

		return 0;
	}


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_blade_extend.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1558 - 1527) / 96. < dProgress && !m_bFullMode)
	{
		m_pEquipWeapon->CrystalOn();
		m_bFullMode = true;
	}

	return 0;
}

_int CMoloch::Update_CrystalEruption(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = m_bFullMode ? EMoloch::FULL_IDLE : EMoloch::IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		m_bEffect = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_crystal_eruption_1.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((152 - 132) / 101. < dProgress && dProgress < (162 - 132) / 101.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bEffect && (156 - 132) / 101. < dProgress)
	{
		m_bEffect = true;

		_vec3 vPos, vLook;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 3.f;

		CRockChips* pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 50, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
		NULL_CHECK_RETURN(pRockChips, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pRockChips), -1);
		pRockChips->Set_Pos(&vPos);
		pRockChips->Set_Speed(50.f);
		pRockChips->Set_Gravity(2.f);
		pRockChips->Set_Scale(0.5f);
		pRockChips->Set_LifeTime(1.f);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", 100, 100, 10.f, 5.f, 2.f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(2.75f, 2.75f, 2.75f), 1.f);

		for (_int i = 0; i < 3; ++i)
		{
			vPos = { 44.4007f, -0.105759f, -152.822f };

			_vec3 vDir = _vec3{ 81.9742f, -0.105765f, -188.656f } -vPos;

			vPos.x += vDir.x * (rand() % 100 / 100.f);
			vPos.y += vDir.y * (rand() % 100 / 100.f);
			vPos.z += vDir.z * (rand() % 100 / 100.f);

			Create_Crystal(&vPos);
		}
	} 


	if ((188 - 132) / 101. < dProgress && dProgress < (202 - 132) / 101.)
	{
		//	¶¥¿¡¼­ Ä® »Ì´Â ÇÁ·¹ÀÓ.
	}
	
	return 0;
}

_int CMoloch::Update_CrystalEruption2(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = m_bFullMode ? EMoloch::FULL_IDLE : EMoloch::IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		m_bEffect = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_crystal_eruption_2.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((250 - 233) / 111. < dProgress && dProgress < (271 - 233) / 111.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if ((298 - 233) / 111. < dProgress && dProgress < (315 - 233) / 111.)
	{
		//	¶¥¿¡¼­ Ä® »Ì´Â ÇÁ·¹ÀÓ.
	}

	if (!m_bEffect && (266 - 233) / 111.  < dProgress)
	{
		m_bEffect = true;
		_vec3 vPos, vLook;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 3.f;

		CRockChips* pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 50, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
		NULL_CHECK_RETURN(pRockChips, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pRockChips), -1);
		pRockChips->Set_Pos(&vPos);
		pRockChips->Set_Speed(50.f);
		pRockChips->Set_Gravity(2.f);
		pRockChips->Set_Scale(0.5f);
		pRockChips->Set_LifeTime(1.f);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", 100, 100, 10.f, 5.f, 2.f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(2.75f, 2.75f, 2.75f), 1.f);

		for (_int i = 0; i < 3; ++i)
		{
			vPos = { 44.4007f, -0.105759f, -152.822f };

			_vec3 vDir = _vec3{ 81.9742f, -0.105765f, -188.656f } -vPos;

			vPos.x += vDir.x * (rand() % 100 / 100.f);
			vPos.y += vDir.y * (rand() % 100 / 100.f);
			vPos.z += vDir.z * (rand() % 100 / 100.f);

			Create_Crystal(&vPos);
		}
	}

	return 0;
}

_int CMoloch::Update_TremorPulse(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = m_bFullMode ? EMoloch::FULL_IDLE : EMoloch::IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		m_bEffect = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_tremor_pulse.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (!m_bEffect && (1325 - 1283) / 91. < dProgress)
	{
		//	Æø¹ß Å¸ÀÌ¹Ö.
		m_bEffect = true;

		_vec3 vPos, vUp;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(Engine::INFO_UP, &vUp);
		D3DXVec3Normalize(&vUp, &vUp);
		vPos += vUp * 3.f;

		CCylinderEffect* pWave = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", 100, 20, 1.f, 0.5f, 0.f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pWave, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Pulse", pWave), -1);
		pWave->Set_Pos(&vPos);
		pWave->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(40.f, 40.f, 40.f), 1.f);
		pWave->Set_DeltaAlpha(-1.f);
		pWave->Set_LifeTime(1.f);


		Boom_Crystal();
	}


	return 0;
}

_int CMoloch::Update_DashStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::DASH_HOLD;
		m_vDashStartPos = { 0.f, 0.f, 0.f };
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}


	return 0;
}

_int CMoloch::Update_DashHold(const _float & fTimeDelta)
{
	m_fDashTime += fTimeDelta;
	if (m_fDashTime > 0.25f)
	{
		m_fDashTime = 0.f;
		m_eCurState = EMoloch::DASH_STRIKE;
		return 0;
	}

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	_vec3 vLook;
	memcpy(&vLook, matWorld.m[2], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);

	m_vDisplacement += vLook * m_fDashSpeed * fTimeDelta;


	return 0;
}

_int CMoloch::Update_DashStrike(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::IDLE;
		m_bEffectSound = false;
		return 0;
	}


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_dash_strike.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((472 - 466) / 56. < dProgress && dProgress < (490 - 466) / 56.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();


	return 0;
}

_int CMoloch::Update_FullIdle(const _float & fTimeDelta)
{
	m_fPatternDelay += fTimeDelta;
	if (m_fPatternDelay < 0.f)
	{
		return 0;
	}

	m_fPatternDelay = 0.f;
	if (!m_bAtk_Start)
	{
		++m_iPattern;

		if (m_iPattern > 4)
			m_iPattern = 0;

		m_bAtk_Start = true;
	}

	_vec3 vPos, vPlayerPos, vDir, vLook;
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	memcpy(&vPos, matWorld.m[3], sizeof(_vec3));
	memcpy(&vLook, matWorld.m[2], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));
	D3DXVec3Normalize(&vDir, &vDir);

	switch (m_iPattern)
	{
	case 0:	//	Attack
	{
		if (fDistance < 5.f)
		{
			_float fDotLook = D3DXVec3Dot(&vDir, &vLook);

			if (fDotLook < 0.f)
			{
				m_eCurState = EMoloch::FULL_ATK_180_R;
				return 0;
			}
		}

		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EMoloch::FULL_RUN;
	}
	break;

	case 1:	//	Dash
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EMoloch::FULL_DASH_START;
	}
	break;
	case 2:
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = rand() % 2 ? EMoloch::CRYSTAL_ERUPTION : EMoloch::CRYSTAL_ERUPTION2;
	}
	break;
	case 3:
	{	
		m_eCurState = EMoloch::TREMOR_PULSE;
	}
	break;
	case 4:
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EMoloch::FULL_GEYSER_START;
	}
	break;
	}

	return 0;
}

_int CMoloch::Update_FullRun(const _float & fTimeDelta)
{

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (!m_bFootStepSound && 
		(((2971 - 2960) / 61. < dProgress && dProgress < (2982 - 2960) / 61.) || 
		((3002 - 2960) / 61. < dProgress && dProgress < (3012 - 2960) / 61.)))
	{
		m_bFootStepSound = true;

		Engine::Play_Sound(L"en_moloch_foot_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	if (m_bFootStepSound && 
		(((2987 - 2960) / 61. < dProgress && dProgress < (2998 - 2960) / 61.) ||
		((3016 - 2960) / 61. < dProgress)) )
	{
		m_bFootStepSound = false;

		Engine::Play_Sound(L"en_moloch_foot_02.ogg", Engine::CSoundMgr::EFFECT);
	}


	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));

	D3DXVec3Normalize(&vDir, &vDir);

	m_vDisplacement += vDir * m_fSpeed * fTimeDelta;

	_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

	m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

	if (m_bAtk_Start)
	{
		switch (m_iPattern)
		{
		case 0:
			if (fDistance < 8.f)
			{
				switch (rand() % 4)
				{
				case 0: m_eCurState = EMoloch::FULL_SWING1; break;
				case 1: m_eCurState = EMoloch::FULL_SWING2; break;
				case 2: m_eCurState = EMoloch::FULL_SWING3; break;
				case 3: m_eCurState = EMoloch::ATTACK01;	break;
				}

			}
			break;
		case 1:
			
			
			break;
		}
	}
	return 0;
}

_int CMoloch::Update_FullDashStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_DASH;
		m_vDashStartPos = { 0.f, 0.f, 0.f };
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_dash_start.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CMoloch::Update_FullDash(const _float & fTimeDelta)
{
	m_fDashTime += fTimeDelta;
	if (m_fDashTime > 0.25f)
	{
		m_fDashTime = 0.f;
		m_eCurState = EMoloch::FULL_DASH_STRIKE;
		m_bEffectSound = false;
		return 0;
	}

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	_vec3 vLook;
	memcpy(&vLook, matWorld.m[2], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);

	m_vDisplacement += vLook * m_fDashSpeed * fTimeDelta;

	return 0;
}

_int CMoloch::Update_FullDashStrike(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_dash_strike_1.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((737 - 727) / 79. < dProgress && dProgress < (753 - 727) / 79.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bEffect && (749 - 727) / 79. < dProgress)
	{
		m_bEffect = true;

		_vec3 vPos, vLook;

		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 5.f;

		CRockChips* pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 50, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
		NULL_CHECK_RETURN(pRockChips, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pRockChips), -1);
		pRockChips->Set_Pos(&vPos);
		pRockChips->Set_Speed(50.f);
		pRockChips->Set_Gravity(2.f);
		pRockChips->Set_Scale(0.5f);
		pRockChips->Set_LifeTime(1.f);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", 100, 100, 10.f, 5.f, 2.f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Distortion", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(2.75f, 2.75f, 2.75f), 1.f);


		Create_Crystal(&vPos);
	}

	return 0;
}

_int CMoloch::Update_FullDashStrike2(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_dash_strike_2.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((815 - 806) / 66. < dProgress && dProgress < (843 - 806) / 66.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bEffect && (839 - 806) / 66. < dProgress)
	{
		m_bEffect = true;

		_vec3 vPos, vLook;

		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos += vLook * 5.f;

		CRockChips* pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 50, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
		NULL_CHECK_RETURN(pRockChips, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pRockChips), -1);
		pRockChips->Set_Pos(&vPos);
		pRockChips->Set_Speed(50.f);
		pRockChips->Set_Gravity(2.f);
		pRockChips->Set_Scale(0.5f);
		pRockChips->Set_LifeTime(1.f);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Noise0", 100, 100, 10.f, 5.f, 2.f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pTest, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Distortion", pTest), -1);
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(2.75f, 2.75f, 2.75f), 1.f);

		Create_Crystal(&vPos);
	}

	return 0;
}

_int CMoloch::Update_FullGeyserStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_GEYSER;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_geyser_start.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CMoloch::Update_FullGeyser(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bAtk_Start = false;
		CMonster::Change_State(SC_INT(EMoloch::FULL_IDLE));
		CMonster::Change_State(SC_INT(EMoloch::FULL_GEYSER));
		m_eCurState = EMoloch::FULL_GEYSER;
		++m_iGeyserCount;
		m_bEffect = false;
		m_bEffectSound = false;
		
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_geyser_1.ogg", Engine::CSoundMgr::EFFECT);
	}


	if (!m_bEffect && (882 - 872) / 56. < dProgress)
	{
		m_bEffect = true;

		_vec3 vPos = { 44.4007f, -0.105759f, -152.822f };

		_vec3 vDir = _vec3{ 81.9742f, -0.105765f, -188.656f } - vPos;

		for (_int i = 0; i < 4; ++i)
		{
			_vec3 vRandPos = vPos;
			vRandPos.x += vDir.x * (rand() % 100 / 100.f);
			vRandPos.y += vDir.y * (rand() % 100 / 100.f);
			vRandPos.z += vDir.z * (rand() % 100 / 100.f);

			Create_Crystal(&vRandPos);
		}
	}

	if (dProgress > 0.5)
	{
		if (m_iGeyserCount > 3)
		{
			m_eCurState = EMoloch::FULL_GEYSER2;
			m_iGeyserCount = 0;
			m_bAtk_Start = false;
			m_bEffect = false;
		}
		//	Æø¹ß ÀÌÆåÆ® ºÎºÐ

	}
	

	return 0;
}

_int CMoloch::Update_FullGeyser2(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bAtk_Start = false;
		m_bEffectSound = false;
		m_eCurState = EMoloch::FULL_IDLE;
		m_bEffect = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_geyser_jump.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((946 - 928) / 86. < dProgress && dProgress < (970 - 928) / 86.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bEffect && (965 - 928) / 86. < dProgress)
	{
		m_bEffect = true;
		Boom_Crystal();

		Engine::Play_Sound(L"en_moloch_atk_cluster_appear_01.ogg", Engine::CSoundMgr::EFFECT);
		Engine::Play_Sound(L"en_moloch_atk_cluster_appear_02.ogg", Engine::CSoundMgr::EFFECT);
		Engine::Play_Sound(L"en_moloch_atk_cluster_appear_03.ogg", Engine::CSoundMgr::EFFECT);

		//Engine::Play_Sound(L"en_moloch_atk_crystal_eruption_1.ogg", Engine::CSoundMgr::EFFECT);
	}


	return 0;
}

_int CMoloch::Update_FullSwing1(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_swing_01.ogg", Engine::CSoundMgr::EFFECT);
	}


	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1052 - 1040) / 76. < dProgress && dProgress < (1073 - 1040) / 76.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();


	return 0;
}

_int CMoloch::Update_FullSwing2(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_swing_02.ogg", Engine::CSoundMgr::EFFECT);
	}



	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1142 - 1116) / 71. < dProgress && dProgress < (1157 - 1116) / 71.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

_int CMoloch::Update_FullSwing3(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_full_swing_03.ogg", Engine::CSoundMgr::EFFECT);
	}


	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1214 - 1187) / 96. < dProgress && dProgress < (1235 - 1187) / 96.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

_int CMoloch::Update_FullAtk180R(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EMoloch::FULL_IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((535 - 522) / 83. < dProgress && dProgress < (557 - 522) / 83.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"en_moloch_atk_180.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

void CMoloch::Create_Crystal(const _vec3 * pPos)
{
	CMeshEffect* pMeshEffect = CMeshEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Dn_HL_TremorCrystal_A", L"Crystal");
	NULL_CHECK_RETURN(pMeshEffect, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_TremorCrystal", pMeshEffect), );
	pMeshEffect->SetUp_ScaleChange(&_vec3(0.001f, 0.001f, 0.001f), &_vec3(0.01f, 0.01f, 0.01f), 20.f);
	pMeshEffect->Set_Pos(pPos);
	pMeshEffect->Set_LifeTime(100.f);
	pMeshEffect->Set_DissolveStartTime(99.f);

	m_pCrytalList.push_back(pMeshEffect);
	pMeshEffect->AddRef();

	CHitGround* pHitGound = CHitGround::Create(m_pGraphicDev, pPos, 3.f, L"Monster_Attack");
	NULL_CHECK_RETURN(pHitGound, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_MonsterHit", pHitGound), );
	pHitGound->Set_LifeTime(1.f);
	pHitGound->Set_Attack(20);
}

void CMoloch::Boom_Crystal()
{
	for (auto& pCrystal : m_pCrytalList)
	{
		pCrystal->Start_Dissolve();

		_vec3 vPos;
		pCrystal->Get_Pos(&vPos);

		//CSphereEffect* pSphereFx = CSphereEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", D3DXCOLOR(1.f, 0.f, 1.f, 1.f), Engine::RENDER_DISTORTION, 5);
		CSphereEffect* pSphereFx = CSphereEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
		NULL_CHECK_RETURN(pSphereFx, );
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pSphereFx), );
		pSphereFx->Set_Pos(&vPos);
		pSphereFx->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(2.f, 2.f, 2.f), 1.f);
		pSphereFx->Set_LifeTime(1.f);
		pSphereFx->Set_Alpha(1.f);
		pSphereFx->Set_DeltaAlpha(-1.f);

		CRockChips* pRockChips = CRockChips::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_RockChips", 50, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
		NULL_CHECK_RETURN(pRockChips, );
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Sphere", pRockChips), );
		pRockChips->Set_Pos(&vPos);
		pRockChips->Set_Speed(50.f);
		pRockChips->Set_Gravity(2.f);
		pRockChips->Set_Scale(0.5f);
		pRockChips->Set_LifeTime(1.f);

		CCylinderEffect* pTest = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Cloud", 100, 100, 10.f, 5.f, 2.f);
		NULL_CHECK_RETURN(pTest, );
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Cloud", pTest), );
		pTest->Set_Pos(&vPos);
		pTest->Set_DeltaAlpha(-1.f);
		pTest->Set_LifeTime(1.f);
		pTest->SetUp_ScaleChange(&_vec3(0.1f, 0.1f, 0.1f), &_vec3(2.75f, 2.75f, 2.75f), 1.f);

		CHitGround* pHitGound = CHitGround::Create(m_pGraphicDev, &vPos, 7.f, L"Monster_Attack");
		NULL_CHECK_RETURN(pHitGound, );
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_MonsterHit", pHitGound), );
		pHitGound->Set_LifeTime(1.f);
		pHitGound->Set_Attack(20);
	}

	for (auto& pCrystal : m_pCrytalList)
		::Safe_Release(pCrystal);
	m_pCrytalList.clear();

	Engine::Get_MainCamera()->CameraShake();
}

_int CMoloch::Update_Attack01(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = m_bFullMode ?  EMoloch::FULL_IDLE : EMoloch::IDLE;
		m_bAtk_Start = false;
		m_bChainAtk = false; 
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_swipe_01.ogg", Engine::CSoundMgr::EFFECT);
	}


	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1387 - 1374) / 56. < dProgress && dProgress < (1403 - 1374) / 56.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bChainAtk && (1406 - 1374) / 56. < dProgress)
	{
		m_bChainAtk = true;
		if (rand() % 2)
		{
			m_eCurState = EMoloch::ATTACK02;
			m_bAtk_Start = false;
			m_bChainAtk = false;
		}
	}


	return 0;
}

_int CMoloch::Update_Attack02(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = m_bFullMode ? EMoloch::FULL_IDLE : EMoloch::IDLE;
		m_bChainAtk = false;

		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_swipe_02.ogg", Engine::CSoundMgr::EFFECT);
	}



	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1440 - 1430) / 51. < dProgress && dProgress < (1451 - 1430) / 51.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	if (!m_bChainAtk && (1459 - 1430) / 51. < dProgress)
	{
		m_bChainAtk = true;
		if (rand() % 2)
		{
			m_eCurState = EMoloch::ATTACK03;
			m_bAtk_Start = false;
			m_bChainAtk = false;
		}
	}


	return 0;
}

_int CMoloch::Update_Attack03(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = m_bFullMode ? EMoloch::FULL_IDLE : EMoloch::IDLE;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_moloch_atk_swipe_03.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if ((1496 - 1481) / 46. < dProgress && dProgress < (1512 - 1481) / 46.)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

CMoloch * CMoloch::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag, const _vec3* pSpawnPos)
{
	CMoloch* pInst = new CMoloch(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, pSpawnPos)))
		::Safe_Release(pInst);

	return pInst;
}

void CMoloch::Free()
{
	for (auto& pCrystal : m_pCrytalList)
		::Safe_Release(pCrystal);

	::Safe_Release(m_pObserver);
	::Safe_Release(m_pEquipWeapon);
	CMonster::Free();
}