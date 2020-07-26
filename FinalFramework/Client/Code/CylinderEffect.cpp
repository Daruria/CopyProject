#include "stdafx.h"
#include "CylinderEffect.h"

#include "CylinderTex.h"
#include "Shader.h"

#include "Export_Function.h"

CCylinderEffect::CCylinderEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CCylinderEffect::~CCylinderEffect()
{
}

HRESULT CCylinderEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag,
	const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight,
	const _ushort& wRenderID, const _uint& iRenderPass)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwSlices, dwStacks, fUpperRadius, fLowerRadius, fHeight), E_FAIL);

	m_wRenderID = wRenderID;
	m_iRenderPass = iRenderPass;

	//m_pTransformCom->Set_Scale(10.f);	

	return S_OK;
}

_int CCylinderEffect::Update_GameObject(const _float& fTimeDelta)
{
	//_tchar szTest[64] = L"";
	//GetPrivateProfileString(L"Effect", L"Speed", nullptr, szTest, 64, L"../../Test.ini");
	//_float fSpeed = _wtof(szTest);
	if (m_bIsDead)
		return 0;
	
	if (m_fDeltaAlpha != 0.f)
		m_fAlpha += m_fDeltaAlpha * fTimeDelta;

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup((Engine::RENDERID)m_wRenderID, this);

	return 0;
}

_int CCylinderEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CCylinderEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(m_iRenderPass);

	m_pBufferCom->Render_Buffer(fTimeDelta, m_fSpeed, &m_vDirUV);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

HRESULT CCylinderEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag,
	const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = Engine::CCylinderTex::Create(m_pGraphicDev, dwSlices, dwStacks, fUpperRadius, fLowerRadius, fHeight);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	D3DLOCKED_RECT	tLockRect;
	ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &tLockRect, NULL, NULL);

	*((_ulong*)tLockRect.pBits) = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pTexture->UnlockRect(0);

	pComponent = m_pColorTextureCom = Engine::CTexture::Create(m_pGraphicDev, &pTexture, Engine::TEX_NORMAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Color", pComponent);
	::Safe_Release(pTexture);

	return S_OK;
}

HRESULT CCylinderEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	//D3DXMatrixIdentity(&matWorld);
	//matWorld._42 += 5.f;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_Alpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	

	return S_OK;
}


CCylinderEffect * CCylinderEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag,
	const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight)
{
	CCylinderEffect* pInst = new CCylinderEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwSlices, dwStacks, fUpperRadius, fLowerRadius, fHeight)))
		::Safe_Release(pInst);

	return pInst;
}

CCylinderEffect * CCylinderEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrTextureTag, const _ulong & dwSlices, const _ulong & dwStacks, const _float & fUpperRadius, const _float & fLowerRadius, const _float & fHeight, const _ushort & wRenderID, const _uint & iRenderPass)
{
	CCylinderEffect* pInst = new CCylinderEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwSlices, dwStacks, fUpperRadius, fLowerRadius, fHeight, wRenderID, iRenderPass)))
		::Safe_Release(pInst);

	return pInst;
}

void CCylinderEffect::Free()
{
	CEffect::Free();
}