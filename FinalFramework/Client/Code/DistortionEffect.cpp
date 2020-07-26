#include "stdafx.h"
#include "DistortionEffect.h"

#include "CircularTrail.h"
#include "Shader.h"

#include "Export_Function.h"

CDistortionEffect::CDistortionEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CDistortionEffect::~CDistortionEffect()
{
}

HRESULT CDistortionEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);



	return S_OK;
}

_int CDistortionEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_fDeltaAlpha != 0.f)
	{
		m_fAlpha += m_fDeltaAlpha * fTimeDelta;

		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;
		else if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			m_bIsDead = true;
			return 0;
		}
	}

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DISTORTION, this);

	return 0;
}

_int CDistortionEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CDistortionEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(5);

	m_pBufferCom->Set_UV(&m_vStartUV, m_vUVSize.x, m_vUVSize.y);
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}


HRESULT CDistortionEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CDistortionEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	//D3DXMatrixIdentity(&matWorld);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_Alpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}


CDistortionEffect * CDistortionEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	CDistortionEffect* pInst = new CDistortionEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag)))
		::Safe_Release(pInst);

	return pInst;
}

void CDistortionEffect::Free()
{
	CEffect::Free();
}