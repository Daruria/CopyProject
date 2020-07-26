#include "stdafx.h"
#include "SparkEffect.h"

#include "RcTex.h"
#include "Shader.h"

#include "Export_Function.h"

CSparkEffect::CSparkEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CSparkEffect::~CSparkEffect()
{
}

HRESULT CSparkEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _vec2* pStartUV, const _vec2* pUVSize)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwColor), E_FAIL);

	m_pTransformCom->Set_Scale(1.f);	
	m_vStartUV	= *pStartUV;
	m_vUVSize	= *pUVSize;

	return S_OK;
}

_int CSparkEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;


	_vec3 vAngle;
	Engine::Get_MainCamera()->Get_Angle(&vAngle);
	m_pTransformCom->Set_AngleY(vAngle.y);

	if (!m_bXScale)
	{
		m_vScale.y += fTimeDelta * m_fSpeed;
		m_vScale.x += fTimeDelta * m_fSpeed * 2.f;
		if (m_vScale.x > m_fScale)
		{
			m_vScale.x = 1.f;
			m_bXScale = true;
		}
	}
	else
	{
		m_vScale.y += fTimeDelta * m_fSpeed;
		m_vScale.x -= fTimeDelta * m_fSpeed;
		if (m_vScale.x < 0.f)
		{
			m_vScale.x = 0.f;
			m_bIsDead = true;
			return 0;
		}
	}

	m_pTransformCom->Set_Scale(m_vScale);

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CSparkEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CSparkEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(2);

	m_pBufferCom->Set_LocalPos(Engine::CRcTex::RTS_CENTER_BOTTOM);
	m_pBufferCom->Set_UV(&m_vStartUV, m_vUVSize.x, m_vUVSize.y);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

void CSparkEffect::Set_Scale(const _float & fScale)
{
	m_pTransformCom->Set_Scale(fScale);
	m_fScale = fScale;
	m_vScale = { 0.f, 0.f, 1.f };
}

HRESULT CSparkEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
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

HRESULT CSparkEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_Glow", 1.f);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	return S_OK;
}


CSparkEffect * CSparkEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, 
	const _vec2* pStartUV, const _vec2* pUVSize)
{
	CSparkEffect* pInst = new CSparkEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor, pStartUV, pUVSize)))
		::Safe_Release(pInst);

	return pInst;
}

void CSparkEffect::Free()
{
	CEffect::Free();
}