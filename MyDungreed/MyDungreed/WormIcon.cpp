#include "stdafx.h"
#include "WormIcon.h"
#include "MapWindow.h"

CWormIcon::CWormIcon()
{
}


CWormIcon::~CWormIcon()
{
	Release();
}

void CWormIcon::Initialize(const string & strKey, const wchar_t * pPath, const string & strOnKey, const wchar_t * pOnPath)
{
	CMyButton::Initialize(strKey, pPath, strOnKey, pOnPath);
	

}

int CWormIcon::Update()
{
	m_bIsOnMouse = IsOnMouseScroll() ? true : false;

	if (m_bIsOnMouse)
		return 1;

	return 0;
}

void CWormIcon::LateUpdate()
{
}

void CWormIcon::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_bIsOnMouse ? m_strOnKey : m_strKey);
	SIZE tSize = m_bIsOnMouse ? m_tOnSize : m_tSize;
	
	GdiTransparentBlt(hDC,
		static_cast<int>(m_tPoint.x - tSize.cx * m_fRatio * 0.5f),
		static_cast<int>(m_tPoint.y - tSize.cy * m_fRatio * 0.5f),
		static_cast<int>(tSize.cx * m_fRatio),
		static_cast<int>(tSize.cy * m_fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
}

void CWormIcon::Release()
{
}

void CWormIcon::Render(HDC hDC, float fx, float fy)
{
	HDC hMemDC = FIND_BITMAP_DC(m_bIsOnMouse ? m_strOnKey : m_strKey);
	SIZE tSize1 = m_bIsOnMouse ? m_tOnSize : m_tSize;

	SIZE tSize = GET_BITMAP_SIZE("MapBase");

	float fRatio = 3.2f;

	int iLeftEdge	= static_cast<int>(WINCX / 2	- (tSize.cx * 0.5f - 3) * fRatio);
	int iRightEdge	= static_cast<int>(WINCX / 2	+ (tSize.cx * 0.5f - 3) * fRatio);

	int iTopEdge	= static_cast<int>(WINCY / 2	- (tSize.cy * 0.5f - 3) * fRatio);
	int iBottomEdge = static_cast<int>(WINCY / 2	+ (tSize.cy * 0.5f - 3) * fRatio);

	int iRenderStartX = static_cast<int>(fx - tSize1.cx * m_fRatio * 0.5f);
	int iRenderStartY = static_cast<int>(fy - tSize1.cy * m_fRatio * 0.5f);

	int iRenderSizeX = static_cast<int>(tSize1.cx * m_fRatio);
	int iRenderSizeY = static_cast<int>(tSize1.cy * m_fRatio);

	int iRenderEndX = iRenderStartX + iRenderSizeX;
	int iRenderEndY = iRenderStartY + iRenderSizeY;

	int iSrcStartX = 0;
	int iSrcStartY = 0;

	int iSrcSizeX = tSize1.cx;
	int iSrcSizeY = tSize1.cy;

	if (iRenderStartX < iLeftEdge)
	{
		iSrcStartX = static_cast<int>((iLeftEdge - iRenderStartX) / m_fRatio);

		iRenderSizeX -= (iLeftEdge - iRenderStartX);
		iRenderStartX = iLeftEdge;

		iSrcSizeX = static_cast<int>(iRenderSizeX / m_fRatio);
	}
	else if (iRenderEndX > iRightEdge)
	{
		iRenderSizeX -= (iRenderEndX - iRightEdge);
		iSrcSizeX = static_cast<int>(iRenderSizeX / m_fRatio);
	}

	if (iRenderStartY < iTopEdge)
	{
		iSrcStartY = static_cast<int>((iTopEdge - iRenderStartY) / m_fRatio);

		iRenderSizeY -= (iTopEdge - iRenderStartY);
		iRenderStartY = iTopEdge;

		iSrcSizeY = static_cast<int>(iRenderSizeY / m_fRatio);
	}
	else if (iRenderEndY > iBottomEdge)
	{
		iRenderSizeY -= (iRenderEndY - iBottomEdge);
		iSrcSizeY = static_cast<int>(iRenderSizeY / m_fRatio);
	}

	bool bRender = GdiTransparentBlt(hDC, iRenderStartX, iRenderStartY, iRenderSizeX, iRenderSizeY,
		hMemDC, iSrcStartX, iSrcStartY, iSrcSizeX, iSrcSizeY, RGB(255, 0, 255));

	//GdiTransparentBlt(hDC,
	//	static_cast<int>(fx - tSize.cx * m_fRatio * 0.5f),
	//	static_cast<int>(fy - tSize.cy * m_fRatio * 0.5f),
	//	static_cast<int>(tSize.cx * m_fRatio),
	//	static_cast<int>(tSize.cy * m_fRatio),
	//	hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
}

const BOOL CWormIcon::IsOnMouseScroll()
{
	CMapWindow* pMapWindow = dynamic_cast<CMapWindow*>(GET_SINGLE(CPlayerUI)->FindUIWindow("Map"));

	if (pMapWindow)
	{
		float fScrollX = pMapWindow->GetScrollX();
		float fScrollY = pMapWindow->GetScrollY();

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		RECT rc =
		{
			(int)(m_tPoint.x + fScrollX - m_tSize.cx * m_fRatio / 2),
			(int)(m_tPoint.y + fScrollY - m_tSize.cy * m_fRatio / 2),
			(int)(m_tPoint.x + fScrollX + m_tSize.cx * m_fRatio / 2),
			(int)(m_tPoint.y + fScrollY + m_tSize.cy * m_fRatio / 2)
		};

		return PtInRect(&rc, pt);
	}

	
}
