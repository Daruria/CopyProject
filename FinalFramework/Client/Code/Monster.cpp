#include "stdafx.h"
#include "Monster.h"

#include "SphereCollider.h"
#include "NavigationMesh.h"
#include "Export_Function.h"
#include "Character.h"
#include "Weapon.h"
#include "Skill.h"
#include "DamageFont.h"
#include "DamageEffect.h"
#include "Particle.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CDynamicObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = NULL;

	pCom = m_pNaviCom = DC_ENG(CNavigationMesh, Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pCom);
	
	return S_OK;
}

HRESULT CMonster::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

	//pEffect->SetMatrix("g_matPreWorld", m_pTransformCom->Get_PreWorldMatrix());
	//pEffect->SetMatrix("g_matPreView", Engine::Get_MainCamera()->Get_PreView());

	pEffect->SetFloat("g_fRim", m_fRim);
	pEffect->SetFloat("g_fAlpha", 1.f);


	return S_OK;
}

void CMonster::Change_State()
{
	
}

_int CMonster::Update_State(const _float & fTimeDelta)
{
	return 0;
}

void CMonster::Change_State(_int iState)
{
	m_pMeshCom->Set_AnimationSet(m_vecAnimSetIdx[iState], m_vecAnimItpl[iState]);
	m_eCurAnimType = m_vecAnimSetType[iState];
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vOldPos);
	Attack_Stop();
	m_bEffect = false;
	m_bEffectSound = false;
}

void CMonster::Set_AnimVariable(_int iState, _int iAnimIdx, ANIMATION_TYPE eAnimType, _double dItpl_Time)
{
	m_vecAnimSetIdx[iState] = iAnimIdx;
	m_vecAnimSetType[iState] = eAnimType;
	m_vecAnimItpl[iState] = dItpl_Time;
}

void CMonster::On_NaviMesh()
{
	if (m_pNaviCom->Get_Option() == Engine::CCell::NO_OPTION)
	{
		_float fY = m_pNaviCom->Get_Height(m_pTransformCom->Get_Info(Engine::INFO_POS));
		m_pTransformCom->Set_PosY(fY);
		m_fGravity = 0.f;
	}

}

void CMonster::Gravity(const _float & fTimeDelta)
{
	m_fGravity += fTimeDelta * GRAVITY * 10.f;

	m_vDisplacement.y += -m_fGravity;
}

void CMonster::PlayAnimation(const _float & fTimeDelta)
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
		m_vAnimDiffer = m_vOldPos + vChangedPos - vPos;	
	}

	Update_Link();
}

void CMonster::Decision_Position()
{
	_vec3 vTargetDisplacement =  m_vAnimDiffer + m_vDisplacement;
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	
	m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNavigationMesh(&vPos, &vTargetDisplacement, nullptr));
	

	On_NaviMesh();
	ZeroMemory(&m_vDisplacement, sizeof(_vec3));
	ZeroMemory(&m_vAnimDiffer, sizeof(_vec3));
}

void CMonster::Attack_Stop()
{

}

void CMonster::Update_Link()
{
	for (auto& Link : m_LinkList)
		memcpy(&Link->matLink, Link->pOriginBone, sizeof(_matrix));
}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CMonster* pInst = new CMonster(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);

	return pInst;
}

HRESULT CMonster::Ready_GameObject()
{
	return S_OK;
}

HRESULT CMonster::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CDynamicObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fRim > 0.f)
		m_fRim -= fTimeDelta;
	


	_int iExit = CDynamicObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	if (isnan(vPos.x) || isnan(vPos.y) || isnan(vPos.z))
		m_bIsDead = true;

	return 0;
}

_int CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CDynamicObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CMonster::Render_GameObject(const _float& fTimeDelta)
{
	PlayAnimation(fTimeDelta);
	Decision_Position();

	if (!m_bFirstRender)
	{
		m_bFirstRender = true;
		return;
	}

	LPD3DXEFFECT	pEffect = m_vecShaderCom[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	if (m_bDissolve)
	{
		m_fDissolveTime += fTimeDelta;
		pEffect->SetFloat("g_fTime", m_fDissolveTime);
		pEffect->SetVector("g_vDissolveColor", &_vec4(1.f, 0.25f, 0.75f, 1.f));
		m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

		pEffect->BeginPass(2);
	}
	else
		pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

void CMonster::Render_GameObject(const _float & fTimeDelta, const _uint & iIndex)
{
	_vec3 vChangedPos = { 0.f, 0.f, 0.f };

	m_pMeshCom->Play_Animation(0.f, &vChangedPos);

	CDynamicObject::Render_GameObject(fTimeDelta, iIndex);
}

void CMonster::Collision_Enter(CGameObject * pOpponent)
{
	CCharacter*  pCharacter = dynamic_cast<CCharacter*>(pOpponent);
	if (pCharacter)
	{
		_int iAttack = pCharacter->Get_Attack();
		_vec3 vMonsterPos;
		pCharacter->Get_Transform()->Get_WorldPos(&vMonsterPos);
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

	CSkill* pSkill = dynamic_cast<CSkill*>(pOpponent);

	if (pSkill)
	{
		_int iAttack = pSkill->Get_Attack();
		_vec3 vOppoPos;
		pSkill->Get_Transform()->Get_WorldPos(&vOppoPos);
		Hit(iAttack, &vOppoPos);
		return;
	}
}

void CMonster::Collision_Stay(CGameObject * pOpponent)
{
}

void CMonster::Collision_Leave(CGameObject * pOpponent)
{
}

void CMonster::Hit(_int iAttack, _vec3 * pAtkPos)
{
	m_iHP -= iAttack;



	if (m_iHP <= 0)
	{
		m_iHP = 0;

	}
}

void CMonster::Add_DamageFont(const _int& iAttack)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//_matrix matView, matProj;
	//Engine::Get_MainCamera()->Get_View(&matView);
	//Engine::Get_MainCamera()->Get_Projection(&matProj);

	//D3DXVec3TransformCoord(&vPos, &vPos, &matView);
	//D3DXVec3TransformCoord(&vPos, &vPos, &matProj);

	//vPos.x *= WINCX * 0.5f;
	//vPos.y *= WINCY * 0.5f;

	//CDamageFont* pDmgFont = CDamageFont::Create(m_pGraphicDev, iAttack);
	//NULL_CHECK_RETURN(pDmgFont, );
	//FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_UI", L"UI_DMG", pDmgFont), );
	//pDmgFont->Set_LifeTime(1.f);
	//pDmgFont->Set_Pos(&vPos);
	//pDmgFont->SetUp_ScalingDelta(0.f, 0.25f, 4.f, -16.f);

	CDamageEffect* pDmgFx = CDamageEffect::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_DamageFont", D3DXCOLOR(1.f, 1.f, 1.f, 1.f), iAttack);
	NULL_CHECK_RETURN(pDmgFx, );
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_DMG", pDmgFx), );
	pDmgFx->Set_LifeTime(1.5f);
	pDmgFx->Set_Pos(&vPos);
	pDmgFx->Set_DigitScale(0.01f);
	pDmgFx->SetUp_DigitScaling(0.f, 0.005f, 0.04f, -0.12f);

}

void CMonster::Add_BloodParticle(const _vec3 * pAtkPos)
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


void CMonster::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(pPos));
}

void CMonster::Set_Pos(const _float & fx, const _float & fy, const _float & fz)
{
	m_pTransformCom->Set_Pos(fx, fy, fz);
	
	m_pNaviCom->Set_Index(m_pNaviCom->Get_Index(&_vec3(fx, fy ,fz)));
}

void CMonster::Set_Angle(const _vec3 * pAngle)
{
	m_pTransformCom->Set_Angle(pAngle);
}

void CMonster::Set_Angle(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Angle(fX, fY, fZ);
}

void CMonster::Set_AngleX(const _float & fX)
{
	m_pTransformCom->Set_AngleX(fX);
}

void CMonster::Set_AngleY(const _float & fY)
{
	m_pTransformCom->Set_AngleY(fY);
}

void CMonster::Set_AngleZ(const _float & fZ)
{
	m_pTransformCom->Set_AngleZ(fZ);
}

void CMonster::Set_NaviIndex(const _ulong & dwIndex)
{
	m_pNaviCom->Set_Index(dwIndex);
}

void CMonster::Find_CorrectIndex()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_ulong dwIndex = m_pNaviCom->Get_Index(&vPos);
	m_pNaviCom->Set_Index(dwIndex);
}

void CMonster::Get_Pos(_vec3 * pOut) const
{
	if (pOut)
		m_pTransformCom->Get_Info(Engine::INFO_POS, pOut);
}

void CMonster::Free()
{
	for (auto& pLinker : m_LinkList)
		::Safe_Delete(pLinker);
	CDynamicObject::Free();
}
