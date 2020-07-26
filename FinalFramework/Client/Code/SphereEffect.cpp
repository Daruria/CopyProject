#include "stdafx.h"
#include "SphereEffect.h"

#include "Export_Function.h"

CSphereEffect::CSphereEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CSphereEffect::~CSphereEffect()
{
}

HRESULT CSphereEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR& dwColor,
	const _ushort& wRenderID, const _uint& iRenderPass)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwColor), E_FAIL);

	m_wRenderID = wRenderID;
	m_iRenderPass = iRenderPass;

	return S_OK;
}

_int CSphereEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_fDeltaDifferScale != 0.f)
	{
		m_fDeltaDifferScale -= fTimeDelta;
		m_vDeltaScale.x += m_fDeltaDifferScale;
		if (m_vDeltaScale.x < 0.f)
			m_vDeltaScale.x = 0.f;

		m_vDeltaScale.y += m_fDeltaDifferScale;
		if (m_vDeltaScale.y < 0.f)
			m_vDeltaScale.y = 0.f;

		m_vDeltaScale.z += m_fDeltaDifferScale;
		if (m_vDeltaScale.z < 0.f)
			m_vDeltaScale.z = 0.f;
	}

	if (m_fDeltaAlpha != 0.f)
	{
		m_fAlpha += m_fDeltaAlpha * fTimeDelta;
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

	m_pRendererCom->Add_RenderGroup((Engine::RENDERID)m_wRenderID, this);

	return 0;
}

_int CSphereEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CEffect::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CSphereEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->CommitChanges();
	 
	pEffect->BeginPass(m_iRenderPass);

	//m_pSphere->DrawSubset(0);
	//m_pBufferCom->Set_UV(&_vec2(0.f, 0.f), 0.5f, 0.5f);
	//m_pBufferCom->Render_Buffer();
	m_pBufferCom->Render_Mesh(pEffect, 0);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

HRESULT CSphereEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR& dwColor)
{
	//FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	//FAILED_CHECK_RETURN(D3DXCreateSphere(m_pGraphicDev, 1.f, 100, 100, &m_pSphere, NULL), E_FAIL);

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();



	//pComponent = m_pNoiseTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, L"Texture_FireNoise"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_NoiseTexture", pComponent);

	//pComponent = m_pMaskTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, L"Texture_FireAlpha"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_AlphaTexture", pComponent);



	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	D3DLOCKED_RECT	tLockRect;
	ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &tLockRect, NULL, NULL);

	*((_ulong*)tLockRect.pBits) = dwColor;

	pTexture->UnlockRect(0);

	pComponent = m_pColorTextureCom = Engine::CTexture::Create(m_pGraphicDev, &pTexture, Engine::TEX_NORMAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Color", pComponent);
	::Safe_Release(pTexture);

	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Fx_Sphere"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CSphereEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_Alpha", m_fAlpha);
	//pEffect->SetFloat("g_Glow", 1.f);

	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	m_pTextureCom->Set_Texture(pEffect,	"g_MaskTexture");

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");


	return S_OK;
}

CSphereEffect * CSphereEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrTextureTag, const D3DXCOLOR& dwColor)
{
	CSphereEffect* pInstance = new CSphereEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor)))
		::Safe_Release(pInstance);

	return pInstance;
}

CSphereEffect * CSphereEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrTextureTag, const D3DXCOLOR & dwColor, const _ushort & wRenderID, const _uint & iRenderPass)
{
	CSphereEffect* pInstance = new CSphereEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor, wRenderID, iRenderPass)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CSphereEffect::Free()
{
	CEffect::Free();
}