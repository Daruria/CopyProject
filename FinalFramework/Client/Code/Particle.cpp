#include "stdafx.h"
#include "Particle.h"

#include "ParticleTex.h"
#include "Shader.h"

#include "Export_Function.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwVtxCnt), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f);

	return S_OK;
}

_int CParticle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fAccTime += fTimeDelta;
	if (m_fAccTime > 2.f)
	{
		m_fAccTime = 0.f;
		m_bIsDead = true;

		return 0;
	}

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CParticle::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CParticle::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer(fTimeDelta);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

void CParticle::Set_Speed(const _float & fSpeed)
{
	m_pBufferCom->Set_Speed(fSpeed);
}

HRESULT CParticle::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = Engine::CParticleTex::Create(m_pGraphicDev, dwVtxCnt);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	D3DLOCKED_RECT	tLockRect;
	ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &tLockRect, NULL, NULL);

	*((_ulong*)tLockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pTexture->UnlockRect(0);

	pComponent = m_pColorTextureCom = Engine::CTexture::Create(m_pGraphicDev, &pTexture, Engine::TEX_NORMAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Color", pComponent);

	::Safe_Release(pTexture);

	return S_OK;
}

HRESULT CParticle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_WINCY", (_float)WINCY);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}


CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const _ulong& dwVtxCnt)
{
	CParticle* pInst = new CParticle(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwVtxCnt)))
		::Safe_Release(pInst);

	return pInst;
}

void CParticle::Free()
{
	CEffect::Free();
}