#include "stdafx.h"
#include "FireEffect.h"

#include "Texture.h"

#include "Export_Function.h"

CFireEffect::CFireEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CFireEffect::~CFireEffect()
{
}

HRESULT CFireEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrAlphaTextureTag)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, wstrNoiseTextureTag, wstrAlphaTextureTag), E_FAIL);


	return S_OK;
}

_int CFireEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;


	if (m_fDeltaAlpha != 0.f)
	{
		m_fAlpha += fTimeDelta * m_fDeltaAlpha;
		if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			m_bIsDead = true;

			return 0;
		}
		else if (m_fAlpha > 1.f)
		{
			m_fAlpha = 1.f;
		}
	}


	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CFireEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFireEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Set_UV(&m_vUVStart, m_vUVSize.x, m_vUVSize.y);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

HRESULT CFireEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrAlphaTextureTag)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Fire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();

	pComponent = m_pNoiseTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrNoiseTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_NoiseTexture", pComponent);

	pComponent = m_pMaskTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrAlphaTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_AlphaTexture", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CFireEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	pEffect->SetFloat("g_fTime", m_fLifeCount);
	pEffect->SetFloatArray("g_vScrollSpeeds", &m_vScrollSpeed.x, 3);
	pEffect->SetFloatArray("g_vScales", &m_vScales.x, 3);

	pEffect->SetFloatArray("g_vDistortion1", &m_vDistortion1.x, 2);
	pEffect->SetFloatArray("g_vDistortion2", &m_vDistortion2.x, 2);
	pEffect->SetFloatArray("g_vDistortion3", &m_vDistortion3.x, 2);
	pEffect->SetFloat("fDistortionScale", m_fDistortionScale);
	pEffect->SetFloat("fDistortionBias", m_fDistortionBias);
	
	pEffect->SetFloat("g_Alpha", m_fAlpha);

	return S_OK;
}

CFireEffect * CFireEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
									const wstring& wstrNoiseTextureTag, const wstring& wstrAlphaTextureTag)
{
	CFireEffect* pInst = new CFireEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, wstrNoiseTextureTag, wstrAlphaTextureTag)))
		::Safe_Release(pInst);

	return pInst;
}

void CFireEffect::Free()
{
	CEffect::Free();
}