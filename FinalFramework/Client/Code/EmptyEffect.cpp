#include "stdafx.h"
#include "EmptyEffect.h"

#include "Shader.h"
#include "MoveOnRoute.h"

#include "Export_Function.h"

CEmptyEffect::CEmptyEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEmptyEffect::~CEmptyEffect()
{
}

HRESULT CEmptyEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwColor), E_FAIL);



	return S_OK;
}

_int CEmptyEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_pRouteCom->IsDead())
	{
		m_bIsDead = true;
		return 0;
	}

	if (m_fDeltaAlpha != 0.f)
	{
		m_fAlpha += m_fDeltaAlpha * fTimeDelta;

		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;
		else if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			return 0;
		}
	}

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	m_fLifeCount = 0.f;

	if (iExit & 0x80000000)
		return -1;

	if (m_pRouteCom->IsDead())
	{
		m_bIsDead = true;
		return 0;
	}

	m_pRendererCom->Add_RenderGroup((Engine::RENDERID)m_wRenderID, this);

	return 0;
}

_int CEmptyEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CEmptyEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(m_iRenderPass);

	m_pBufferCom->Set_UV(&m_vStartUV, m_vUVSize.x, m_vUVSize.y);
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}


void CEmptyEffect::Set_RenderID_Pass(const _ushort & wRenderID, const _int & iRenderPass)
{
	m_wRenderID = wRenderID;
	m_iRenderPass = iRenderPass;
}

HRESULT CEmptyEffect::Add_MoveOnRoute(Engine::CMoveOnRoute * pRouteOnMove)
{
	m_pRouteCom = pRouteOnMove;
	NULL_CHECK_RETURN(pRouteOnMove, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_MoveOnRoute", pRouteOnMove);

	return S_OK;
}

HRESULT CEmptyEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

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

	return S_OK;
}

HRESULT CEmptyEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_Glow", m_fGlow);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}


CEmptyEffect * CEmptyEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	CEmptyEffect* pInst = new CEmptyEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor)))
		::Safe_Release(pInst);

	return pInst;
}

void CEmptyEffect::Free()
{
	CEffect::Free();
}