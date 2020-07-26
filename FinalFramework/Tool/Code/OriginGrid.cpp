#include "stdafx.h"
#include "OriginGrid.h"

COriginGrid::COriginGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:  CGameObject(pGraphicDev)
{
}

COriginGrid::~COriginGrid()
{
}

HRESULT COriginGrid::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	return S_OK;
}

_int COriginGrid::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	//m_pRenderer->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

_int COriginGrid::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_GameObject(fTimeDelta);

	return iExit;
}

void COriginGrid::Render_GameObject(const _float& fTimeDelta)
{
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	matWorld._41 -= m_uiCntX * m_fItv / 2;
	matWorld._43 -= m_uiCntZ * m_fItv / 2;

	matWorld._11 = m_uiCntX * m_fItv;
	for (_uint i = 0; i <= m_uiCntZ; ++i)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pBufferCom->Render_Buffer();
		matWorld._43 += m_fItv;
	}

	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(-90.f));
	matWorld *= matRotY;

	for (_uint i = 0; i <= m_uiCntX; ++i)
	{
		matWorld._41 += m_fItv;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT COriginGrid::Ready_Component()
{
	CComponent* pComp = nullptr;

	pComp = m_pBufferCom = dynamic_cast<CLineCol*>(Clone(RESOURCE_STATIC, L"Buffer_LineCol"));
	NULL_CHECK_RETURN(pComp, E_FAIL);
	m_pBufferCom->Set_Color(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComp);

	pComp = m_pTransformCom = CTransform::Create();
	NULL_CHECK_RETURN(pComp, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComp);

	pComp = m_pRenderer = Get_Renderer();
	NULL_CHECK_RETURN(pComp, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComp);
	m_pRenderer->AddRef();

	return S_OK;
}


COriginGrid * COriginGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COriginGrid* pInst = new COriginGrid(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		Safe_Release(pInst);

	return pInst;
}

void COriginGrid::Free()
{
	CGameObject::Free();
}
