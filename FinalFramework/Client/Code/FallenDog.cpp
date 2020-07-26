#include "stdafx.h"
#include "FallenDog.h"
#include "SphereCollider.h"
#include "DynamicMesh.h"
#include "Transform.h"

#include "Export_Function.h"
#include "Observer.h"
#include "InvisibleWeapon.h"

CFallenDog::CFallenDog(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CFallenDog::~CFallenDog()
{
}

HRESULT CFallenDog::Ready_GameObject()
{
	return S_OK;
}

HRESULT CFallenDog::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);


	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(40, 0.05);
	m_eCurState = EFallenDog::SPAWN;

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	const D3DXFRAME_DERIVED* pFrame = nullptr;

	m_pLeftHand = CInvisibleWeapon::Create(m_pGraphicDev, 1.f);
	NULL_CHECK_RETURN(m_pLeftHand, E_FAIL);
	BONE_LINKER*	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Hand_L_FD")->CombinedTransformationMatrix;
	m_pLeftHand->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pLeftHand->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_pRightHand = CInvisibleWeapon::Create(m_pGraphicDev, 1.f);
	NULL_CHECK_RETURN(m_pRightHand, E_FAIL);
	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Hand_R_FD")->CombinedTransformationMatrix;
	m_pRightHand->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pRightHand->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_pHead = CInvisibleWeapon::Create(m_pGraphicDev, 1.5f);
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Head_FD")->CombinedTransformationMatrix;
	m_pHead->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pHead->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);


	m_fSpeed = 8.f;
	m_iHP = 200;
	m_iHPMax = 200;

	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);

	return S_OK;
}

HRESULT CFallenDog::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrName, EFallenDog eStartState)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);


	m_pTransformCom->Set_Scale(0.01f);
	m_pMeshCom->Set_AnimationSet(40, 0.05);
	m_eCurState = eStartState;

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	const D3DXFRAME_DERIVED* pFrame = nullptr;

	m_pLeftHand = CInvisibleWeapon::Create(m_pGraphicDev, 1.f);
	NULL_CHECK_RETURN(m_pLeftHand, E_FAIL);
	BONE_LINKER*	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Hand_L_FD")->CombinedTransformationMatrix;
	m_pLeftHand->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pLeftHand->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_pRightHand = CInvisibleWeapon::Create(m_pGraphicDev, 1.f);
	NULL_CHECK_RETURN(m_pRightHand, E_FAIL);
	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Hand_R_FD")->CombinedTransformationMatrix;
	m_pRightHand->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pRightHand->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_pHead = CInvisibleWeapon::Create(m_pGraphicDev, 1.5f);
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Head_FD")->CombinedTransformationMatrix;
	m_pHead->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pHead->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);


	m_fSpeed = 5.f;
	m_iHP = 200;
	m_iHPMax = 200;

	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);

	return S_OK;
}

_int CFallenDog::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Update_State(fTimeDelta);
	Change_State();

	_int iExit = CMonster::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = Update_Hands(fTimeDelta);
	if (iExit & 0x80000000)
		return -1;

	if (m_eCurState != EFallenDog::DEATH01 && m_eCurState != EFallenDog::SPAWN)
		Gravity(fTimeDelta);
	//On_NaviMesh();

	Engine::Add_GameObject_To_CollisionList(L"Monster_Body", this);

	return 0;
}

_int CFallenDog::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CMonster::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CFallenDog::Render_GameObject(const _float& fTimeDelta)
{
	CMonster::Render_GameObject(fTimeDelta);
}

HRESULT CFallenDog::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	_ulong dwFlag = TS_RENDEROFFSET;
	FAILED_CHECK_RETURN(CMonster::Add_Component(wContainerIdx, wstrBufferTag, dwFlag), E_FAIL);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(180.f)));

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
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_Root_FD")->CombinedTransformationMatrix;
	m_pColliderCom->Set_Bone(&pLinker->matLink);
	m_LinkList.push_back(pLinker);



	return S_OK;
}

void CFallenDog::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	CMonster::Change_State(SC_INT(m_eCurState));

	m_ePreState = m_eCurState;
}

_int CFallenDog::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CFallenDog::EFallenDog::IDLE:
		iExit = Update_Idle(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::RUN:
		iExit = Update_Run(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::FALL:
		iExit = Update_Fall(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::IMPACT_BACK:
	case CFallenDog::EFallenDog::IMPACT_FRONT:
	case CFallenDog::EFallenDog::IMPACT_LEFT:
	case CFallenDog::EFallenDog::IMPACT_RIGHT:
	case CFallenDog::EFallenDog::IMPACT_HEAVY:
		iExit = Update_Impact(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::DEATH01:
		iExit = Update_Death(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::ATTACK01:
		iExit = Update_Attack01(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::ATTACK02:
		iExit = Update_Attack02(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::ATK_3HIT:
		iExit = Update_Atk3Hit(fTimeDelta);
		break;
	case CFallenDog::EFallenDog::START_WALK:
		iExit = Update_StartWalk(fTimeDelta);
		break;
	}

	return iExit;
}

void CFallenDog::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;

	m_fRim = 1.f;

	Add_DamageFont(iAttack);
	Add_BloodParticle(pAtkPos);

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_eCurState = EFallenDog::DEATH01;
		m_pColliderCom->Deactivate();
	}
	else
	{
		_vec3 vPos, vLook, vRight, vDir;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));
		D3DXVec3Normalize(&vRight, m_pTransformCom->Get_Info(Engine::INFO_RIGHT));
		D3DXVec3Normalize(&vDir, &(vPos - *pAtkPos));

		_float fLookDot = D3DXVec3Dot(&vLook, &vDir);
		_float fRightDot = D3DXVec3Dot(&vRight, &vDir);

		EFallenDog eState = fabsf(fLookDot) > fabsf(fRightDot) ? (fLookDot > 0.f ? EFallenDog::IMPACT_BACK : EFallenDog::IMPACT_FRONT) :
																 (fRightDot > 0.f ? EFallenDog::IMPACT_LEFT : EFallenDog::IMPACT_RIGHT);

		if (m_eCurState == eState)
		{
			CMonster::Change_State(SC_INT(EFallenDog::IDLE));
			CMonster::Change_State(SC_INT(eState));
		}
		m_eCurState = eState;
	}
}

void CFallenDog::On_NaviMesh()
{
	if (m_eCurState == EFallenDog::DEATH01)
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
		if (m_eCurState == EFallenDog::IDLE)
			m_eCurState = EFallenDog::FALL;
	}
		break;
	}
}

HRESULT CFallenDog::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_INT(EFallenDog::EEND));
	m_vecAnimSetType.resize(SC_INT(EFallenDog::EEND));
	m_vecAnimItpl.resize(SC_INT(EFallenDog::EEND));

	Set_AnimVariable(SC_INT(EFallenDog::IDLE), 24, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::RUN), 35, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::SPAWN), 40, AT_ONCE, 0.0000005);

	Set_AnimVariable(SC_INT(EFallenDog::ATTACK01), 13, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::ATTACK02), 14, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::ATK_3HIT), 9, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EFallenDog::DEATH01), 16, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::FALL), 32, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EFallenDog::IMPACT_BACK), 25, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::IMPACT_FRONT), 26, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::IMPACT_LEFT), 28, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::IMPACT_RIGHT), 29, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EFallenDog::IMPACT_HEAVY), 27, AT_ONCE, 0.05);

	Set_AnimVariable(SC_INT(EFallenDog::START_WALK), 52, AT_REPEAT, 0.05);

	return S_OK;
}

void CFallenDog::Attack_Stop()
{
	m_pLeftHand->Stop();
	m_pRightHand->Stop();
	m_pHead->Stop();
}

_int CFallenDog::Update_Hands(const _float & fTimeDelta)
{
	_int iExit = m_pLeftHand->Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = m_pRightHand->Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = m_pHead->Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

_int CFallenDog::Update_Idle(const _float & fTimeDelta)
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
		m_eCurState = EFallenDog::ATTACK02;
		return 0;
	} 
	else if (fDistance < 2.5f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EFallenDog::ATTACK01;
		return 0;
	}
	else if (fDistance < 30.f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EFallenDog::RUN;
		return 0;
	}

	return 0;
}

_int CFallenDog::Update_Run(const _float & fTimeDelta)
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
		m_eCurState = EFallenDog::ATTACK02;
		return 0;
	}
	else if (fDistance < 2.5f)
	{
		m_eCurState = EFallenDog::ATTACK01;
		return 0;
	}
	else if (fDistance < 10.f)
	{
		m_eCurState = EFallenDog::ATK_3HIT;
		return 0;
	}
	else if (fDistance > 30.f)
	{
		m_eCurState = EFallenDog::IDLE;
		return 0;
	}
	return 0;
}

_int CFallenDog::Update_Fall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	if (vPos.y < -20.f)
		m_eCurState = EFallenDog::DEATH01;

	return 0;
}

_int CFallenDog::Update_Spawn(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EFallenDog::IDLE;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_fallendog_spawn_01.ogg", Engine::CSoundMgr::EFFECT);
		//Engine::Play_Sound(L"en_fallendog_spawn_vo_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CFallenDog::Update_Death(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bDissolve = true;
		
		if (m_fDissolveTime > 1.5f)
			m_bIsDead = true;

		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_fallendog_death_01.ogg", Engine::CSoundMgr::EFFECT);
		//Engine::Play_Sound(L"en_fallendog_death_vo_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CFallenDog::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EFallenDog::IDLE;
		return 0;
	}

	return 0;
}

_int CFallenDog::Update_StartWalk(const _float & fTimeDelta)
{
	m_fStartWalkTime += fTimeDelta;

	if (m_fStartWalkTime > 3.f)
	{
		m_eCurState = EFallenDog::IDLE;
		return 0;
	}

	_vec3 vLook;
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));
	
	m_vDisplacement += vLook * m_fSpeed * fTimeDelta * 0.5f;

	return 0;
}

_int CFallenDog::Update_Attack01(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EFallenDog::IDLE;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_fallendog_atk_slash_01.ogg", Engine::CSoundMgr::EFFECT);
		//Engine::Play_Sound(L"en_fallendog_atk_slash_vo_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.1 && dProgress < 0.7)
		m_pLeftHand->Attack();
	else
		m_pLeftHand->Stop();

	return 0;
}

_int CFallenDog::Update_Attack02(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EFallenDog::IDLE;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_fallendog_atk_slash_02.ogg", Engine::CSoundMgr::EFFECT);
		//Engine::Play_Sound(L"en_fallendog_atk_slash_vo_02.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (dProgress > 0.1 && dProgress < 0.7)
		m_pRightHand->Attack();
	else
		m_pRightHand->Stop();


	return 0;
}

_int CFallenDog::Update_Atk3Hit(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EFallenDog::IDLE;
		m_bEffectSound = false;
		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_fallendog_atk_3hitcombo_01.ogg", Engine::CSoundMgr::EFFECT);
		//Engine::Play_Sound(L"en_fallendog_atk_3hitcombo_vo_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();

	if (dProgress > 31./116. && dProgress < 45./116.)
		m_pRightHand->Attack();
	else if (dProgress > 46. / 116. && dProgress < 62. / 116.)
	{
		m_pRightHand->Stop();
		m_pLeftHand->Attack();
	}
	else
	{
		m_pRightHand->Stop();
		m_pLeftHand->Stop();
		if (dProgress > 74. / 116. && dProgress < 91. / 116.)
		{
			m_pHead->Attack();
		}
		else
			m_pHead->Stop();
	}
		


	return 0;
}

CFallenDog * CFallenDog::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CFallenDog* pInst = new CFallenDog(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);

	return pInst;
}

CFallenDog * CFallenDog::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag, EFallenDog eStartState)
{
	CFallenDog* pInst = new CFallenDog(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, eStartState)))
		::Safe_Release(pInst);

	return pInst;
}

void CFallenDog::Free()
{
	::Safe_Release(m_pHead);
	::Safe_Release(m_pLeftHand);
	::Safe_Release(m_pRightHand);
	::Safe_Release(m_pObserver);
	CMonster::Free();
}

