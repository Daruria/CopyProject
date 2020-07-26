#include "stdafx.h"
#include "TrailEffect.h"

#include "TrailBuffer.h"
#include "Shader.h"
#include "MoveOnRoute.h"

#include "Export_Function.h"

CTrailEffect::CTrailEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTrailEffect::~CTrailEffect()
{
}

HRESULT CTrailEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _ushort& wRenderID, const _uint& iRenderPass)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwColor), E_FAIL);

	m_pTransformCom->Set_Scale(1.f);	

	m_wRenderID = wRenderID;
	m_iRenderPass = iRenderPass;

	return S_OK;
}

_int CTrailEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	_vec3 vPos, vUp;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(m_eAxis, &vUp);

	D3DXVec3Normalize(&vUp, &vUp);

	Add_Vertex(&(vPos + vUp * m_fUpLength), &(vPos + vUp * -m_fDownLength));

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup((Engine::RENDERID)m_wRenderID, this);

	return 0;
}

_int CTrailEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CTrailEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(m_iRenderPass);

	m_pBufferCom->Render_Buffer(&m_TrailList, m_iInterpoleCnt);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}

void CTrailEffect::Add_Vertex(const _vec3 * pTopPos, const _vec3 * pBottomPos)
{
	//if (m_fItvlTime + m_fInterval < m_fLifeCount)
	{
		//m_fItvlTime = m_fLifeCount;

		m_TrailList.push_back(make_pair(*pTopPos, *pBottomPos));

		if (m_TrailList.size() > 30)
			m_TrailList.pop_front();
	}
	//else
	//{
	//	if (!m_TrailList.empty())
	//	{
	//		m_TrailList.back().first = *pTopPos;
	//		m_TrailList.back().second = *pBottomPos;
	//	}
	//}
	

}

void CTrailEffect::Clear_Vertex()
{
	m_TrailList.clear();

	m_pBufferCom->Clear_Vertex();
}

HRESULT CTrailEffect::Add_MoveOnRoute(Engine::CMoveOnRoute * pRouteOnMove)
{
	NULL_CHECK_RETURN(pRouteOnMove, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_MoveOnRoute", pRouteOnMove);
	

	return S_OK;
}

HRESULT CTrailEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTrailBuffer*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Buffer_Trail"));
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

HRESULT CTrailEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);
	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_Alpha", m_fAlpha);
	pEffect->SetFloat("g_Glow", 0.f);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	return S_OK;
}


CTrailEffect * CTrailEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor,
	_ushort wRenderID, _uint iRenderPass)
{
	CTrailEffect* pInst = new CTrailEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor, wRenderID, iRenderPass)))
		::Safe_Release(pInst);

	return pInst;
}

void CTrailEffect::Free()
{
	CEffect::Free();
}