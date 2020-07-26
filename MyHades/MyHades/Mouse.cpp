#include "stdafx.h"
#include "Mouse.h"

IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse():
	m_wstrMouseKey(L""),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrMouseKey);
	NULL_CHECK(pTexInfo);

	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	D3DXVECTOR3 vMousePos = { static_cast<float>(pt.x), static_cast<float>(pt.y),0.f };
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, vMousePos.x, vMousePos.y, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

D3DXVECTOR3 CMouse::GetMousePos()
{
	// TODO: insert return statement here
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3(static_cast<FLOAT>(pt.x), static_cast<FLOAT>(pt.y), 0.f);
}

D3DXVECTOR3 CMouse::GetWorldMousePos()
{
	// TODO: insert return statement here

	return D3DXVECTOR3();
}

HRESULT CMouse::Initalize()
{
	ShowCursor(FALSE);

	m_wstrMouseKey = L"CursorDefault";

	return S_OK;
}

void CMouse::Release()
{
	ShowCursor(TRUE);
}
