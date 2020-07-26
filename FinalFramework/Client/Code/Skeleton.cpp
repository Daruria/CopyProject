#include "stdafx.h"
#include "Skeleton.h"
#include "SphereCollider.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "UndeadShield.h"
#include "UndeadSpear.h"

#include "Export_Function.h"
#include "Observer.h"
#include "UndeadPortal.h"
#include "SkeletonSword.h"

CSkeleton::CSkeleton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CSkeleton::~CSkeleton()
{
}

HRESULT CSkeleton::Ready_GameObject()
{
	return S_OK;
}

HRESULT CSkeleton::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName, const _vec3* pSpawnPoint)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	//FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);

	m_pTransformCom->Set_Pos(pSpawnPoint);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pSpawnPoint));

	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(0, 0.05);
	m_eCurState = ESkeleton::SPAWN;

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	m_pEquipWeapon = CSkeletonSword::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEquipWeapon, E_FAIL);

	BONE_LINKER*	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_S_Weapon_R")->CombinedTransformationMatrix;
	m_pEquipWeapon->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pEquipWeapon->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_fSpeed = 5.f;
	m_iHP = 100;
	m_iHPMax = 100;

	m_pSpawnPortal = CUndeadPortal::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_FireTile", L"Texture_FireNoise", L"Texture_PortalMask", 2.f, _vec4(0.f, 0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(m_pGraphicDev, E_FAIL);
	Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Portal", m_pSpawnPortal);

	m_vSpawnPoint = *pSpawnPoint;
	m_pSpawnPortal->Set_Pos(&_vec3(m_vSpawnPoint.x, m_vSpawnPoint.y + 1.f, m_vSpawnPoint.z));
	m_pSpawnPortal->Set_State(CUndeadPortal::EUndeadPortal::OPENING);
	m_pSpawnPortal->Set_Angle(D3DX_PI * 0.5f, 0.f, 0.f);

	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);


	return S_OK;
}

_int CSkeleton::Update_GameObject(const _float& fTimeDelta)
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

	if (m_eCurState != ESkeleton::DEATH && m_eCurState != ESkeleton::SPAWN)
		Gravity(fTimeDelta);
	
	//On_NaviMesh();

	Engine::Add_GameObject_To_CollisionList(L"Monster_Body", this);

	return 0;
}

_int CSkeleton::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CMonster::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CSkeleton::Render_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == ESkeleton::SPAWN)
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
			m_pEquipWeapon->Set_RenderOn();
		}
	}
	

	CMonster::Render_GameObject(fTimeDelta);
}

HRESULT CSkeleton::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
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
	m_pColliderCom->Set_Bone(&pLinker->matLink);
	m_LinkList.push_back(pLinker);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(0.f)));


	return S_OK;
}

void CSkeleton::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	CMonster::Change_State(SC_INT(m_eCurState));

	if (m_eCurState == ESkeleton::IDLE)
	{
		m_pEquipWeapon->Set_RenderOn();
	}

	m_ePreState = m_eCurState;
}

_int CSkeleton::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CSkeleton::ESkeleton::IDLE:
		iExit = Update_Idle(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::RUN:
		iExit = Update_Run(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::FALL:
		iExit = Update_Fall(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::IMPACT_L:
	case CSkeleton::ESkeleton::IMPACT_R:
	case CSkeleton::ESkeleton::IMPACT_B:
	case CSkeleton::ESkeleton::IMPACT_F:
		iExit = Update_Impact(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::DEATH:
		iExit = Update_Death(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::ATK_MELEE1:
		iExit = Update_AtkMelee1(fTimeDelta);
		break;
	case CSkeleton::ESkeleton::ATK_MELEE2:
		iExit = Update_AtkMelee2(fTimeDelta);
		break;


	}

	return iExit;
}

void CSkeleton::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;
	m_pEquipWeapon->Stop();

	m_fRim = 1.f;
	Add_DamageFont(iAttack);
	Add_BloodParticle(pAtkPos);

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_eCurState = ESkeleton::DEATH;
		m_pColliderCom->Deactivate();
		return;
	}

	_vec3 vPos, vDir, vLook, vRight;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));
	D3DXVec3Normalize(&vRight, m_pTransformCom->Get_Info(Engine::INFO_RIGHT));
	D3DXVec3Normalize(&vDir, &(vPos - *pAtkPos));

	_float fLookDot = D3DXVec3Dot(&vLook, &vDir);
	_float fRightDot = D3DXVec3Dot(&vRight, &vDir);

	m_eCurState = fabsf(fLookDot) > fabsf(fRightDot) ?	(fLookDot > 0.f ? ESkeleton::IMPACT_B : ESkeleton::IMPACT_F) :
														(fRightDot > 0.f ? ESkeleton::IMPACT_L : ESkeleton::IMPACT_R);


	m_pEquipWeapon->Stop();
}

void CSkeleton::On_NaviMesh()
{
	if (m_eCurState == ESkeleton::DEATH)
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
		if (m_eCurState == ESkeleton::IDLE || m_eCurState == ESkeleton::RUN)
			m_eCurState = ESkeleton::FALL;
	}
		break;
	}
}

void CSkeleton::PlayAnimation(const _float & fTimeDelta)
{
	_vec3 vChangedPos = { 0.f, 0.f, 0.f }, vPos, vTargetPos;
	switch (m_eCurAnimType)
	{
	case AT_REPEAT:
		m_pMeshCom->Play_Animation(fTimeDelta, &vChangedPos);
		break;
	case AT_ONCE:
		if (m_eCurState == ESkeleton::SPAWN)
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
	if (m_eCurState != ESkeleton::SPAWN_WALK && m_eCurState != ESkeleton::RUN && (vChangedPos.x || vChangedPos.y || vChangedPos.z))
	{
		D3DXVec3TransformNormal(&vChangedPos, &vChangedPos, m_pTransformCom->Get_RenderMatrix());
		m_vAnimDiffer = m_vOldPos + vChangedPos - vPos;
	}

	Update_Link();
}

HRESULT CSkeleton::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_INT(ESkeleton::EEND));
	m_vecAnimSetType.resize(SC_INT(ESkeleton::EEND));
	m_vecAnimItpl.resize(SC_INT(ESkeleton::EEND));

	Set_AnimVariable(SC_INT(ESkeleton::IDLE), 70, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::RUN), 97, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::DEATH), 67, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::FALL), 80, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::IMPACT_L), 77, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::IMPACT_R), 78, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::IMPACT_B), 71, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::IMPACT_F), 72, AT_ONCE, 0.05);
	
	Set_AnimVariable(SC_INT(ESkeleton::ATK_MELEE1), 2, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::ATK_MELEE2), 3, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(ESkeleton::SPAWN), 100, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(ESkeleton::SPAWN_WALK), 111, AT_REPEAT, 0.05);




	return S_OK;
}

_int CSkeleton::Update_Weapon(const _float & fTimeDelta)
{
	if (!m_pEquipWeapon)
		return 0;

	_int iExit = m_pEquipWeapon->Update_GameObject(fTimeDelta);

	return iExit;
}

_int CSkeleton::Update_Idle(const _float & fTimeDelta)
{
	m_fPatternDelay += fTimeDelta;
	
	if (m_fPatternDelay < 0.5f)
		return 0;
	else
		m_fPatternDelay = 0.f;

	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));
	D3DXVec3Normalize(&vDir, &vDir);


	if (fDistance < 2.5f)
	{
		m_eCurState = rand() % 2 ? ESkeleton::ATK_MELEE1 : ESkeleton::ATK_MELEE2;

		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		if (isnan(fAngle))
			fAngle = 0.f;
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
	}
	else if (fDistance < 30.f)
	{
		m_eCurState = ESkeleton::RUN;

		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
	}


	return 0;
}

_int CSkeleton::Update_Run(const _float & fTimeDelta)
{
	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));

	D3DXVec3Normalize(&vDir, &vDir);

	m_vDisplacement += vDir * m_fSpeed * fTimeDelta;

	_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);
	m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

	if (fDistance < 2.5f)
	{
		m_eCurState = rand() % 2 ? ESkeleton::ATK_MELEE1 : ESkeleton::ATK_MELEE2;
	}


	return 0;
}

_int CSkeleton::Update_Fall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y < -20.f)
	{
		m_bIsDead = true;
	}

	return 0;
}

_int CSkeleton::Update_Death(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bDissolve = true;
		m_pEquipWeapon->Start_Dissolve();
		if (m_fDissolveTime > 2.f)
			m_bIsDead = true;

		return 0;
	}

	return 0;
}

_int CSkeleton::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = ESkeleton::IDLE;
		return 0;
	}

	return 0;
}

_int CSkeleton::Update_Spawn(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = ESkeleton::IDLE;
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

_int CSkeleton::Update_AtkMelee1(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = ESkeleton::IDLE;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (dProgress > 0.2 && dProgress < 0.5)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

_int CSkeleton::Update_AtkMelee2(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = ESkeleton::IDLE;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (dProgress > 0.2 && dProgress < 0.5)
		m_pEquipWeapon->Attack();
	else
		m_pEquipWeapon->Stop();

	return 0;
}

CSkeleton * CSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag,
	const _vec3* pSpawnPoint)
{
	CSkeleton* pInst = new CSkeleton(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, pSpawnPoint)))
		::Safe_Release(pInst);

	return pInst;
}

void CSkeleton::Free()
{
	::Safe_Release(m_pObserver);
	::Safe_Release(m_pEquipWeapon);
	CMonster::Free();
}

