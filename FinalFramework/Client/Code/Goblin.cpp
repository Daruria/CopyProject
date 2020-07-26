#include "stdafx.h"
#include "Goblin.h"
#include "SphereCollider.h"
#include "DynamicMesh.h"
#include "Transform.h"

#include "Export_Function.h"
#include "Observer.h"
#include "GoblinSword.h"
#include "DamageFont.h"
#include "Particle.h"

CGoblin::CGoblin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CGoblin::~CGoblin()
{
}

HRESULT CGoblin::Ready_GameObject()
{
	return S_OK;
}

HRESULT CGoblin::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(0, 0.05);
	m_eCurState = EGoblin::SPAWN;


	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	m_pEquipWeapon = CGoblinSword::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipWeapon, E_FAIL);
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Goblin_Weapon_Fleamag_Sword")->CombinedTransformationMatrix;
	m_pEquipWeapon->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pEquipWeapon->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_fSpeed = 8.f;
	m_iHP = 100;
	m_iHPMax = 100;


	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);
	return S_OK;
}

HRESULT CGoblin::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrName, EGoblin eStartState)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(0, 0.05);
	m_eCurState = eStartState;


	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	m_pEquipWeapon = CGoblinSword::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipWeapon, E_FAIL);
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Goblin_Weapon_Fleamag_Sword")->CombinedTransformationMatrix;
	m_pEquipWeapon->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pEquipWeapon->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_fSpeed = 5.f;
	m_iHP = 100;
	m_iHPMax = 100;


	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);
	return S_OK;
}

_int CGoblin::Update_GameObject(const _float& fTimeDelta)
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

	if (m_eCurState != EGoblin::DEATH01 && m_eCurState != EGoblin::SPAWN &&
		m_eCurState != EGoblin::SIT_IDLE && m_eCurState != EGoblin::SIT_END)
		Gravity(fTimeDelta);
	//On_NaviMesh();

	Engine::Add_GameObject_To_CollisionList(L"Monster_Body", this);

	return 0;
}

_int CGoblin::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CMonster::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CGoblin::Render_GameObject(const _float& fTimeDelta)
{
	CMonster::Render_GameObject(fTimeDelta);
}

HRESULT CGoblin::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	_ulong dwFlag = TS_RENDEROFFSET;
	FAILED_CHECK_RETURN(CMonster::Add_Component(wContainerIdx, wstrBufferTag, dwFlag), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matColl;
	D3DXMatrixIdentity(&matColl);
	//matColl._42 = 0.5f;
	pCom = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_BONE, m_pTransformCom, &matColl, 1.f, L"Col_Monster");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	m_pColliderCom->Activate();

	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("null_Spine")->CombinedTransformationMatrix;
	D3DXMatrixIdentity(&pLinker->matLink);
	m_pColliderCom->Set_Bone(&pLinker->matLink);
	m_LinkList.push_back(pLinker);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(0.f)));


	return S_OK;
}

void CGoblin::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	CMonster::Change_State(SC_INT(m_eCurState));

	m_ePreState = m_eCurState;
}

_int CGoblin::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CGoblin::EGoblin::IDLE:
		iExit = Update_Idle(fTimeDelta);
		break;
	case CGoblin::EGoblin::RUN:
		iExit = Update_Run(fTimeDelta);
		break;
	case CGoblin::EGoblin::FALL:
		iExit = Update_Fall(fTimeDelta);
		break;
	case CGoblin::EGoblin::IMPACT:
		iExit = Update_Impact(fTimeDelta);
		break;
	case CGoblin::EGoblin::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CGoblin::EGoblin::DEATH01:
		iExit = Update_Death(fTimeDelta);
		break;
	case CGoblin::EGoblin::ATTACK01:
		iExit = Update_Attack01(fTimeDelta);
		break;
	case CGoblin::EGoblin::ATTACK02:
		iExit = Update_Attack02(fTimeDelta);
		break;
	case CGoblin::EGoblin::ATTACK03:
		iExit = Update_Attack03(fTimeDelta);
		break;
	case CGoblin::EGoblin::SIT_IDLE:
		iExit = Update_SitIdle(fTimeDelta);
		break;
	case CGoblin::EGoblin::SIT_END:
		iExit = Update_SitEnd(fTimeDelta);
		break;
	}

	return iExit;
}

void CGoblin::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;
	m_pEquipWeapon->Stop();

	Add_DamageFont(iAttack);
	Add_BloodParticle(pAtkPos);
	
	m_fRim = 1.f;

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_eCurState = EGoblin::DEATH01;
		m_pColliderCom->Deactivate();
	}
	else
	{
		if (m_eCurState == EGoblin::IMPACT)
		{
			CMonster::Change_State(SC_INT(EGoblin::IDLE));
			CMonster::Change_State(SC_INT(EGoblin::IMPACT));
		}
		m_eCurState = EGoblin::IMPACT;
	}
}


void CGoblin::On_NaviMesh()
{
	if (m_eCurState == EGoblin::DEATH01)
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
		if (m_eCurState == EGoblin::IDLE)
			m_eCurState = EGoblin::FALL;
	}
		break;
	}
}

HRESULT CGoblin::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_INT(EGoblin::EEND));
	m_vecAnimSetType.resize(SC_INT(EGoblin::EEND));
	m_vecAnimItpl.resize(SC_INT(EGoblin::EEND));

	Set_AnimVariable(SC_INT(EGoblin::IDLE), 70, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::RUN), 57, AT_REPEAT, 0.05);

	Set_AnimVariable(SC_INT(EGoblin::ATTACK01), 64, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::ATTACK02), 65, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::ATTACK03), 22, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EGoblin::DEATH01), 27, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::FALL), 32, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::IMPACT), 42, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::SPAWN), 108, AT_ONCE, 0.000005);

	Set_AnimVariable(SC_INT(EGoblin::SIT_IDLE), 63, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EGoblin::SIT_END), 60, AT_ONCE, 0.05);

	return S_OK;
}

_int CGoblin::Update_Weapon(const _float & fTimeDelta)
{
	if (!m_pEquipWeapon)
		return 0;

	m_pEquipWeapon->Update_GameObject(fTimeDelta);
	return 0;
}

_int CGoblin::Update_Idle(const _float & fTimeDelta)
{
	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));
	D3DXVec3Normalize(&vDir, &vDir);

	if (fDistance < 2.5f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EGoblin::ATTACK02;
		return 0;
	} 
	else if (fDistance < 5.f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EGoblin::ATTACK01;
		return 0;
	}
	else if (fDistance < 30.f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EGoblin::RUN;
		return 0;
	}

	return 0;
}

_int CGoblin::Update_Run(const _float & fTimeDelta)
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

	if (fDistance < 2.5f)
	{
		m_eCurState = EGoblin::ATTACK02;
		return 0;
	}
	else if (fDistance < 5.f)
	{
		m_eCurState = EGoblin::ATTACK01;
		return 0;
	}
	else if (fDistance > 30.f)
	{
		m_eCurState = EGoblin::IDLE;
		return 0;
	}
	return 0;
}

_int CGoblin::Update_Fall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y < -20.f)
		m_eCurState = EGoblin::DEATH01;

	return 0;
}

_int CGoblin::Update_Death(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		//m_bIsDead = true;

		//m_bDissolve = true;

		m_bDissolve = true;
		m_pEquipWeapon->Start_Dissolve();

		if (m_fDissolveTime > 2.f)
			m_bIsDead = true;
		
		return 0;
	}

	

	return 0;
}

_int CGoblin::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EGoblin::IDLE;
		return 0;
	}

	return 0;
}

_int CGoblin::Update_Spawn(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EGoblin::IDLE;
		return 0;
	}

	return 0;
}

_int CGoblin::Update_SitIdle(const _float & fTimeDelta)
{
	return 0;
}

void CGoblin::Decision_Position()
{
	_vec3 vTargetDisplacement = m_vAnimDiffer + m_vDisplacement;
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	
	if (m_eCurState == EGoblin::SIT_END)
		m_pTransformCom->Set_Pos(vPos + vTargetDisplacement);
	else
		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNavigationMesh(&vPos, &vTargetDisplacement, nullptr));


	On_NaviMesh();
	ZeroMemory(&m_vDisplacement, sizeof(_vec3));
	ZeroMemory(&m_vAnimDiffer, sizeof(_vec3));
}

void CGoblin::Sit_End()
{
	m_eCurState = EGoblin::SIT_END;
	_vec3 vPlayerPos, vPos, vLook, vDir;
	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	memcpy(&vLook, matWorld.m[2], sizeof(_vec3));
	memcpy(&vPos, matWorld.m[3], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);
	vDir = vPlayerPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	_float fAngle = CMyMath::AngleBetween2Vectors(&vDir, &vLook, &AXIS_Y);
	m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

	m_vTarget = vPlayerPos;
}

_int CGoblin::Update_SitEnd(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_float fY = m_pNaviCom->Get_Height(&vPos);

	_vec3 vDir;
	vDir = m_vTarget - vPos;
	if (vPos.y < fY || D3DXVec3Length(&vDir) < 0.5f)
	{
		m_pTransformCom->Set_PosY(fY);
		
		m_eCurState = EGoblin::IDLE;
		
		_ulong dwIndex = m_pNaviCom->Get_Index(&vPos);
		m_pNaviCom->Set_Index(dwIndex);

		return 0;
	}


	m_vDisplacement += vDir * fTimeDelta * 2.f;

	return 0;
}

_int CGoblin::Update_Attack01(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EGoblin::IDLE;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.5 && dProgress < 0.7)
		m_pEquipWeapon->Attack();
	else if (dProgress > 0.7)
		m_pEquipWeapon->Stop();

	return 0;
}

_int CGoblin::Update_Attack02(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EGoblin::IDLE;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.3 && dProgress < 0.5)
		m_pEquipWeapon->Attack();
	else if (dProgress > 0.5)
		m_pEquipWeapon->Stop();

	return 0;
}

_int CGoblin::Update_Attack03(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EGoblin::IDLE;
		return 0;
	}
	return 0;
}

CGoblin * CGoblin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CGoblin* pInst = new CGoblin(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);

	return pInst;
}

CGoblin * CGoblin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag, EGoblin eStartState)
{
	CGoblin* pInst = new CGoblin(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, eStartState)))
		::Safe_Release(pInst);

	return pInst;
}

void CGoblin::Free()
{
	::Safe_Release(m_pObserver);
	::Safe_Release(m_pEquipWeapon);
	CMonster::Free();
}

