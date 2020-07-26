#include "stdafx.h"
#include "Mouse.h"

#include "Export_Function.h"

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CMouse::~CMouse()
{
}

HRESULT CMouse::Ready_GameObject()
{
	Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_Cursor", Engine::TEX_NORMAL, 
						L"../Bin/Resources/Texture/UI/UI_Arrow.tga");

	Engine::CComponent* pCom = Engine::Get_Resources(Engine::RESOURCE_STATIC, L"Buffer_RcColTex");
	if (!pCom)
		FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", Engine::BUFFER_RCCOLTEX), E_FAIL);

	FAILED_CHECK_RETURN(CUI::Add_Component(Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_Cursor"), E_FAIL);

	m_wstrName = L"UI_Mouse";

	const Engine::TEX_INFO* pTexInfo = m_pTextureCom->Get_TexInfo();
	m_pTransformCom->Set_Scale(pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f);

	m_uiRcPos = Engine::CRcTex::RTS_CENTER_CENTER;
	m_vUV = { 1.f, 1.f };

	::ShowCursor(FALSE);

	m_bCursorRender = true;

	return S_OK;
}

_int CMouse::Update_GameObject(const _float& fTimeDelta)
{
	_vec2 vMousePos;
	Engine::CMyMath::ClientMousePos(g_hWnd, &vMousePos);
	vMousePos.x -= WINCX * 0.5f;
	vMousePos.y = -vMousePos.y + WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vMousePos.x, vMousePos.y, 0.f);

	_int iExit = CUI::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

_int CMouse::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CUI::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CMouse::Render_GameObject(const _float& fTimeDelta)
{
	if (m_bCursorRender)
		CUI::Render_GameObject(fTimeDelta);
}

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouse* pInst = new CMouse(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject()))
		::Safe_Release(pInst);

	return pInst;
}

void CMouse::Free()
{
	::ShowCursor(TRUE);
	CUI::Free();
}
