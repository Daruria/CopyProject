#include "stdafx.h"
#include "UndeadPortal.h"

#include "Texture.h"
#include "TrailBuffer.h"
#include "Shader.h"
#include "NavigationMesh.h"

#include "Export_Function.h"
#include "CircleTex.h"
#include "Monster.h"

CUndeadPortal::CUndeadPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUndeadPortal::~CUndeadPortal()
{
}

HRESULT CUndeadPortal::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, 
	const wstring& wstrMaskTextureTag, const _float& fMaxScale, _vec4 vColor)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, wstrNoiseTextureTag, wstrMaskTextureTag), E_FAIL);

	m_pTransformCom->Set_Scale(0.f);

	m_fSizing_Speed = 10.f;
	m_fMaxScale = fMaxScale;

	m_vColor = vColor;

	return S_OK;
}

_int CUndeadPortal::Update_GameObject(const _float& fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = Update_State(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//m_pBufferCom->Move_UV_Up(fTimeDelta, 1.f);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CUndeadPortal::LateUpdate_GameObject(const _float& fTimeDelta)
{


	return 0;
}

void CUndeadPortal::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}


_int CUndeadPortal::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;
	switch (m_eCurState)
	{
	case CUndeadPortal::EUndeadPortal::OPENING:
		iExit = Update_Opening(fTimeDelta);
		break;
	case CUndeadPortal::EUndeadPortal::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CUndeadPortal::EUndeadPortal::CLOSING:
		iExit = Update_Closing(fTimeDelta);
		break;
	}

	return iExit;
}

_int CUndeadPortal::Update_Opening(const _float & fTimeDelta)
{
	m_fCurScale += fTimeDelta * m_fSizing_Speed;

	if (m_fCurScale >= m_fMaxScale)
	{
		m_fCurScale = m_fMaxScale;
		m_eCurState = EUndeadPortal::SPAWN;
	}

	m_pTransformCom->Set_Scale(m_fCurScale);

	return 0;
}

_int CUndeadPortal::Update_Spawn(const _float & fTimeDelta)
{
	return 0;
}

_int CUndeadPortal::Update_Closing(const _float & fTimeDelta)
{
	m_fCurScale -= fTimeDelta * m_fSizing_Speed;

	if (m_fCurScale <= 0.f)
	{
		m_fCurScale = 0.f;
		m_bIsDead = true;
	}

	m_pTransformCom->Set_Scale(m_fCurScale);

	return 0;
}

void CUndeadPortal::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CUndeadPortal::Set_Pos(const _vec3 & rPos)
{
	m_pTransformCom->Set_Pos(rPos);
}

void CUndeadPortal::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CUndeadPortal::Set_PosX(const _float & fX)
{
	m_pTransformCom->Set_PosX(fX);
}

void CUndeadPortal::Set_PosY(const _float & fY)
{
	m_pTransformCom->Set_PosY(fY);
}

void CUndeadPortal::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

void CUndeadPortal::Set_Angle(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Angle(fX, fY, fZ);
}

void CUndeadPortal::Set_State(EUndeadPortal eState)
{
	m_eCurState = eState;
}

HRESULT CUndeadPortal::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pNoiseTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrNoiseTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_NoiseTexture", pComponent);

	pComponent = m_pMaskTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrMaskTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MaskTexture", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCircleTex*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Buffer_CircleTex"));
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Portal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();
	
	return S_OK;
}

HRESULT CUndeadPortal::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	m_pNoiseTextureCom->Set_Texture(pEffect, "g_NoiseTexture");
	m_pMaskTextureCom->Set_Texture(pEffect, "g_MaskTexture");

	pEffect->SetFloat("g_Alpha", 1.f);
	pEffect->SetFloat("g_Glow", 1.f);
	pEffect->SetFloat("g_fTime", m_fLifeCount);

	_vec3 vScrollSpeed	= { 1.f, 0.5f, 0.25f };
	_vec3 vScales		= { 1.f, 3.f, 2.f };

	_vec2 vDistortion1 = { 1.f, 1.f };
	_vec2 vDistortion2 = { 2.f, 2.f };
	_vec2 vDistortion3 = { 3.f, 3.f };

	pEffect->SetFloatArray("g_vScrollSpeeds", &vScrollSpeed.x, 3);
	pEffect->SetFloatArray("g_vScales", &vScales.x, 3);

	pEffect->SetFloatArray("g_vDistortion1", &vDistortion1.x, 2);
	pEffect->SetFloatArray("g_vDistortion2", &vDistortion2.x, 2);
	pEffect->SetFloatArray("g_vDistortion3", &vDistortion3.x, 2);
	pEffect->SetFloat("fDistortionScale", 1.f);
	pEffect->SetFloat("fDistortionBias", 0.4f);
	pEffect->SetVector("g_vColor", &m_vColor);

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}

CUndeadPortal * CUndeadPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
	const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag, const _float& fMaxScale, _vec4 vColor)
{
	CUndeadPortal* pInst = new CUndeadPortal(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, wstrNoiseTextureTag, wstrMaskTextureTag, fMaxScale, vColor)))
		::Safe_Release(pInst);

	return pInst;
}

void CUndeadPortal::Free()
{
	CGameObject::Free();
}