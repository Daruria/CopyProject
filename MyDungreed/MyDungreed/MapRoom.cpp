#include "stdafx.h"
#include "MapRoom.h"
#include "WormIcon.h"
#include "UIWindow.h"
#include "MapWindow.h"

CMapRoom::CMapRoom():
	m_bIsCurRoom(false),
	m_bIsWormIconOn(false),
	m_eStage(SC_END),
	m_pWormIcon(nullptr),
	m_bBlinking(false),
	m_dwBlinkingTime(0),
	m_dwBlinkingInterval(0)
{
}


CMapRoom::~CMapRoom()
{
	Release();
}

void CMapRoom::Initialize()
{
	INSERT_BITMAP("MapRoom", L"Image/UI/Map/Room.bmp");
	INSERT_BITMAP("CurrentMapRoom", L"Image/UI/Map/CurrentRoom.bmp");

	m_tInfo.icx = 24;
	m_tInfo.icy = 24;

	m_dwBlinkingTime = GetTickCount();
	m_dwBlinkingInterval = 500;
}

int CMapRoom::Update()
{
	CUIWindow* pUIWindow = GET_SINGLE(CPlayerUI)->FindUIWindow("Map");

	CMapWindow* pMapWindow = nullptr;

	if (pUIWindow)
		pMapWindow = dynamic_cast<CMapWindow*>(pUIWindow);

	if (pMapWindow && pMapWindow->GetCurStage() == m_eStage)
	{
		m_bIsCurRoom = true;

		if (m_dwBlinkingTime + m_dwBlinkingInterval < GetTickCount())
		{
			m_bBlinking = !m_bBlinking;
			m_dwBlinkingTime = GetTickCount();
		}
	}
	else
		m_bIsCurRoom = false;

	if (m_pWormIcon)
	{
		if (m_pWormIcon->Update())
		{
			m_bIsWormIconOn = true;
			return 1;
		}
		else
			m_bIsWormIconOn = false;
	}

	return 0;
}

void CMapRoom::LateUpdate()
{
}

void CMapRoom::Render(HDC hDC)
{
	//HDC hMemDC = NULL;

	//if (m_bIsCurRoom)
	//	hMemDC = FIND_BITMAP_DC(m_bBlinking ? "MapRoom" : "CurrentMapRoom");
	//else
	//	hMemDC = FIND_BITMAP_DC("MapRoom");

	//SIZE tSize = GET_BITMAP_SIZE("MapTitle");

	//float fRatio = 3.2f;

	//int iLeftEdge = static_cast<int>(WINCX / 2 - tSize.cx * 0.5f * fRatio + 10);
	//int iRightEdge = static_cast<int>(WINCX / 2 + tSize.cx * 0.5f * fRatio - 10);

	//int iTopEdge = static_cast<int>(WINCY / 2 - tSize.cy * 0.5f * fRatio + 10);
	//int iBottomEdge = static_cast<int>(WINCY / 2 - tSize.cy * 0.5f * fRatio - 10);

	//int iRenderStartX = static_cast<int>(m_tInfo.fx - m_tInfo.icx * m_fRatio * 0.5f);
	//int iRenderStartY = static_cast<int>(m_tInfo.fy - m_tInfo.icy * m_fRatio * 0.5f);

	//int iRenderSizeX = static_cast<int>(m_tInfo.icx * m_fRatio);
	//int iRenderSizeY = static_cast<int>(m_tInfo.icy * m_fRatio);

	//int iRenderEndX = iRenderStartX + iRenderSizeX;
	//int iRenderEndY = iRenderStartY + iRenderSizeY;

	//int iSrcStartX = 0;
	//int iSrcStartY = 0;

	//int iSrcSizeX = m_tInfo.icx;
	//int iSrcSizeY = m_tInfo.icy;

	//if (iRenderStartX < iLeftEdge)
	//{
	//	iRenderStartX = iLeftEdge;
	//	iRenderSizeX -= (iLeftEdge - iRenderStartX);

	//	iSrcStartX = static_cast<int>((iLeftEdge - iRenderStartX) / m_fRatio);
	//	iSrcSizeX = static_cast<int>(iRenderSizeX / m_fRatio);
	//}
	//else if (iRenderEndX > iRightEdge)
	//{
	//	iRenderSizeX -= (iRenderEndX - iRightEdge);
	//	iSrcSizeX = static_cast<int>(iRenderSizeX / m_fRatio);
	//}

	//if (iRenderStartY < iTopEdge)
	//{
	//	iRenderStartY = iTopEdge;
	//	iRenderSizeY -= (iTopEdge - iRenderStartY);

	//	iSrcStartY = static_cast<int>((iTopEdge - iRenderStartY) / m_fRatio);
	//	iSrcSizeY = static_cast<int>(iRenderSizeY / m_fRatio);
	//}
	//else if (iRenderEndY > iBottomEdge)
	//{
	//	iRenderSizeY -= (iRenderEndY - iBottomEdge);
	//	iSrcSizeY = static_cast<int>(iRenderSizeY / m_fRatio);
	//}

	//bool bRender = GdiTransparentBlt(hDC, iRenderStartX, iRenderStartY, iRenderSizeX, iRenderSizeY,
	//	hMemDC, iSrcStartX, iSrcStartY, iSrcSizeX, iSrcSizeY, RGB(255, 0, 255));

	//if (m_pWormIcon)
	//	m_pWormIcon->Render(hDC, m_tInfo.fx, m_tInfo.fy);

	//LinkRender(hDC);
}

void CMapRoom::Release()
{
	SAFE_DELETE(m_pWormIcon);
}

void CMapRoom::Render(HDC hDC, float fScrollX, float fScrollY)
{
	HDC hMemDC = NULL;

	if (m_bIsCurRoom)
		hMemDC = FIND_BITMAP_DC(m_bBlinking ? "MapRoom" : "CurrentMapRoom");
	else
		hMemDC = FIND_BITMAP_DC("MapRoom");

	SIZE tSize = GET_BITMAP_SIZE("MapBase");

	float fRatio = 3.2f;

	int iLeftEdge	= static_cast<int>(WINCX / 2	- (tSize.cx * 0.5f - 3) * fRatio);
	int iRightEdge	= static_cast<int>(WINCX / 2	+ (tSize.cx * 0.5f - 3) * fRatio);

	int iTopEdge	= static_cast<int>(WINCY / 2	- (tSize.cy * 0.5f - 3) * fRatio);
	int iBottomEdge = static_cast<int>(WINCY / 2	+ (tSize.cy * 0.5f - 3) * fRatio);

	int iRenderStartX = static_cast<int>(fScrollX + m_tInfo.fx - m_tInfo.icx * m_fRatio * 0.5f);
	int iRenderStartY = static_cast<int>(fScrollY + m_tInfo.fy - m_tInfo.icy * m_fRatio * 0.5f);

	int iRenderSizeX = static_cast<int>(m_tInfo.icx * m_fRatio);
	int iRenderSizeY = static_cast<int>(m_tInfo.icy * m_fRatio);

	int iRenderEndX = iRenderStartX + iRenderSizeX;
	int iRenderEndY = iRenderStartY + iRenderSizeY;

	int iSrcStartX = 0;
	int iSrcStartY = 0;

	int iSrcSizeX = m_tInfo.icx;
	int iSrcSizeY = m_tInfo.icy;

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

	if (m_pWormIcon)
		m_pWormIcon->Render(hDC, fScrollX + m_tInfo.fx, fScrollY + m_tInfo.fy);

	LinkRender(hDC, fScrollX, fScrollY);
}

void CMapRoom::AddWormIcon()
{
	if (m_pWormIcon)
		return;

	m_pWormIcon = new CWormIcon;
	m_pWormIcon->Initialize("WormIcon", L"Image/UI/Map/Worm.bmp", "WormIconOn", L"Image/UI/Map/Worm_Selected.bmp");
	m_pWormIcon->SetRatio(2.f);
	m_pWormIcon->SetPoint(static_cast<int>(m_tInfo.fx), static_cast<int>(m_tInfo.fy));
}

void CMapRoom::LinkRender(HDC hDC)
{
	if (m_usFlagLink & Flag_Link_Up)
		CMath::DrawLine(hDC,
			static_cast<int>(m_tInfo.fx),
			static_cast<int>(m_tInfo.fy - m_tInfo.icy * 0.5f),
			static_cast<int>(m_tInfo.fx),
			static_cast<int>(m_tInfo.fy - m_tInfo.icy * 0.5f - 50), RGB(255, 255, 255));

	if (m_usFlagLink & Flag_Link_Down)
		CMath::DrawLine(hDC,
			static_cast<int>(m_tInfo.fx),
			static_cast<int>(m_tInfo.fy + m_tInfo.icy * 0.5f),
			static_cast<int>(m_tInfo.fx),
			static_cast<int>(m_tInfo.fy + m_tInfo.icy * 0.5f + 50), RGB(255, 255, 255));

	if (m_usFlagLink & Flag_Link_Left)
		CMath::DrawLine(hDC,
			static_cast<int>(m_tInfo.fx - m_tInfo.icx * 0.5f),
			static_cast<int>(m_tInfo.fy),
			static_cast<int>(m_tInfo.fx - m_tInfo.icx * 0.5f - 50),
			static_cast<int>(m_tInfo.fy), RGB(255, 255, 255));

	if (m_usFlagLink & Flag_Link_Right)
		CMath::DrawLine(hDC,
			static_cast<int>(m_tInfo.fx + m_tInfo.icx * 0.5f),
			static_cast<int>(m_tInfo.fy),
			static_cast<int>(m_tInfo.fx + m_tInfo.icx * 0.5f + 50),
			static_cast<int>(m_tInfo.fy), RGB(255, 255, 255));

}

void CMapRoom::LinkRender(HDC hDC, float fScrollX, float fScrollY)
{
	SIZE tSize = GET_BITMAP_SIZE("MapBase");

	float fRatio = 3.2f;

	int iLeftEdge	= static_cast<int>(WINCX / 2 - (tSize.cx * 0.5f - 3) * fRatio);
	int iRightEdge	= static_cast<int>(WINCX / 2 + (tSize.cx * 0.5f - 3) * fRatio);
												  			   	   
	int iTopEdge	= static_cast<int>(WINCY / 2 - (tSize.cy * 0.5f - 3) * fRatio);
	int iBottomEdge = static_cast<int>(WINCY / 2 + (tSize.cy * 0.5f - 3) * fRatio);

	if (m_usFlagLink & Flag_Link_Up)
	{
		int iX = static_cast<int>(fScrollX + m_tInfo.fx);
		int iTopY = static_cast<int>(fScrollY + m_tInfo.fy - m_tInfo.icy * m_fRatio * 0.5f - 20);
		int iBottomY = static_cast<int>(fScrollY + m_tInfo.fy - m_tInfo.icy * m_fRatio * 0.5f);
		if (iTopY < iTopEdge)
			iTopY = iTopEdge;
		if (iBottomY > iBottomEdge)
			iBottomY = iBottomEdge;

		if (iLeftEdge <= iX && iX <= iRightEdge &&
			iTopEdge <= iBottomY && iTopY <= iBottomEdge)
			CMath::DrawLine(hDC, iX, iBottomY, iX, iTopY, RGB(255, 255, 255));
	}

	if (m_usFlagLink & Flag_Link_Down)
	{
		int iX = static_cast<int>(fScrollX + m_tInfo.fx);
		int iTopY = static_cast<int>(fScrollY + m_tInfo.fy + m_tInfo.icy * m_fRatio * 0.5f);
		int iBottomY = static_cast<int>(fScrollY + m_tInfo.fy + m_tInfo.icy * m_fRatio * 0.5f + 20);

		if (iTopY < iTopEdge)
			iTopY = iTopEdge;
		if (iBottomY > iBottomEdge)
			iBottomY = iBottomEdge;

		if (iLeftEdge <= iX && iX <= iRightEdge &&
			iTopEdge <= iBottomY && iTopY <= iBottomEdge)
			CMath::DrawLine(hDC, iX, iBottomY, iX, iTopY, RGB(255, 255, 255));
	}

	if (m_usFlagLink & Flag_Link_Left)
	{
		int iLeftX = static_cast<int>(fScrollX + m_tInfo.fx - m_tInfo.icx * m_fRatio * 0.5f - 20);
		int iRightX = static_cast<int>(fScrollX + m_tInfo.fx - m_tInfo.icx * m_fRatio * 0.5f);
		int iY = static_cast<int>(fScrollY + m_tInfo.fy);

		if (iLeftX < iLeftEdge)
			iLeftX = iLeftEdge;
		if (iRightX > iRightEdge)
			iRightX = iRightEdge;

		if (iLeftEdge <= iRightX && iLeftX <= iRightEdge && 
			iTopEdge <= iY && iY <= iBottomEdge)
			CMath::DrawLine(hDC, iLeftX, iY, iRightX, iY, RGB(255, 255, 255));
	}

	if (m_usFlagLink & Flag_Link_Right)
	{
		int iLeftX = static_cast<int>(fScrollX + m_tInfo.fx + m_tInfo.icx * m_fRatio * 0.5f);
		int iRightX = static_cast<int>(fScrollX + m_tInfo.fx + m_tInfo.icx * m_fRatio * 0.5f + 20);
		int iY = static_cast<int>(fScrollY + m_tInfo.fy);

		if (iLeftX < iLeftEdge)
			iLeftX = iLeftEdge;
		if (iRightX > iRightEdge)
			iRightX = iRightEdge;

		if (iLeftEdge <= iRightX && iLeftX <= iRightEdge &&
			iTopEdge <= iY && iY <= iBottomEdge)
			CMath::DrawLine(hDC, iLeftX, iY, iRightX, iY, RGB(255, 255, 255));
	}
}
