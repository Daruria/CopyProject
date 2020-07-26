#include "stdafx.h"
#include "Item.h"


CItem::CItem():
	m_iPrice(0),
	m_fRatio(1.f),
	m_strIconKey(""),
	m_strName(""),
	m_eType(IT_END)
{
}


CItem::~CItem()
{
}

void CItem::SetIconImage(const string & strKey, const wchar_t * pPath)
{
	m_strIconKey = strKey;
	INSERT_BITMAP(strKey, pPath);
}

void CItem::IconRender(HDC hDC, float fx, float fy)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strIconKey);
	SIZE tSize = GET_BITMAP_SIZE(m_strIconKey);

	GdiTransparentBlt(hDC, 
		static_cast<int>(fx - tSize.cx * m_fRatio / 2), 
		static_cast<int>(fy - tSize.cx  * m_fRatio / 2), 
		static_cast<int>(tSize.cx * m_fRatio), 
		static_cast<int>(tSize.cy * m_fRatio), hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
}

void CItem::DiscRender(HDC hDC, float fx, float fy)
{

}
