#include "stdafx.h"
#include "CircularEffect.h"

#include "CircularTrail.h"
#include "Shader.h"

#include "Export_Function.h"

CCircularEffect::CCircularEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CCircularEffect::~CCircularEffect()
{
}

HRESULT CCircularEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwColor), E_FAIL);

	m_pTransformCom->Set_Scale(10.f);	

	return S_OK;
}

_int CCircularEffect::Update_GameObject(const _float& fTimeDelta)
{
	//_tchar szTest[64] = L"";
	//GetPrivateProfileString(L"Effect", L"Speed", nullptr, szTest, 64, L"../../Test.ini");
	//_float fSpeed = _wtof(szTest);

	if (m_bIsDead)
		return 0;





	m_pBufferCom->Change_UV(fTimeDelta, m_fSpeed);
	

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CCircularEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CCircularEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

void CCircularEffect::Set_Angle(const _float & fRadian)
{
	m_pBufferCom->Set_Angle(fRadian);
}

HRESULT CCircularEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCircularTrail*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Buffer_CircularTrail"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);
	m_pBufferCom->Set_Angle(D3DX_PI);

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

HRESULT CCircularEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	return S_OK;
}


CCircularEffect * CCircularEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	CCircularEffect* pInst = new CCircularEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor)))
		::Safe_Release(pInst);

	return pInst;
}

void CCircularEffect::Free()
{
	CEffect::Free();
}