#include "stdafx.h"
#include "UIWindow.h"


CUIWindow::CUIWindow():
	m_bLife(true),
	m_bIsOn(false),
	m_fRatio(1.f)
{
}


CUIWindow::~CUIWindow()
{
	Release();
}

void CUIWindow::Initialize()
{
	INSERT_BITMAP("TalkBox", L"Image/UI/Rect/TalkBox.bmp");
}

void CUIWindow::Initialize(const string & strKey, const wchar_t * pPath)
{
}

int CUIWindow::Update()
{
	return 0;
}

void CUIWindow::LateUpdate()
{
}

void CUIWindow::Render(HDC hDC)
{
}

void CUIWindow::Release()
{
}

void CUIWindow::TalkBoxRender(HDC hDC, float fx, float fy, int icx, int icy, 
							float fRatio, TALK_BOX_SORT_BY eXSortBy, TALK_BOX_SORT_BY eYSortBy)
{
	HDC hMemDC = FIND_BITMAP_DC("TalkBox");
	
	float fRenderStartX = 0, fRenderStartY = 0;

	switch (eXSortBy)
	{
	case CUIWindow::LEFT:	fRenderStartX = fx;				break;
	case CUIWindow::CENTER:	fRenderStartX = fx - icx / 2;	break;
	case CUIWindow::RIGHT:	fRenderStartX = fx - icx;		break;
	}
	switch (eYSortBy)
	{
	case CUIWindow::LEFT:	fRenderStartY = fy;				break;
	case CUIWindow::CENTER: fRenderStartY = fy - icy / 2;	break;
	case CUIWindow::RIGHT:	fRenderStartY = fy - icy;		break;
	}

	int iArrfX[3] = {	static_cast<int>(fRenderStartX), 
						static_cast<int>(fRenderStartX + 4 * fRatio), 
						static_cast<int>(fRenderStartX + icx - 4 * fRatio) };
	int iArrfY[3] = {	static_cast<int>(fRenderStartY), 
						static_cast<int>(fRenderStartY + 4 * fRatio),
						static_cast<int>(fRenderStartY + icy - 4 * fRatio) };

	int iArrCX[3] = {	static_cast<int>(4 * fRatio), 
						static_cast<int>(icx - 8 * fRatio),	
						static_cast<int>(4 * fRatio) };
	int iArrCY[3] = {	static_cast<int>(4 * fRatio),	
						static_cast<int>(icy - 8 * fRatio), 
						static_cast<int>(4 * fRatio) };
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			BOOL bRender = GdiTransparentBlt(hDC, 
											iArrfX[j], iArrfY[i], 
											iArrCX[j], iArrCY[i], hMemDC, 
											j * 4, i * 4, 4, 4, RGB(255, 0, 255));
		}
	}
}
