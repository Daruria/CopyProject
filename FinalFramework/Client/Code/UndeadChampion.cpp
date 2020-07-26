#include "stdafx.h"
#include "UndeadChampion.h"
#include "SphereCollider.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "UndeadShield.h"
#include "UndeadSpear.h"

#include "Export_Function.h"
#include "Observer.h"
#include "UndeadPortal.h"
#include "Skeleton.h"
#include "ThrowSpear.h"
#include "RockChips.h"
#include "CylinderEffect.h"
#include "HitGround.h"

CUndeadChampion::CUndeadChampion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CUndeadChampion::~CUndeadChampion()
{
}

HRESULT CUndeadChampion::Ready_GameObject()
{
	return S_OK;
}

HRESULT CUndeadChampion::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pSpawnPoint)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);

	m_pTransformCom->Set_Pos(pSpawnPoint);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pSpawnPoint));

	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(0, 0.05);
	m_eCurState = EUndeadChampion::SPAWN_WALK;

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	m_pEquipWeapon = CUndeadSpear::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipWeapon, E_FAIL);

	BONE_LINKER*	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_S_Weapon_R")->CombinedTransformationMatrix;
	m_pEquipWeapon->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pEquipWeapon->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_pEquipShield = CUndeadShield::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipShield, E_FAIL);

	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_S_Weapon_L")->CombinedTransformationMatrix;
	m_pEquipShield->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pEquipShield->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_fSpeed = 5.f;
	m_iHP = 2000;
	m_iHPMax = 2000;

	m_pSpawnPortal = CUndeadPortal::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", L"Texture_FireNoise", L"Texture_PortalMask", 5.f, _vec4(0.f, 0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(m_pGraphicDev, E_FAIL);
	Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Portal", m_pSpawnPortal);

	m_vSpawnPoint = *pSpawnPoint;
	m_pSpawnPortal->Set_Pos(&_vec3(m_vSpawnPoint.x, m_vSpawnPoint.y, m_vSpawnPoint.z + 5.f));
	m_pSpawnPortal->Set_State(CUndeadPortal::EUndeadPortal::OPENING);

	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);


	return S_OK;
}

_int CUndeadChampion::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Update_State(fTimeDelta);
	Change_State();

	_int iExit = CMonster::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = Update_Weapon(fTimeDelta);
	if (iExit & 0x80000000)
		return -1;

	if (m_eCurState != EUndeadChampion::DEATH01 && m_eCurState != EUndeadChampion::SPAWN)
		Gravity(fTimeDelta);
	
	//On_NaviMesh();

	Engine::Add_GameObject_To_CollisionList(L"Monster_Body", this);

	return 0;
}

_int CUndeadChampion::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CMonster::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CUndeadChampion::Render_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == EUndeadChampion::SPAWN)
	{
		if (CUndeadPortal::EUndeadPortal::OPENING == m_pSpawnPortal->Get_State())
			return;

		if (m_pMeshCom->Is_InItpProgress())
		{
			PlayAnimation(fTimeDelta);
			Decision_Position();
			return;
		}
		else
		{
			m_pEquipShield->Set_RenderOn();
			m_pEquipWeapon->Set_RenderOn();
		}
	}
	else if (m_eCurState == EUndeadChampion::SPAWN_WALK)
	{
		if (m_fSpawnWalkTime < 0.5f)
		{
			m_pEquipShield->Set_RenderOff();
			m_pEquipWeapon->Set_RenderOff();
			PlayAnimation(fTimeDelta);
			Decision_Position();
			return;
		}
		else
		{
			m_pEquipShield->Set_RenderOn();
			m_pEquipWeapon->Set_RenderOn();
		}
	}

	CMonster::Render_GameObject(fTimeDelta);
}

HRESULT CUndeadChampion::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	_ulong dwFlag = TS_RENDEROFFSET;
	FAILED_CHECK_RETURN(CMonster::Add_Component(wContainerIdx, wstrBufferTag, dwFlag), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matColl;
	D3DXMatrixIdentity(&matColl);
	//matColl._42 = 0.5f;
	pCom = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_BONE, m_pTransformCom, &matColl, 2.f, L"Col_Monster");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	m_pColliderCom->Activate();
	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("null_Spine")->CombinedTransformationMatrix;
	m_pColliderCom->Set_Bone(&pLinker->matLink);
	m_LinkList.push_back(pLinker);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(0.f)));


	return S_OK;
}

void CUndeadChampion::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	CMonster::Change_State(SC_INT(m_eCurState));

	if (m_eCurState == EUndeadChampion::IDLE)
	{
		m_pEquipShield->Set_RenderOn();
		m_pEquipWeapon->Set_RenderOn();
	}

	m_bEffect = false;
	m_ePreState = m_eCurState;
}

_int CUndeadChampion::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CUndeadChampion::EUndeadChampion::IDLE:
		iExit = Update_Idle(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::RUN:
		iExit = Update_Run(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::FALL:
		iExit = Update_Fall(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::IMPACT:
		iExit = Update_Impact(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::DEATH01:
		iExit = Update_Death(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SHOCKWAVE:
		iExit = Update_ShockWave(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SPEARTOSS:
		iExit = Update_SpearToss(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::DASH_ATTACK:
		iExit = Update_DashAttack(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SUMMON_START:
		iExit = Update_SummonStart(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SUMMON:
		iExit = Update_Summon(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SUMMON_END:
		iExit = Update_SummonEnd(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::BLOCK_START:
		iExit = Update_BlockStart(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::BLOCK_IDLE:
		iExit = Update_BlockIdle(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::BLOCK_END:
		iExit = Update_BlockEnd(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::CHARGE_START:
		iExit = Update_ChargeStart(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::CHARGE_IDLE:
		iExit = Update_ChargeIdle(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::CHARGE_END:
		iExit = Update_ChargeEnd(fTimeDelta);
		break;
	case CUndeadChampion::EUndeadChampion::SPAWN_WALK:
		iExit = Update_SpawnWalk(fTimeDelta);
		break;
	}

	return iExit;
}

void CUndeadChampion::Hit(_int iAttack, _vec3 * pAtkPos)
{
	if (m_eCurState == EUndeadChampion::BLOCK_IDLE)
		return;

	m_iHP -= iAttack;
	m_pEquipWeapon->Stop();

	m_fRim = 1.f;

	Add_DamageFont(iAttack);
	Add_BloodParticle(pAtkPos);
	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_eCurState = EUndeadChampion::DEATH01;
		m_pColliderCom->Deactivate();
	}
}

void CUndeadChampion::On_NaviMesh()
{
	if (m_eCurState == EUndeadChampion::DEATH01)
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
		if (m_eCurState == EUndeadChampion::IDLE || m_eCurState == EUndeadChampion::RUN)
			m_eCurState = EUndeadChampion::FALL;
	}
		break;
	}
}

void CUndeadChampion::PlayAnimation(const _float & fTimeDelta)
{
	_vec3 vChangedPos = { 0.f, 0.f, 0.f }, vPos, vTargetPos;
	switch (m_eCurAnimType)
	{
	case AT_REPEAT:
		m_pMeshCom->Play_Animation(fTimeDelta, &vChangedPos);
		break;
	case AT_ONCE:
		if (m_eCurState == EUndeadChampion::SPAWN)
		{
			if (CUndeadPortal::EUndeadPortal::OPENING != m_pSpawnPortal->Get_State())
				m_pMeshCom->Play_Animation(fTimeDelta, &vChangedPos);
		}
		else if (!m_pMeshCom->Is_AnimationSetEnd())
			m_pMeshCom->Play_Animation(fTimeDelta, &vChangedPos);
		break;
	}

	if (m_pMeshCom->Is_InItpProgress())
		return;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	if (m_eCurState != EUndeadChampion::SPAWN_WALK && m_eCurState != EUndeadChampion::RUN && (vChangedPos.x || vChangedPos.y || vChangedPos.z))
	{
		D3DXVec3TransformNormal(&vChangedPos, &vChangedPos, m_pTransformCom->Get_RenderMatrix());
		m_vAnimDiffer = m_vOldPos + vChangedPos - vPos;
	}

	Update_Link();
}

HRESULT CUndeadChampion::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_INT(EUndeadChampion::EEND));
	m_vecAnimSetType.resize(SC_INT(EUndeadChampion::EEND));
	m_vecAnimItpl.resize(SC_INT(EUndeadChampion::EEND));

	Set_AnimVariable(SC_INT(EUndeadChampion::IDLE), 18, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::RUN), 45, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::DEATH01), 13, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::FALL), 27, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::IMPACT), 19, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::SPAWN), 44, AT_ONCE, 0.05);


	Set_AnimVariable(SC_INT(EUndeadChampion::SHOCKWAVE),	4, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::SPEARTOSS),	5, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::DASH_ATTACK),	3, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::SUMMON_START), 8, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::SUMMON),		6, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::SUMMON_END),	7, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::BLOCK_START),	11, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::BLOCK_IDLE),	10, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::BLOCK_END),	9, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::CHARGE_START), 2, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::CHARGE_IDLE),	1, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EUndeadChampion::CHARGE_END),	0, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EUndeadChampion::SPAWN_WALK), 55, AT_REPEAT, 0.05);




	return S_OK;
}

_int CUndeadChampion::Update_Weapon(const _float & fTimeDelta)
{
	if (!m_pEquipWeapon)
		return 0;

	_int iExit = m_pEquipWeapon->Update_GameObject(fTimeDelta);

	if (!m_pEquipShield)
		return 0;

	iExit = m_pEquipShield->Update_GameObject(fTimeDelta);

	return iExit;
}

_int CUndeadChampion::Update_Idle(const _float & fTimeDelta)
{
	m_fPatternDelay += fTimeDelta;

	if (m_fPatternDelay < 0.5f)
		return 0;
	else
		m_fPatternDelay = 0.f;

	m_pColliderCom->Activate();

	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));
	D3DXVec3Normalize(&vDir, &vDir);


	if (!m_bAtk_Start)
	{
		++m_iAtk_Phase;
		//m_iAtk_Phase = 1;
		if (m_iAtk_Phase > SC_INT(EUndeadPhase::CHARGE))
			m_iAtk_Phase = 0;

		m_bAtk_Start = true;


	}
	
	switch (m_iAtk_Phase)
	{
	case SC_INT(CUndeadChampion::EUndeadPhase::SHOCKWAVE):
		if (fDistance > 2.5f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
		{
			m_eCurState = CUndeadChampion::EUndeadChampion::SHOCKWAVE;

			_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
			m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		}
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::SPEARTOSS):
		if (fDistance > 10.f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
		{
			m_eCurState = CUndeadChampion::EUndeadChampion::SPEARTOSS;

			_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
			m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		}
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::DASH_ATTACK):
		if (fDistance > 5.f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
		{
			m_eCurState = CUndeadChampion::EUndeadChampion::DASH_ATTACK;

			_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
			m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		}
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::SUMMON):
		m_eCurState = CUndeadChampion::EUndeadChampion::SUMMON_START;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::BLOCK):
		m_eCurState = CUndeadChampion::EUndeadChampion::BLOCK_START;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::CHARGE):
		if (fDistance > 10.f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
		{
			m_eCurState = CUndeadChampion::EUndeadChampion::CHARGE_START;
			_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
			m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		}
		break;
	}

	return 0;
}

_int CUndeadChampion::Update_Run(const _float & fTimeDelta)
{


	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));

	D3DXVec3Normalize(&vDir, &vDir);

	//_ulong dwOption = 0;
	//_vec3 vTargetPos = m_pNaviCom->Move_OnNavigationMesh(&vPos, &(vDir * m_fSpeed * fTimeDelta), nullptr);
	//m_pTransformCom->Set_Pos(&vTargetPos);

	m_vDisplacement += vDir * m_fSpeed * fTimeDelta;

	_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

	m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

	if (!m_bAtk_Start)
	{
		++m_iAtk_Phase;

		if (m_iAtk_Phase > SC_INT(EUndeadPhase::CHARGE))
			m_iAtk_Phase = 0;

		m_bAtk_Start = true;
	}

	switch (m_iAtk_Phase)
	{
	case SC_INT(CUndeadChampion::EUndeadPhase::SHOCKWAVE):
		if (fDistance > 2.5f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
			m_eCurState = CUndeadChampion::EUndeadChampion::SHOCKWAVE;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::SPEARTOSS):
		if (fDistance > 10.f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
			m_eCurState = CUndeadChampion::EUndeadChampion::SPEARTOSS;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::DASH_ATTACK):
		if (fDistance > 5.f)
			m_eCurState = CUndeadChampion::EUndeadChampion::RUN;
		else
			m_eCurState = CUndeadChampion::EUndeadChampion::DASH_ATTACK;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::SUMMON):
		m_eCurState = CUndeadChampion::EUndeadChampion::SUMMON_START;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::BLOCK):
		m_eCurState = CUndeadChampion::EUndeadChampion::BLOCK_START;
		break;
	case SC_INT(CUndeadChampion::EUndeadPhase::CHARGE):
		m_eCurState = CUndeadChampion::EUndeadChampion::CHARGE_START;
		break;
	}

	return 0;
}

_int CUndeadChampion::Update_Fall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y < -20.f)
	{
		if (!m_pSpawnPortal)
		{
			m_pSpawnPortal = CUndeadPortal::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", L"Texture_FireNoise", L"Texture_PortalMask", 5.f, _vec4(0.f, 0.f, 0.f, 1.f));
			Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Portal", m_pSpawnPortal);

			m_pSpawnPortal->Set_State(CUndeadPortal::EUndeadPortal::OPENING);
			m_pSpawnPortal->Set_Pos(&_vec3(m_vSpawnPoint.x, m_vSpawnPoint.y - 2.f, m_vSpawnPoint.z));
			m_pSpawnPortal->Set_Angle(D3DXToRadian(90.f), 0.f, 0.f);
		}

		if (CUndeadPortal::EUndeadPortal::SPAWN == m_pSpawnPortal->Get_State())
		{
			m_eCurState = EUndeadChampion::SPAWN;
			PlayAnimation(0.1f);
			m_pTransformCom->Set_Pos(m_vSpawnPoint);
			m_fGravity = 0.f;
			m_pEquipShield->Set_RenderOff();
			m_pEquipWeapon->Set_RenderOff();
		}
	}

	

	return 0;
}

_int CUndeadChampion::Update_Death(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bEffect = false;
		m_bDissolve = true;
		m_pEquipWeapon->Start_Dissolve();
		m_pEquipShield->Start_Dissolve();
		if (m_fDissolveTime > 2.f)
			m_bIsDead = true;

		return 0;
	}

	return 0;
}

_int CUndeadChampion::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bEffect = false;
		m_eCurState = EUndeadChampion::IDLE;
		return 0;
	}

	return 0;
}

_int CUndeadChampion::Update_Spawn(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EUndeadChampion::IDLE;
		m_bEffect = false;
		if (m_pSpawnPortal)
		{
			m_pSpawnPortal->Set_State(CUndeadPortal::EUndeadPortal::CLOSING);
			m_pSpawnPortal = nullptr;
		}
		m_pColliderCom->Activate();
		return 0;
	}

	m_pColliderCom->Deactivate();

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (dProgress > 0.5)
	{
		
	}


	return 0;
}

_int CUndeadChampion::Update_ShockWave(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::IDLE;
		m_bAtk_Start = false;
		m_bShockWave = false;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_shockwave_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if ((161 - 137) / 76. < dProgress && dProgress < 39. / 76.)
	{
		m_pEquipShield->Attack();
	}
	else
	{
		m_pEquipShield->Stop();
	}

	if (!m_bShockWave && dProgress > (170 - 137)  / 76.)
	{
		m_bShockWave = true;

		//	Effect 추가 부분

		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

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
		pTest->Set_DeltaScale(&_vec3(0.1f, 0.1f, 0.1f));
		pTest->Set_MaxScale(&_vec3(0.5f, 0.5f, 0.5f));
		pTest->Set_Scale(0.4f);

		CHitGround* pHitGround = CHitGround::Create(m_pGraphicDev, &vPos, 3.f, L"Monster_Attack");
		NULL_CHECK_RETURN(pHitGround, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_HitGround", pHitGround), -1);
	}

	return 0;
}

_int CUndeadChampion::Update_SpearToss(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::IDLE;
		m_bAtk_Start = false;
		m_bEffect = false;
		m_bEffectSound = false;
		m_bSpearReturnSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_speartoss_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (40. / 141. < dProgress && dProgress < (297 - 213) / 141.)
	{
		m_pEquipWeapon->Attack();
	}
	else
	{
		m_pEquipWeapon->Stop();
	}

	if (!m_bEffect && (255 - 213)/ 141. < dProgress && dProgress < (294 - 213) / 141.)
	{
		//	던지는 이펙트 추가 부분
		m_bEffect = true;
		_vec3 vPos, vLook, vAngle;
		m_pEquipWeapon->Get_Pos(&vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		m_pTransformCom->Get_Angle(&vAngle);
		D3DXVec3Normalize(&vLook, &vLook);
		vLook.y -= 0.2f;
		CThrowSpear* pThrow = CThrowSpear::Create(m_pGraphicDev, &vPos, &vLook, &vAngle, 80.f, m_pNaviCom->Get_Height(&vPos));
		NULL_CHECK_RETURN(pThrow, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_ThrowSpear", pThrow), -1);
		pThrow->Set_LifeTime(0.25f);



	}
	if ((280 - 213) / 141. < dProgress && dProgress < (302 - 213) / 141.)
	{
		//	받는 이펙트 추가 부분.

		if (!m_bSpearReturnSound)
		{
			m_bSpearReturnSound = true;

			Engine::Play_Sound(L"en_undeadchamp_atk_speartoss_return.ogg", Engine::CSoundMgr::EFFECT);
		}
	}

	return 0;
}

_int CUndeadChampion::Update_DashAttack(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::IDLE;
		m_bAtk_Start = false;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_dash_01.ogg", Engine::CSoundMgr::EFFECT);
	}


	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if ((104 - 82) / 55. < dProgress && dProgress < (119 - 82) / 55.)
	{
		m_pEquipWeapon->Attack();
	}
	else
	{
		m_pEquipWeapon->Stop();
	}




	return 0;
}

_int CUndeadChampion::Update_SummonStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::SUMMON;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}
	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_summon_start_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	return 0;
}

_int CUndeadChampion::Update_Summon(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::SUMMON_END;
		m_bSummon = false;
		m_bEffect = false;
		return 0;
	}

	if (!m_bSummon)
	{
		wstring wstrBufferCom = L"Mesh_Skeleton";
		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		
		for (_int i = 0; i < 3; ++i)
		{
			_vec3 vSpawnPos = vPos + _vec3((rand() % 100 - 50.f) / 20.f, i * 0.01f, (rand() % 100 - 50.f) / 20.f);

			if (Engine::CCell::NO_OPTION != m_pNaviCom->Get_Option(&vPos))
			{
				_ulong dwIndex = m_pNaviCom->Get_NearIndex(Engine::CCell::NO_OPTION, &vPos);
				m_pNaviCom->Get_Center(dwIndex, &vSpawnPos);
			}

			CSkeleton* pSkel = CSkeleton::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, wstrBufferCom, L"Skeleton", &vSpawnPos);
			NULL_CHECK_RETURN(pSkel, -1);
			FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Skeleton", pSkel), E_FAIL);
		}
		m_bSummon = true;
	}



	return 0;
}

_int CUndeadChampion::Update_SummonEnd(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::IDLE;
		m_bAtk_Start = false;
		m_bEffect = false;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_summon_end.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	return 0;
}

_int CUndeadChampion::Update_BlockStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::BLOCK_IDLE;
		m_bEffect = false;
		return 0;
	}
	


	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_block_start.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CUndeadChampion::Update_BlockIdle(const _float & fTimeDelta)
{
	m_fBlockTime += fTimeDelta;

	if (m_fBlockTime >= 5.f)
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::BLOCK_END;
		m_bEffect = false;
		m_fBlockTime = 0.f;
		m_fBlockEffectTime = 0.f;
		return 0;
	}

	if (m_fBlockTime >= m_fBlockEffectTime)
	{
		m_fBlockEffectTime += 0.5f;

		_vec3 vAngle, vPos, vLook;

		m_pEquipShield->Get_Pos(&vPos);

		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		m_pTransformCom->Get_Angle(&vAngle);

		D3DXVec3Normalize(&vLook, &vLook);
		//vPos += 1.f * vLook;

		vAngle.x += D3DX_PI * 0.5f;

		CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 50, 20, 0.5f, 1.f, 0.2f, Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pEffect, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Throw", pEffect), -1);
		pEffect->Set_Pos(&vPos);
		pEffect->Set_Rotation(&vAngle);
		pEffect->SetUp_ScaleChange(&_vec3(1.f, 1.f, 1.f), &_vec3(4.f, 4.f, 4.f), 1.f);
		pEffect->Set_DeltaAlpha(-1.f);
		pEffect->Set_LifeTime(1.f);
	}
	

	//if (m_pMeshCom->Is_AnimationSetEnd())
	//{
	//	m_eCurState = CUndeadChampion::EUndeadChampion::BLOCK_END;
	//	m_bEffect = false;
	//	return 0;
	//}

	//_double dProgress = m_pMeshCom->Get_AnimationProgress();

	return 0;
}

_int CUndeadChampion::Update_BlockEnd(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::IDLE;
		m_bEffect = false;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_block_end.ogg", Engine::CSoundMgr::EFFECT);
	}



	return 0;
}

_int CUndeadChampion::Update_ChargeStart(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::CHARGE_IDLE;
		m_bEffect = false;
		m_bEffectSound = false;
		m_pEquipShield->Attack();
		return 0;
	}



	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_charge_start_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	return 0;
}

_int CUndeadChampion::Update_ChargeIdle(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::CHARGE_END;
		m_bEffect = false;
		m_bEffectSound = false;
		Engine::Stop_Sound(Engine::CSoundMgr::EFFECT_LOOP);
		
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_charge_loop.ogg", Engine::CSoundMgr::EFFECT_LOOP);
	}

	m_fChargeFxTime += fTimeDelta;

	if (m_fChargeFxTime >= 0.1f)
	{
		m_fChargeFxTime -= 0.1f;


		_vec3 vAngle, vPos, vLook;

		m_pEquipShield->Get_Pos(&vPos);

		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		m_pTransformCom->Get_Angle(&vAngle);

		D3DXVec3Normalize(&vLook, &vLook);
		vPos += 1.f * vLook;

		vAngle.x += D3DX_PI * 0.5f;

		CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 50, 20, 0.5f, 1.f, 1.f);
		NULL_CHECK_RETURN(pEffect, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Throw", pEffect), -1);
		pEffect->Set_Pos(&vPos);
		pEffect->Set_Rotation(&vAngle);
		pEffect->SetUp_ScaleChange(&_vec3(2.f, 2.f, 2.f), &_vec3(4.f, 4.f, 4.f), 2.f);
		pEffect->Set_DeltaAlpha(-2.f);
		pEffect->Set_LifeTime(0.5f);
	}

	/*_double dProgress = m_pMeshCom->Get_AnimationProgress();*/

	

	return 0;
}

_int CUndeadChampion::Update_ChargeEnd(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = CUndeadChampion::EUndeadChampion::IDLE;
		m_bEffect = false;
		m_bAtk_Start = false;
		m_bEffectSound = false;
		m_pEquipShield->Stop();
		return 0;
	}



	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_undeadchamp_atk_charge_end_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (!m_bEffect && 2 / 34. < dProgress)
	{
		m_bEffect = true;

		_vec3 vAngle, vPos, vLook;

		m_pEquipShield->Get_Pos(&vPos);
		
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);
		m_pTransformCom->Get_Angle(&vAngle);

		D3DXVec3Normalize(&vLook, &vLook);
		vPos += 1.f * vLook;

		vAngle.x += D3DX_PI * 0.5f;

		CCylinderEffect* pEffect = CCylinderEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Wave", 50, 20, 0.5f, 1.f, 1.f);
		NULL_CHECK_RETURN(pEffect, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_ChargeFx", pEffect), -1);
		pEffect->Set_Pos(&vPos);
		pEffect->Set_Rotation(&vAngle);
		pEffect->SetUp_ScaleChange(&_vec3(2.f, 2.f, 2.f), &_vec3(4.f, 4.f, 4.f), 2.f);
		pEffect->Set_DeltaAlpha(-2.f);
		pEffect->Set_LifeTime(0.5f);
	}

	return 0;
}

_int CUndeadChampion::Update_SpawnWalk(const _float & fTimeDelta)
{
	m_fSpawnWalkTime += fTimeDelta;

	if (m_fSpawnWalkTime > 4.f)
	{
		m_eCurState = EUndeadChampion::IDLE;
		m_bEffect = false;
		if (m_pSpawnPortal)
		{
			m_pSpawnPortal->Set_State(CUndeadPortal::EUndeadPortal::CLOSING);
			m_pSpawnPortal = nullptr;
		}
		m_pColliderCom->Activate();
	}

	_vec3 vLook;	
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	memcpy(&vLook, matWorld.m[2], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);

	m_vDisplacement += vLook * fTimeDelta * m_fSpeed * 0.5f;

	m_pColliderCom->Deactivate();

	return 0;
}

CUndeadChampion * CUndeadChampion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag,
	const _vec3* pSpawnPoint)
{
	CUndeadChampion* pInst = new CUndeadChampion(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, pSpawnPoint)))
		::Safe_Release(pInst);

	return pInst;
}

void CUndeadChampion::Free()
{
	::Safe_Release(m_pObserver);
	::Safe_Release(m_pEquipShield);
	::Safe_Release(m_pEquipWeapon);
	CMonster::Free();
}

