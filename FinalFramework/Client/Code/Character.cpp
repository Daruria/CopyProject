#include "stdafx.h"
#include "Character.h"
#include "Transform.h"
#include "SphereCollider.h"

#include "Export_Function.h"
#include "Monster.h"
#include "Weapon.h"
#include "HitGround.h"
#include "DamageEffect.h"
#include "TrailEffect.h"
#include "FollowRoute.h"
#include "Particle.h"
#include "HealingObject.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CDynamicObject(pGraphicDev)
{
}

CCharacter::~CCharacter()
{
}

HRESULT CCharacter::Add_Component(const _ushort & wContainerIdx, const wstring & wstrBufferTag)
{
	_ulong dwFlag = TS_RENDEROFFSET;

	FAILED_CHECK_RETURN(CDynamicObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlag), E_FAIL);

	_matrix matRenderOffset;
	m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(90.f)));

	Engine::CComponent*		pCom = nullptr;

	pCom = m_pNaviCom = DC_ENG(CNavigationMesh, Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pCom);

	_matrix matColl;
	D3DXMatrixIdentity(&matColl);

	pCom = m_pColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_BONE, m_pTransformCom, &matColl, 1.f, L"Col_PlayerBody");
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	m_pColliderCom->Activate();
	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("null_Spine");
	m_pColliderCom->Set_Bone(&pFrame->CombinedTransformationMatrix);
	//pCom = m_pTransformCom = m_pPlayerTransformCom = CPlayerTransform::Create();
	//NULL_CHECK_RETURN(pCom, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pCom);

	//pCom = m_pRenderer = Engine::Get_Renderer();
	//NULL_CHECK_RETURN(pCom, E_FAIL);
	//m_pRenderer->AddRef();
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pCom);

	//pCom = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(wContainerIdx, wstrBufferTag));
	//NULL_CHECK_RETURN(pCom, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pCom);


	return S_OK;
}

HRESULT CCharacter::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_RenderMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);


	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);


	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	pEffect->SetMatrix("g_matPreWorld", m_pTransformCom->Get_PreWorldMatrix());
	pEffect->SetMatrix("g_matPreView", Engine::Get_MainCamera()->Get_PreView());

	pEffect->SetFloat("g_fRim", m_fRim);
	pEffect->SetFloat("g_fCapture", m_fCapture);
	pEffect->SetFloat("g_fAlpha", 1.f);

	return S_OK;
}

HRESULT CCharacter::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;	

	return S_OK;
}

_int CCharacter::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);
	Change_State();
	Update_State(fTimeDelta);

	if (m_fRim > 0.f)
	{
		m_fRim -= fTimeDelta;
	}

	//static _float s_fSpeed = 1.f;
	//m_fRim += fTimeDelta * s_fSpeed;
	//if (m_fRim > 1.f)
	//{
	//	s_fSpeed = -1.f;
	//	m_fRim = 1.f;
	//}
	//else if (m_fRim < 0.f)
	//{
	//	s_fSpeed = 1.f;
	//	m_fRim = 0.f;
	//}

	//m_fCapTimeCount += fTimeDelta;
	//if (m_fCapTimeCount > m_fCapTime)
	//{
	//	m_fCapture = 1.f;
	//	m_fCapTimeCount = 0.f;
	//}
	//else
	//	m_fCapture = 0.f;

	m_fCapture = 1.f;

	_int iExit = CDynamicObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

#ifdef _DEBUG
	if (Engine::KeyDown(DIK_0))
		m_bNaviMeshRender = !m_bNaviMeshRender;

	if (Engine::KeyDown(DIK_GRAVE))
		system("cls");

	if (Engine::KeyDown(DIK_SEMICOLON))
	{
		_vec3 vPos, vWorld;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

		m_pTransformCom->Get_WorldPos(&vWorld);
		cout << "Info_Pos" << endl;
		cout << "X : " << vPos.x << "\tY : " << vPos.y << "\tZ : " << vPos.z << endl;
		cout << "World_Pos" << endl;
		cout << "X : " << vWorld.x << "\tY : " << vWorld.y << "\tZ : " << vWorld.z << endl;
		cout << "Navi Index : " << m_pNaviCom->Get_Index() << endl;
	}

#endif // _DEBUG

	_vec3 vScale;
	m_pTransformCom->Get_Scale(&vScale);
	if (isnan(vScale.x) || isnan(vScale.y) || isnan(vScale.z))
		m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);


	Engine::Add_GameObject_To_CollisionList(L"Player_Body", this);



	return 0;
}

_int CCharacter::LateUpdate_GameObject(const _float& fTimeDelta)
{

	

	_int iExit = CDynamicObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CCharacter::Render_GameObject(const _float& fTimeDelta)
{
	PlayAnimation(fTimeDelta);
	Decision_Position();

#ifdef _DEBUG
	if (m_bNaviMeshRender)
		m_pNaviCom->Render_NavigationMesh();
#endif

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetRenderMatrix());
	//m_pMeshCom->Render_Meshes();

	LPD3DXEFFECT	pEffect = m_vecShaderCom[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}


void CCharacter::Collision_Enter(CGameObject * pOpponent)
{
	CMonster*  pMonster = dynamic_cast<CMonster*>(pOpponent);
	if (pMonster)
	{
		_int iAttack = pMonster->Get_Attack();
		_vec3 vMonsterPos;
		pMonster->Get_Transform()->Get_WorldPos(&vMonsterPos);
		Hit(iAttack, &vMonsterPos);
		return;
	}
	
	CWeapon* pWeapon = dynamic_cast<CWeapon*>(pOpponent);
		
	if (pWeapon)
	{
		_int iAttack = pWeapon->Get_Attack();
		_vec3 vOppoPos;
		pWeapon->Get_Transform()->Get_WorldPos(&vOppoPos);
		Hit(iAttack, &vOppoPos);
		return;
	}

	CHitGround* pHitGround = dynamic_cast<CHitGround*>(pOpponent);

	if (pHitGround)
	{
		_int iAttack = pHitGround->Get_Attack();
		_vec3 vHitGround;
		pHitGround->Get_Transform()->Get_Info(Engine::INFO_POS, &vHitGround);
		Hit(iAttack, &vHitGround);
		return;
	}

	CHealingObject* pHealingObject = dynamic_cast<CHealingObject*>(pOpponent);

	if (pHealingObject)
	{
		_int iHealing = pHealingObject->Get_HealingPoint();
		m_iHP += iHealing;

		m_fRim = 1.f;
		if (m_iHPMax < m_iHP)
			m_iHP = m_iHPMax;

		Engine::Update_Subject(L"Player", SC_INT(EPlayerData::HP), &m_iHP);
		
		return;
	}
}

void CCharacter::Collision_Stay(CGameObject * pOpponent)
{
}

void CCharacter::Collision_Leave(CGameObject * pOpponent)
{
}

void CCharacter::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;


	m_fRim = 1.f;
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	CDamageEffect* pDmgFx = CDamageEffect::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_DamageFont", D3DXCOLOR(1.f, 0.f, 0.f, 1.f), iAttack);
	NULL_CHECK_RETURN(pDmgFx, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_DMG", pDmgFx), );
	pDmgFx->Set_LifeTime(1.5f);
	pDmgFx->Set_Pos(&vPos);
	pDmgFx->Set_DigitScale(0.01f);
	pDmgFx->SetUp_DigitScaling(0.f, 0.005f, 0.04f, -0.12f);

	if (m_iHP <= 0)
	{
		m_iHP = 0;
	}

	Engine::Update_Subject(L"Player", SC_INT(EPlayerData::HP), &m_iHP);



}

void CCharacter::Move_Pos(const _vec3 * pPos)
{

}

_ulong CCharacter::Get_NaviIndex() const
{
	return m_pNaviCom->Get_Index();
}

void CCharacter::Set_NaviIndex(const _ulong & dwIndex)
{
	m_pNaviCom->Set_Index(dwIndex);
}

void CCharacter::Falling(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	if (vPos.y < -20.f)
	{
		_vec3 vReturnPos;
		m_pNaviCom->Get_Center(m_dwOldIndex, &vReturnPos);
		m_pTransformCom->Set_Pos(vReturnPos);
		m_pNaviCom->Set_Index(m_dwOldIndex);
		m_eCurState.ePlayer = EPlayer::IDLE;
		m_fGravity = 0.f;
	}

	if (m_pNaviCom->Get_Option() == 0)
	{
		_float fY = m_pNaviCom->Get_Height(&vPos);

		if (vPos.y <= fY)
		{
			m_pTransformCom->Set_PosY(fY);
			m_eCurState.ePlayer = EPlayer::IDLE;
			m_fGravity = 0.f;
		}
	}
}

void CCharacter::SmoothTurn(const _float & fTimeDelta)
{
	_vec3 vUp, vLook;
	m_pTransformCom->Get_Info(Engine::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);

	_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &m_vMoveDir, &vUp);
	_float fTargetAngle = Engine::CMyMath::AngleBetween2Vectors(&AXIS_Z, &m_vMoveDir, &vUp);
	_float fRotDir = fAngle < 0.f ? -1.f : 1.f;
	_float	fAngleDelta = fRotDir * fTimeDelta * m_fSpeed;
	if (fabsf(fAngle) < fabsf(fAngleDelta))
		m_pTransformCom->Set_AngleY(fTargetAngle);
	else
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngleDelta);
}

void CCharacter::Gravity(const _float & fTimeDelta)
{
	m_fGravity += fTimeDelta * GRAVITY * 50.f;

	m_vDisplacement.y += -m_fGravity * fTimeDelta;
	//m_pTransformCom->Move_PosY(-m_fGravity);

}

void CCharacter::PlayAnimation(const _float & fTimeDelta)
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
	if (vChangedPos.x || vChangedPos.y || vChangedPos.z)
	{
		D3DXVec3TransformNormal(&vChangedPos, &vChangedPos, m_pTransformCom->Get_RenderMatrix());
		m_vAnimDiffer = m_vOldPos + vChangedPos - vPos;

		//_vec3 vDifference = m_vOldPos + vChangedPos - vPos;

		//vTargetPos = m_pNaviCom->Move_OnNavigationMesh(&vPos, &vDifference, nullptr);
		//m_pTransformCom->Set_Pos(vTargetPos);

	}
}

void CCharacter::Stop_Attack()
{

}

void CCharacter::On_NaviMesh()
{
	//if (m_pNaviCom->Get_Option() == Engine::CCell::NO_OPTION)
	//{
	//	_float fY = m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS));
	//	m_pTransformCom->Set_PosY(fY);
	//	m_fGravity = 0.f;
	//}

	switch (m_pNaviCom->Get_Option())
	{
	case Engine::CCell::NO_OPTION:
	{
		if (m_eCurState == EPlayer::JUMP)
			return;
		_float fY = m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS));
		m_pTransformCom->Set_PosY(fY);
		m_fGravity = 0.f;
	}
	break;

	case Engine::CCell::FALL:
	{
		if (m_eCurState == EPlayer::IDLE)
			m_eCurState = EPlayer::FALL;
	}
	break;
	}
}

void CCharacter::Decision_Position()
{
	if (m_eCurState.eWar == EWar::ATK_FLAME_BRAND_LOOP)
		ZeroMemory(&m_vAnimDiffer, sizeof(_vec3));

	if (isnan(m_vAnimDiffer.x) || isnan(m_vAnimDiffer.y) || isnan(m_vAnimDiffer.z))
		ZeroMemory(&m_vAnimDiffer, sizeof(_vec3));

	if (isnan(m_vDisplacement.x) || isnan(m_vDisplacement.y) || isnan(m_vDisplacement.z))
		ZeroMemory(&m_vDisplacement, sizeof(_vec3));

	_vec3 vTargetDisplacement = m_vAnimDiffer + m_vDisplacement;
	_vec3 vPos, vTargetPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//vTargetPos = vPos + vTargetDisplacement;
	
	//if (!Engine::Check_ByList(&vTargetPos, DC_ENG(CSphereCollider, m_pColliderCom)->Get_Radius(), L"MapObject"));
	m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNavigationMesh(&vPos, &vTargetDisplacement, nullptr));

	Check_NaviIndex();

	On_NaviMesh();
	ZeroMemory(&m_vDisplacement, sizeof(_vec3));
	ZeroMemory(&m_vAnimDiffer, sizeof(_vec3));
}

void CCharacter::Check_NaviIndex()
{
	_ulong dwIndex = m_pNaviCom->Get_Index(m_pTransformCom->Get_Info(Engine::INFO_POS));
	if (m_pNaviCom->Get_Index() != dwIndex)
		m_pNaviCom->Set_Index(dwIndex);
}

void CCharacter::Set_AnimVariable(_int iState, _int iAnimIdx, ANIMATION_TYPE eAnimType, _double dItpl_Time)
{
	m_vecAnimSetIdx[iState] = iAnimIdx;
	m_vecAnimSetType[iState] = eAnimType;
	m_vecAnimItpl[iState] = dItpl_Time;
}

void CCharacter::Add_BloodParticle(const _vec3 * pAtkPos)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	vPos += *pAtkPos;
	vPos *= 0.5f;

	CParticle* pEffect = CParticle::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_EffectDot", 50);
	NULL_CHECK(pEffect);
	Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Particle", pEffect);
	pEffect->Set_Scale(0.1f);
	pEffect->Set_Speed(30.f);
	pEffect->Set_Pos(&vPos);
}

void CCharacter::Change_State(_int iState)
{
	Stop_Attack();
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[iState], m_vecAnimItpl[iState]);
	m_eCurAnimType = m_vecAnimSetType[iState];
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
	if (m_eCurState != EPlayer::DASH)
		m_pColliderCom->Activate();

	m_bEffect = false;
	m_pRenderer->Off_Blur();
}

void CCharacter::Change_Idle()
{
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[SC_PSTATE(IDLE)], m_vecAnimItpl[SC_PSTATE(IDLE)]);
	m_eCurAnimType = AT_REPEAT;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
}

void CCharacter::Change_Run()
{
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[SC_PSTATE(RUN)], m_vecAnimItpl[SC_PSTATE(RUN)]);
	m_eCurAnimType = AT_REPEAT;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
}

void CCharacter::Change_Jump()
{
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[SC_PSTATE(JUMP)], m_vecAnimItpl[SC_PSTATE(JUMP)]);
	m_eCurAnimType = AT_ONCE;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
}

void CCharacter::Change_Fall()
{
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[SC_PSTATE(FALL)], m_vecAnimItpl[SC_PSTATE(FALL)]);
	m_eCurAnimType = AT_ONCE;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
}

void CCharacter::Change_Dash()
{
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[SC_PSTATE(DASH)], m_vecAnimItpl[SC_PSTATE(DASH)]);
	m_eCurAnimType = AT_ONCE;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
}

void CCharacter::Change_Test()
{
	m_pMeshCom->Set_AnimationSet(m_uiTestAniIndex, m_vecAnimItpl[SC_PSTATE(TEST)]);
	m_eCurAnimType = AT_REPEAT;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
}

void CCharacter::Update_Idle(const _float & fTimeDelta)
{
	if (m_pNaviCom->Get_Option() == 1)
	{
		m_dwOldIndex = m_pNaviCom->Get_NearIndex(Engine::CCell::NO_OPTION);
		m_eCurState.ePlayer = EPlayer::FALL;
		return;
	}
	//else
	//{
	//	m_pTransformCom->Set_PosY(m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS)));
	//}
}

void CCharacter::Update_Run(const _float & fTimeDelta)
{
	if (m_pNaviCom->Get_Option() == 1)
	{
		m_dwOldIndex = m_pNaviCom->Get_NearIndex(Engine::CCell::NO_OPTION);
		m_eCurState.ePlayer = EPlayer::FALL;
		return;
	}

	m_fRunSoundItvl += fTimeDelta;
	if (m_fRunSoundItvl > 0.4f)
	{
		m_fRunSoundItvl = 0.f;
		Engine::Play_Sound(L"char_war_foot_01.ogg", Engine::CSoundMgr::EFFECT);
	}

	//else
	//{
	//	m_pTransformCom->Set_PosY(m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS)));
	//}
}

void CCharacter::Update_Jump(const _float & fTimeDelta)
{
	//m_pTransformCom->Move_PosY(fTimeDelta * m_fJumpSpeed);
	m_vDisplacement.y += fTimeDelta * m_fJumpSpeed;

	Gravity(fTimeDelta);

	Falling(fTimeDelta);

}

void CCharacter::Update_Fall(const _float & fTimeDelta)
{
	Gravity(fTimeDelta);

	Falling(fTimeDelta);
}

void CCharacter::Update_Dash(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState.ePlayer = EPlayer::IDLE;
	}

	if (m_pMeshCom->Get_AnimationProgress() > 0.4)
		m_pColliderCom->Activate();

	if (!m_bEffect)
	{
		m_bEffect = true;
		CTrailEffect* pTrailEffect = CTrailEffect::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_DashTrail", D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Engine::RENDER_DISTORTION, 5);
		NULL_CHECK_RETURN(pTrailEffect, );
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_TrailDistortion", pTrailEffect), );
		pTrailEffect->Set_LifeTime(0.5f);
		pTrailEffect->Set_InterpoleCnt(0);
		pTrailEffect->Set_UpLength(2.f);
		pTrailEffect->Set_DownLength(2.f);
		pTrailEffect->Set_Axis(Engine::INFO_RIGHT);

		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		pTrailEffect->Set_Pos(&vPos);

		Engine::CTransform*	pEffectTrans = dynamic_cast<Engine::CTransform*>(pTrailEffect->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
		NULL_CHECK_RETURN(pEffectTrans, );

		CFollowRoute* pFollowRoute = CFollowRoute::Create(pEffectTrans, this);
		NULL_CHECK_RETURN(pFollowRoute, );

		pTrailEffect->Add_MoveOnRoute(pFollowRoute);

	}
	//m_vDisplacement += m_vMoveDir * fTimeDelta * 100.f;

	//if (!m_bBlur)
	//{
	//	m_bBlur = true;
	//	m_pRenderer->On_Blur();
	//}
	//
	//if (m_pNaviCom->Get_Option() == 0)
	//{
	//	m_pTransformCom->Set_PosY(m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS)));
	//}
}

void CCharacter::Update_Test(const _float & fTimeDelta)
{

}

void CCharacter::Update_Impact(const _float & fTimeDelta)
{
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = EPlayer::IDLE;
	}

	if (!m_bEffect)
	{
		m_bEffect = true;
		m_pRenderer->On_Blur();
	}
}

void CCharacter::KeyInput_Move(const _float & fTimeDelta)
{
	_vec3 vCamLook, vCamRight;
	Engine::Get_MainCameraLook(&vCamLook);
	Engine::Get_MainCameraRight(&vCamRight);

	vCamRight.y = 0;
	vCamLook.y = 0;

	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vCamRight, &vCamRight);

	_vec3 vMove = { 0.f, 0.f, 0.f };

	if (Engine::KeyPress(DIK_W))
	{
		vMove += vCamLook;
	}
	if (Engine::KeyPress(DIK_S))
	{
		vMove -= vCamLook;
	}
	if (Engine::KeyPress(DIK_A))
	{
		vMove -= vCamRight;
	}
	if (Engine::KeyPress(DIK_D))
	{
		vMove += vCamRight;
	}


	if (0.f != vMove.x || 0.f != vMove.y || 0.f != vMove.z)
	{
		D3DXVec3Normalize(&m_vMoveDir, &vMove);

		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

		SmoothTurn(fTimeDelta);

		//m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNavigationMesh(&vPos, &(m_vMoveDir * m_fSpeed * fTimeDelta), nullptr));
		

		if ((m_eCurState != EPlayer::FALL && m_eCurState != EPlayer::JUMP) || 
			m_pNaviCom->Get_Height(&vPos) < vPos.y)
			m_vDisplacement += m_vMoveDir * m_fSpeed * fTimeDelta;

		if (m_eCurState != EPlayer::JUMP && m_eCurState != EPlayer::FALL && m_eCurState.eWar != EWar::ATK_FLAME_BRAND_LOOP)
			m_eCurState = EPlayer::RUN;
		
	}
	else
	{
		switch (m_eCurState.ePlayer)
		{
		case EPlayer::IDLE:
		case EPlayer::RUN:
			m_eCurState.ePlayer = EPlayer::IDLE;
			break;
		case EPlayer::JUMP:
			break;
		case EPlayer::FALL:
			break;
		case EPlayer::DASH:
			break;
		case EPlayer::FINISH:
			break;
		default:
			break;
		}

	}
}

void CCharacter::KeyInput_Jump(const _float & fTimeDelta)
{
	if (EPlayer::JUMP == m_eCurState.ePlayer)
		return;

	if (Engine::KeyDown(DIK_SPACE))
	{
		Stop_Attack();
		m_dwOldIndex = m_pNaviCom->Get_Index();
		m_eCurState.ePlayer = EPlayer::JUMP;
		m_pTransformCom->Move_PosY(fTimeDelta * m_fJumpSpeed);
	}

}

void CCharacter::KeyInput_Fall(const _float & fTimeDelta)
{
}

void CCharacter::KeyInput_Dash(const _float & fTimeDelta)
{
	//if (EPlayer::DASH == m_eCurState.ePlayer)
	//	return;

	if (Engine::KeyDown(DIK_LSHIFT))
	{
		Stop_Attack();
		_vec3 vUp, vLook;
		m_pTransformCom->Get_Info(Engine::INFO_UP, &vUp);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vLook);

		_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&vLook, &m_vMoveDir, &AXIS_Y);
		m_pTransformCom->Rotation(Engine::ROT_Y, fAngle);

		if (EPlayer::DASH == m_eCurState.ePlayer)
		{
			Change_State(SC_PSTATE(IDLE));
			Change_State(SC_PSTATE(DASH));
		}

		m_pColliderCom->Deactivate();
		m_eCurState.ePlayer = EPlayer::DASH;

		Engine::Play_Sound(L"char_war_dash_1_01.ogg", Engine::CSoundMgr::EFFECT);
	}
}

void CCharacter::KeyInput_Test(const _float & fTimeDelta)
{
	/*if (Engine::KeyDown(DIK_UP))
	{
		++m_uiTestAniIndex;
		_ulong MaxAnim = m_pMeshCom->Get_AnimationMax();
		if (m_pMeshCom->Get_AnimationMax() <= m_uiTestAniIndex)
			m_uiTestAniIndex = 0;
		Change_Test();
		cout << "Animation Index : " << m_uiTestAniIndex << endl;
	}

	if (Engine::KeyDown(DIK_DOWN))
	{
		if (m_uiTestAniIndex <= 0)
			m_uiTestAniIndex = m_pMeshCom->Get_AnimationMax() - 1;
		else
			--m_uiTestAniIndex;
		Change_Test();
		cout << "Animation Index : " << m_uiTestAniIndex << endl;
	}*/
}

void CCharacter::Free()
{
	CDynamicObject::Free();
}
