#include "stdafx.h"
#include "HellHound.h"
#include "SphereCollider.h"
#include "DynamicMesh.h"
#include "Transform.h"

#include "Export_Function.h"
#include "Observer.h"
#include "InvisibleWeapon.h"

CHellHound::CHellHound(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CHellHound::~CHellHound()
{
}

HRESULT CHellHound::Ready_GameObject()
{
	return S_OK;
}

HRESULT CHellHound::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_AnimIndex(), E_FAIL);


	m_pTransformCom->Set_Scale(0.01f);

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	m_pHead = CInvisibleWeapon::Create(m_pGraphicDev, 1.5f);
	NULL_CHECK_RETURN(m_pHead, E_FAIL);

	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("Bone_HellHound_Head")->CombinedTransformationMatrix;
	m_pHead->Set_Parent(&pLinker->matLink, m_pTransformCom);
	m_pHead->Add_TransformFlags(TS_PARENT_NO_SCALE);
	m_LinkList.push_back(pLinker);

	m_fSpeed = 8.f;
	m_iHP = 100;
	m_iHPMax = 100;

	m_eCurState = EHellHound::SPAWN;
	CMonster::Change_State(SC_INT(m_eCurState));
	PlayAnimation(0.1f);
	return S_OK;
}

_int CHellHound::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	Update_State(fTimeDelta);
	Change_State();

	_int iExit = CMonster::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = m_pHead->Update_GameObject(fTimeDelta);
	if (iExit & 0x80000000)
		return -1;

	if (m_eCurState != EHellHound::DEATH01 && m_eCurState != EHellHound::SPAWN)
		Gravity(fTimeDelta);
	//On_NaviMesh();

	Engine::Add_GameObject_To_CollisionList(L"Monster_Body", this);

	return 0;
}

_int CHellHound::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CMonster::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CHellHound::Render_GameObject(const _float& fTimeDelta)
{
	CMonster::Render_GameObject(fTimeDelta);
}

HRESULT CHellHound::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	_ulong dwFlag = TS_RENDEROFFSET;
	FAILED_CHECK_RETURN(CMonster::Add_Component(wContainerIdx, wstrBufferTag, dwFlag), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matColl;
	D3DXMatrixIdentity(&matColl);
	//matColl._42 = 0.5f;
	pCom = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_BONE, m_pTransformCom, &matColl, 1.5f, L"Col_Monster");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	m_pColliderCom->Activate();
	
	BONE_LINKER* pLinker = new BONE_LINKER;
	ZeroMemory(pLinker, sizeof(BONE_LINKER));
	pLinker->pOriginBone = &m_pMeshCom->Get_FrameByName("null_Spine")->CombinedTransformationMatrix;
	m_pColliderCom->Set_Bone(&pLinker->matLink);
	m_LinkList.push_back(pLinker);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(180.f)));


	return S_OK;
}

void CHellHound::Change_State()
{
	if (m_ePreState == m_eCurState)
		return;

	CMonster::Change_State(SC_INT(m_eCurState));

	m_ePreState = m_eCurState;
}

_int CHellHound::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;

	switch (m_eCurState)
	{
	case CHellHound::EHellHound::IDLE:
		iExit = Update_Idle(fTimeDelta);
		break;
	case CHellHound::EHellHound::RUN:
		iExit = Update_Run(fTimeDelta);
		break;
	case CHellHound::EHellHound::FALL:
		iExit = Update_Fall(fTimeDelta);
		break;
	case CHellHound::EHellHound::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CHellHound::EHellHound::IMPACT_BACK:
	case CHellHound::EHellHound::IMPACT_FRONT:
	case CHellHound::EHellHound::IMPACT_LEFT:
	case CHellHound::EHellHound::IMPACT_RIGHT:
		iExit = Update_Impact(fTimeDelta);
		break;
	case CHellHound::EHellHound::DEATH01:
		iExit = Update_Death(fTimeDelta);
		break;
	case CHellHound::EHellHound::ATTACK01:
		iExit = Update_Attack01(fTimeDelta);
		break;
	}

	return iExit;
}

void CHellHound::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;

	m_fRim = 1.f;
	Add_DamageFont(iAttack);
	Add_BloodParticle(pAtkPos);


	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_eCurState = EHellHound::DEATH01;
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

		EHellHound eState = fabsf(fLookDot) > fabsf(fRightDot) ? (fLookDot > 0.f ? EHellHound::IMPACT_BACK : EHellHound::IMPACT_FRONT) :
																 (fRightDot > 0.f ? EHellHound::IMPACT_LEFT : EHellHound::IMPACT_RIGHT);

		if (m_eCurState == eState)
		{
			CMonster::Change_State(SC_INT(EHellHound::IDLE));
			CMonster::Change_State(SC_INT(eState));
		}
		m_eCurState = eState;
	}
}

void CHellHound::On_NaviMesh()
{
	if (m_eCurState == EHellHound::DEATH01)
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
		if (m_eCurState == EHellHound::IDLE)
			m_eCurState = EHellHound::FALL;
	}
		break;
	}
}

HRESULT CHellHound::SetUp_AnimIndex()
{
	m_vecAnimSetIdx.resize(SC_INT(EHellHound::EEND));
	m_vecAnimSetType.resize(SC_INT(EHellHound::EEND));
	m_vecAnimItpl.resize(SC_INT(EHellHound::EEND));

	Set_AnimVariable(SC_INT(EHellHound::IDLE), 14, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::RUN), 35, AT_REPEAT, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::SPAWN), 2, AT_ONCE, 0.0000005);

	Set_AnimVariable(SC_INT(EHellHound::ATTACK01), 0, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::DEATH01), 7, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::FALL), 32, AT_REPEAT, 0.05);

	Set_AnimVariable(SC_INT(EHellHound::IMPACT_BACK), 15, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::IMPACT_FRONT), 16, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::IMPACT_LEFT), 17, AT_ONCE, 0.05);
	Set_AnimVariable(SC_INT(EHellHound::IMPACT_RIGHT), 18, AT_ONCE, 0.05);

	return S_OK;
}

void CHellHound::Attack_Stop()
{
	m_pHead->Stop();
}

void CHellHound::PlayAnimation(const _float & fTimeDelta)
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
		break;
	}

	if (m_pMeshCom->Is_InItpProgress())
		return;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	if (m_eCurState != EHellHound::RUN && (vChangedPos.x || vChangedPos.y || vChangedPos.z))
	{
		D3DXVec3TransformNormal(&vChangedPos, &vChangedPos, m_pTransformCom->Get_RenderMatrix());
		m_vAnimDiffer = m_vOldPos + vChangedPos - vPos;
	}

	Update_Link();
}

_int CHellHound::Update_Idle(const _float & fTimeDelta)
{
	_vec3 vPos, vPlayerPos, vDir, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	D3DXVec3Normalize(&vLook, m_pTransformCom->Get_Info(Engine::INFO_LOOK));

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	_float fDistance = D3DXVec3Length(&(vDir = vPlayerPos - vPos));
	D3DXVec3Normalize(&vDir, &vDir);

	if (fDistance < 6.f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EHellHound::ATTACK01;
		return 0;
	}
	else if (fDistance < 30.f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);
		m_eCurState = EHellHound::RUN;
		return 0;
	}

	return 0;
}

_int CHellHound::Update_Run(const _float & fTimeDelta)
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

	if (fDistance < 6.f)
	{
		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &vDir, &AXIS_Y);

		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		m_eCurState = EHellHound::ATTACK01;
		return 0;
	}
	else if (fDistance > 30.f)
	{
		m_eCurState = EHellHound::IDLE;
		return 0;
	}
	return 0;
}

_int CHellHound::Update_Fall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	if (vPos.y < -20.f)
		m_eCurState = EHellHound::DEATH01;

	return 0;
}

_int CHellHound::Update_Spawn(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EHellHound::IDLE;
		return 0;
	}

	return 0;
}

_int CHellHound::Update_Death(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_bDissolve = true;

		if (m_fDissolveTime > 2.f)
		{
			m_bIsDead = true;
			m_bEffectSound = false;
		}

		

		return 0;
	}

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;

		Engine::Play_Sound(L"en_hellhound_death_vo_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	return 0;
}

_int CHellHound::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EHellHound::IDLE;
		return 0;
	}

	return 0;
}

_int CHellHound::Update_Attack01(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EHellHound::IDLE;
		m_bEffectSound = false;
		return 0;
	}

	_double dProgress = m_pMeshCom->Get_AnimationProgress();
	if (dProgress > 0.2 && dProgress < 0.5)
		m_pHead->Attack();
	else
		m_pHead->Stop();

	if (!m_bEffectSound)
	{
		m_bEffectSound = true;
		Engine::Play_Sound(L"en_hellhound_atk_bite_01.ogg", Engine::CSoundMgr::MONSTER);
	}

	return 0;
}

CHellHound * CHellHound::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CHellHound* pInst = new CHellHound(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);

	return pInst;
}

void CHellHound::Free()
{
	::Safe_Release(m_pHead);
	::Safe_Release(m_pObserver);
	CMonster::Free();
}

