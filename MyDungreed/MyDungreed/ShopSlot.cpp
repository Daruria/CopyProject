#include "stdafx.h"
#include "ShopSlot.h"
#include "Item.h"

CShopSlot::CShopSlot()
{
}


CShopSlot::~CShopSlot()
{
	Release();
}

int CShopSlot::Update()
{
	return CUISlot::Update();
}

void CShopSlot::LateUpdate()
{
	CUISlot::LateUpdate();
}

void CShopSlot::Render(HDC hDC)
{
	CMyButton::Render(hDC);
	if (m_pItem && m_bIconRender)
	{
		m_pItem->IconRender(hDC, static_cast<float>(m_tPoint.x - m_fRatio * 43), static_cast<float>(m_tPoint.y));

		GET_SINGLE(CFontManager)->Render(hDC, m_tPoint.x - m_fRatio * 30, m_tPoint.y - 10 * m_fRatio, FT_NORMAL, 3.f, m_pItem->GetName());
		int iPrice = m_pItem->GetPrice();
		GET_SINGLE(CFontManager)->Render(hDC, m_tPoint.x + 30 * m_fRatio, m_tPoint.y + 1 * m_fRatio, FT_NORMAL, 2.f, to_string(iPrice));
	}
}

void CShopSlot::Release()
{
}
