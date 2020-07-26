#include "stdafx.h"
#include "UI.h"

#include "Export_Function.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Add_Component(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrTextureTag)
{
	Engine::CComponent*		pCom = nullptr;

	pCom = m_pBufferCom = dynamic_cast<Engine::CRcAlphaTex*>(Engine::Clone(wContainerIdx, wstrBufferTag));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pCom);

	pCom = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pCom);

	pCom = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pCom);

	pCom = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pCom);
	pCom->AddRef();


	return S_OK;
}

HRESULT CUI::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrTextureTag, const wstring & wstrName, const D3DXCOLOR& dwColor)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag, wstrTextureTag), E_FAIL);

	m_wstrName = wstrName;

	m_uiRcPos	= Engine::CRcTex::RTS_CENTER_CENTER;
	m_vUV		= { 1.f, 1.f };

	m_dwColor = dwColor;

	return S_OK;
}


_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CUI::Render_GameObject(const _float& fTimeDelta)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture();

	m_pBufferCom->Set_Color(m_dwColor);
	m_pBufferCom->Set_LocalPos((Engine::CRcAlphaTex::RC_TEX_POS)m_uiRcPos);
	m_pBufferCom->Set_UV((Engine::CRcAlphaTex::RC_TEX_POS)m_uiRcPos, &m_vUV);

	m_pBufferCom->Render_Buffer();
}

void CUI::Set_Pos(const _float & fX, const _float & fY)
{
	m_pTransformCom->Set_PosX(fX);
	m_pTransformCom->Set_PosY(fY);
}

void CUI::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CUI::Set_Pos(const _vec2 * pPos)
{
	m_pTransformCom->Set_PosX(pPos->x);
	m_pTransformCom->Set_PosY(pPos->y);
}

void CUI::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CUI::Set_PosX(const _float & fX)
{
	m_pTransformCom->Set_PosX(fX);
}

void CUI::Set_PosY(const _float & fY)
{
	m_pTransformCom->Set_PosY(fY);
}

void CUI::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

void CUI::Set_Scale(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Scale(fX, fY, fZ);
}

void CUI::Set_Scale(const _float & fX, const _float & fY)
{
	m_pTransformCom->Set_ScaleX(fX);
	m_pTransformCom->Set_ScaleY(fY);
}

void CUI::Set_Scale(const _vec3 * pScale)
{
	m_pTransformCom->Set_Scale(pScale);
}

void CUI::Set_ScaleX(const _float & fX)
{
	m_pTransformCom->Set_ScaleX(fX);
}

void CUI::Set_ScaleY(const _float & fY)
{
	m_pTransformCom->Set_ScaleY(fY);
}

void CUI::Set_ScaleZ(const _float & fZ)
{
	m_pTransformCom->Set_ScaleZ(fZ);
}

void CUI::Set_RcPos(const _uint & uiRcPos)
{
	m_uiRcPos = uiRcPos;
}

void CUI::Set_UV(const _vec2 * pUV)
{
	memcpy(&m_vUV, pUV, sizeof(_vec2));
}

void CUI::Set_UV(const _float & fU, const _float & fV)
{
	m_vUV.x = fU;
	m_vUV.y = fV;
}

void CUI::Set_U(const _float & fU)
{
	m_vUV.x = fU;
}

void CUI::Set_V(const _float & fV)
{
	m_vUV.y = fV;
}

void CUI::Set_Scale(const _vec2 * pScale)
{
	m_pTransformCom->Set_ScaleX(pScale->x);
	m_pTransformCom->Set_ScaleY(pScale->y);
}

_float CUI::Get_OriginWidth(_uint iIndex) const
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(iIndex);

	return _float(pTexInfo->tImgInfo.Width);
}

_float CUI::Get_OriginHeight(_uint iIndex) const
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(iIndex);

	return _float(pTexInfo->tImgInfo.Height);
}

void CUI::Get_OriginSize(_vec2 * pOut, _uint iIndex) const
{
	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo(iIndex);

	pOut->x = (_float)pTexInfo->tImgInfo.Width;
	pOut->y = (_float)pTexInfo->tImgInfo.Height;
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrTextureTag, const wstring& wstrName, const D3DXCOLOR& dwColor)
{
	CUI* pInst = new CUI(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrTextureTag, wstrName, dwColor)))
		::Safe_Release(pInst);


	return pInst;
}

void CUI::Free()
{
	Engine::CGameObject::Free();
}
