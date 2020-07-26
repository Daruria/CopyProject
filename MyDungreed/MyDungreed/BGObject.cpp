#include "stdafx.h"
#include "BGObject.h"
#include "ScrollManager.h"

//	m_tInfo가 가지고 있는 값은 스크롤XY 를 기준으로 상대위치를 가지고 있다.


CBGObject::CBGObject()
	:
	m_strKey(""),
	m_iInterval(0)
{
}


CBGObject::~CBGObject()
{
	Release();
}

void CBGObject::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(m_strKey, pPath);
	SIZE tSize = GET_BITMAP_SIZE(m_strKey);
	m_tInfo.icx = tSize.cx;
	m_tInfo.icy = tSize.cy;
}

const POSITION CBGObject::GetScreenFromScroll()
{
	return POSITION{ m_tInfo.fx + WINCX / 2, m_tInfo.fy + WINCY / 2};
}

void CBGObject::Initialize()
{
	
}

int CBGObject::Update()
{
	return 0;
}

void CBGObject::LateUpdate()
{

}

void CBGObject::Render(HDC hDC)
{
	HDC	hMemDC = FIND_BITMAP_DC(m_strKey);
	INFO tInfo = m_tInfo;
	float fScrollX = CScrollManager::GetScrollX();
	float fScrollY = CScrollManager::GetScrollY();
	
	POSITION tScreenPos = CScrollManager::WorldToScreen(tInfo);

	POSITION tPosInScreen = GetScreenFromScroll();
	
	if (hMemDC)
	{
		if (0 < tPosInScreen.fx - m_tInfo.icx / 2)
			GdiTransparentBlt(hDC, 
				static_cast<int>(tPosInScreen.fx - m_tInfo.icx / 2 - m_tInfo.icx - m_iInterval), 
				static_cast<int>(tPosInScreen.fy - m_tInfo.icy / 2), 
				m_tInfo.icx, m_tInfo.icy, hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
		
		GdiTransparentBlt(hDC, static_cast<int>(tPosInScreen.fx - m_tInfo.icx / 2),
			static_cast<int>(tPosInScreen.fy - m_tInfo.icy / 2), 
			m_tInfo.icx, m_tInfo.icy, 
			hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
		
		int iCount = 0;
		while (tPosInScreen.fx + m_tInfo.icx / 2 + (iCount * m_tInfo.icx) < WINCX)
		{
			++iCount;
			GdiTransparentBlt(hDC, 
				static_cast<int>(tPosInScreen.fx - m_tInfo.icx / 2 + (m_tInfo.icx * iCount) + m_iInterval),
					static_cast<int>(tPosInScreen.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy,
					hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
		}

		//if (tPosInScreen.fx + m_tInfo.icx / 2 < WINCX)
		//	GdiTransparentBlt(hDC, tPosInScreen.fx + m_tInfo.icx / 2 + m_tInfo.icx + m_iInterval, 
		//		tPosInScreen.fy - m_tInfo.icy / 2, m_tInfo.icx, m_tInfo.icy, 
		//		hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	}
	
	else
	{
		int icx = m_tInfo.icx;
		int icy = m_tInfo.icy;
		if (icx == 0)	icx = 100;
		if (icy == 0)	icy = 100;
		CAN_NOT_FOUND_BITMAP(hDC, static_cast<int>(tPosInScreen.fx - m_tInfo.icx / 2), 
			static_cast<int>(tPosInScreen.fy - m_tInfo.icy / 2), icx, icy);
	};
}

void CBGObject::Release()
{
}
