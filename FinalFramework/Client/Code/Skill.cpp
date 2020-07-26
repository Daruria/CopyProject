#include "stdafx.h"
#include "Skill.h"

#include "Transform.h"
#include "Shader.h"
#include "Collider.h"
#include "Renderer.h"

#include "Export_Function.h"
#include "Particle.h"

CSkill::CSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSkill::~CSkill()
{
}

HRESULT CSkill::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CSkill::Update_GameObject(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CSkill::LateUpdate_GameObject(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CSkill::Render_GameObject(const _float & fTimeDelta)
{
	
}

void CSkill::Collision_Enter(CGameObject* pOpponent)
{
	_vec3 vPos, vOppoPos;
	m_pTransformCom->Get_WorldPos(&vPos);

	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(pOpponent->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
	NULL_CHECK_RETURN(pTransform, );
	pTransform->Get_WorldPos(&vOppoPos);

	vPos = (vPos + vOppoPos) * 0.5f;

	CParticle* pEffect = CParticle::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_EffectDot", 50);
	NULL_CHECK(pEffect);
	Engine::Add_GameObject(L"Layer_GameLogic", L"Obj_Particle", pEffect);
	pEffect->Set_Scale(0.1f);
	pEffect->Set_Speed(30.f);
	pEffect->Set_Pos(&vPos);
}

void CSkill::Collision_Stay(CGameObject* pOpponent)
{

}

void CSkill::Collision_Leave(CGameObject* pOpponent)
{

}

HRESULT CSkill::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	

	return S_OK;
}

HRESULT CSkill::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	return S_OK;
}

void CSkill::Free()
{
	CGameObject::Free();
}
