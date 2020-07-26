#include "stdafx.h"
#include "UI.h"


CUI::CUI()
{
}


CUI::~CUI()
{
	Release();
}

void CUI::Initialize(const string& strKey, const wchar_t* pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(m_strKey, pPath);
	m_tSize = GET_BITMAP_SIZE(m_strKey);
}

int CUI::Update()
{
	return 0;
}

void CUI::LateUpdate()
{
}

void CUI::Render(HDC hDC)
{
	HDC hMemDC = GET_SINGLE(CBitmapManager)->FindBitmapDC(m_strKey);
	
	int iRenderStartX = m_tPoint.x - m_tSize.cx / 2;
	int iRenderStartY = m_tPoint.y - m_tSize.cy / 2;

	if (hMemDC)
		GdiTransparentBlt(hDC, iRenderStartX, iRenderStartY, m_tSize.cx, m_tSize.cy, hMemDC, 0, 0, m_tSize.cx, m_tSize.cy, TRANS_VALUE);
	else
		CAN_NOT_FOUND_BITMAP(hDC, iRenderStartX, iRenderStartY, m_tSize.cx, m_tSize.cy);

}

void CUI::Release()
{
}
