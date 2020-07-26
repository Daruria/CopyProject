#include "stdafx.h"
#include "MyButton.h"

CMyButton::CMyButton():
	m_fRatio(1.f)
{
}

CMyButton::~CMyButton()
{
	Release();
}

const BOOL CMyButton::IsOnMouse()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc = 
	{ 
		(int)(m_tPoint.x - m_tSize.cx * m_fRatio / 2), 
		(int)(m_tPoint.y - m_tSize.cy * m_fRatio / 2),
		(int)(m_tPoint.x + m_tSize.cx * m_fRatio / 2), 
		(int)(m_tPoint.y + m_tSize.cy * m_fRatio / 2)
	};

	return PtInRect(&rc, pt);
}

void CMyButton::Initialize(const string& strKey, const wchar_t* pPath, const string& strOnKey, const wchar_t* pOnPath)
{
	CUI::Initialize(strKey, pPath);

	m_bIsOnMouse = false;

	m_strOnKey = strOnKey;
	INSERT_BITMAP(m_strOnKey, pOnPath);
	m_tOnSize = GET_BITMAP_SIZE(m_strOnKey);
}

int CMyButton::Update()
{
	m_bIsOnMouse = IsOnMouse() ? true : false;
	if (m_bIsOnMouse && KEYDOWN(VK_LBUTTON))
		return 1;

	return 0;
}

void CMyButton::LateUpdate()
{
}

void CMyButton::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(!m_bIsOnMouse ? m_strKey : m_strOnKey);
	SIZE tSize = !m_bIsOnMouse ? m_tSize : m_tOnSize;

	int iRenderStartX = static_cast<int>(m_tPoint.x - tSize.cx * m_fRatio / 2);
	int iRenderStartY = static_cast<int>(m_tPoint.y - tSize.cy * m_fRatio / 2);

	if (hMemDC)
		GdiTransparentBlt(hDC, iRenderStartX, iRenderStartY, 
			static_cast<int>(tSize.cx * m_fRatio), 
			static_cast<int>(tSize.cy * m_fRatio), hMemDC, 0, 0, tSize.cx, tSize.cy, TRANS_VALUE);
	else
		CAN_NOT_FOUND_BITMAP(hDC, iRenderStartX, iRenderStartY, tSize.cx, tSize.cy);
}

void CMyButton::Release()
{
}
