#include "stdafx.h"
#include "Mouse.h"
#include "Item.h"

DEFINITION_SINGLE(CMouse);

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	INSERT_BITMAP("ShootingCursor", L"Image/Cursor/ShootingCursor2.bmp");
	INSERT_BITMAP("BasicCurosr", L"Image/Cursor/BasicCursor.bmp");
	SIZE tSize = GET_BITMAP_SIZE("BasicCurosr");
	m_tSize.cx = tSize.cx;
	m_tSize.cy = tSize.cy;

	m_iCursor = 0;
	m_strKey = "BasicCurosr";

	m_pItem = nullptr;

	ShowCursor(false);
}

int CMouse::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	m_tPoint.x = pt.x;
	m_tPoint.y = pt.y;


	return 0;
}

void CMouse::LateUpdate()
{
}

void CMouse::Render(HDC hDC)
{
	if (m_pItem)
		m_pItem->IconRender(hDC, static_cast<float>(m_tPoint.x), static_cast<float>(m_tPoint.y));

	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	if (hMemDC)
		GdiTransparentBlt(hDC, 
			m_tPoint.x - m_tSize.cx * 3 / 2, 
			m_tPoint.y - m_tSize.cy * 3 / 2, 
			m_tSize.cx * 3, 
			m_tSize.cy * 3,
			hMemDC, 0, 0, m_tSize.cx, m_tSize.cy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, m_tPoint.x - m_tSize.cx / 2, m_tPoint.y - m_tSize.cy / 2, m_tSize.cx, m_tSize.cy);
	

}

void CMouse::Release()
{
	ShowCursor(true);
}

void CMouse::SetCursor(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(strKey, pPath);
	SIZE tSize = GET_BITMAP_SIZE(strKey);
	m_tSize.cx = tSize.cx;
	m_tSize.cy = tSize.cy;
}

void CMouse::ChangeCursor(int iCursor)
{
	if (m_iCursor != iCursor)
	{
		m_iCursor = iCursor;
		switch (m_iCursor)
		{
		case 0:
			m_strKey = "BasicCurosr";
			m_tSize = GET_BITMAP_SIZE(m_strKey);
			break;
		case 1:
			m_strKey = "ShootingCursor";
			m_tSize = GET_BITMAP_SIZE(m_strKey);
			break;
		}
	}
	
}
