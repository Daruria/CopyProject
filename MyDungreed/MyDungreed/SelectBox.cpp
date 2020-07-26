#include "stdafx.h"
#include "SelectBox.h"
#include "UIWindow.h"
#include "Player.h"

CSelectBox::CSelectBox():
	m_eSortByX(X_END),
	m_eSortByY(Y_END),
	m_tEdgeSize(SIZE{}),
	m_fTextRatio(0.f),
	m_strText(""),
	m_pParentWindow(nullptr),
	m_pChildWindow(nullptr)
{
}


CSelectBox::~CSelectBox()
{
	Release();
}

void CSelectBox::Initialize(const string & strKey, const wchar_t * pPath, const string & strOnKey, const wchar_t * pOnPath)
{
	CMyButton::Initialize(strKey, pPath, strOnKey, pOnPath);
}

int CSelectBox::Update()
{
	m_bIsOnMouse = IsBoxOnMouse() ? true : false;

	if (m_bIsOnMouse && KEYUP(VK_LBUTTON))
	{
		if (!m_pChildWindow)		//	실행 할 창이 없으면 그냥 종료.
		{
			m_pParentWindow->SetLife(false);
			GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
			
			return 1;
		}
		else						//	창이 있으면, 오픈 하고 대화 종료.
		{
			m_pParentWindow->SetLife(false);		//	부모창(대화창) 종료
			//GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
			
			list<CUIWindow*>* pWindowList = GET_SINGLE(CPlayerUI)->GetWindowList();

			bool bIsAlreadyExist = false;
			for (auto pWindow : *pWindowList)		//	창 열기 전에, UIList에 이미 추가되었는지 확인.
			{
				if (m_pChildWindow->GetKey() == pWindow->GetKey())
				{
					bIsAlreadyExist = true;
					break;
				}
			}
			
			if (!bIsAlreadyExist)					//	추가 되어 있지 않으면 추가.
				GET_SINGLE(CPlayerUI)->AddWindow(m_pChildWindow);
				
			
			m_pChildWindow->WindowOn();			//	자식 창 오픈.

			CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
			//pPlayer->SetCanAction(false);			//	플레이어 키입력 막기.

			return 1;
		}
	}

	return 0;
}

void CSelectBox::LateUpdate()
{
}

void CSelectBox::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(!m_bIsOnMouse ? m_strKey : m_strOnKey);
	SIZE tSize = !m_bIsOnMouse ? m_tSize : m_tOnSize;

	int iRenderStartX = m_tPoint.x - static_cast<int>(m_eSortByX) * (m_tSize.cx / 2);
	int iRenderStartY = m_tPoint.y - static_cast<int>(m_eSortByY) * (m_tSize.cy / 2);

	int iArrX[3]	= { iRenderStartX, static_cast<int>(iRenderStartX + m_tEdgeSize.cx * m_fRatio), static_cast<int>(iRenderStartX + m_tSize.cx - m_tEdgeSize.cx * m_fRatio) };
	int iArrY[3]	= { iRenderStartY, static_cast<int>(iRenderStartY + m_tEdgeSize.cy * m_fRatio), static_cast<int>(iRenderStartY + m_tSize.cy - m_tEdgeSize.cy * m_fRatio) };

	int iArrCX[3]	= { static_cast<int>(m_tEdgeSize.cx * m_fRatio), static_cast<int>(m_tSize.cx - 2 * m_tEdgeSize.cx * m_fRatio), static_cast<int>(m_tEdgeSize.cx * m_fRatio) };
	int iArrCY[3]	= { static_cast<int>(m_tEdgeSize.cy * m_fRatio), static_cast<int>(m_tSize.cy - 2 * m_tEdgeSize.cy * m_fRatio), static_cast<int>(m_tEdgeSize.cy * m_fRatio) };

	SIZE tSrcSize = GET_BITMAP_SIZE(!m_bIsOnMouse ? m_strKey : m_strOnKey);

	int iArrSrcX[3]		= { 0, m_tEdgeSize.cx, tSrcSize.cx - m_tEdgeSize.cx};
	int iArrSrcY[3]		= { 0, m_tEdgeSize.cy, tSrcSize.cy - m_tEdgeSize.cy};

	int iArrSrcCX[3]	= {m_tEdgeSize.cx, tSrcSize.cx - m_tEdgeSize.cx * 2, m_tEdgeSize.cx};
	int iArrSrcCY[3]	= {m_tEdgeSize.cy, tSrcSize.cy - m_tEdgeSize.cy * 2, m_tEdgeSize.cy};

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			GdiTransparentBlt(hDC, iArrX[j], iArrY[i], iArrCX[j], iArrCY[i], hMemDC, iArrSrcX[j], iArrSrcY[i], iArrSrcCX[j], iArrSrcCY[i], RGB(255, 0, 255));
		}
	}

	GET_SINGLE(CFontManager)->Render(hDC, static_cast<float>(m_tPoint.x), static_cast<float>(m_tPoint.y), FT_NORMAL, m_fTextRatio, m_strText, FPX_CENTER, FPY_MIDDLE);

}

void CSelectBox::Release()
{
}

void CSelectBox::BoxSetting(const string & strText, const SIZE & tEdgeSize, float fRatio, float fTextRatio, BOX_SORT_BY_X eSortByX, BOX_SORT_BY_Y eSortByY)
{
	SetText(strText);
	SetEdgeSize(tEdgeSize);
	SetRatio(fRatio);
	SetTextRatio(fTextRatio);
	SetSortByX(eSortByX);
	SetSortByY(eSortByY);

	m_tSize.cx		= GET_SINGLE(CFontManager)->GetFontTotalXSize(m_strText, FT_NORMAL, m_fTextRatio) + static_cast<int>(m_tEdgeSize.cx * m_fRatio * 2);
	m_tOnSize.cx	= GET_SINGLE(CFontManager)->GetFontTotalXSize(m_strText, FT_NORMAL, m_fTextRatio) + static_cast<int>(m_tEdgeSize.cx * m_fRatio * 2);
	m_tSize.cy		= GET_SINGLE(CFontManager)->GetFontTotalYSize(m_strText, FT_NORMAL, m_fTextRatio) + static_cast<int>(m_tEdgeSize.cy * m_fRatio * 2);
	m_tOnSize.cy	= GET_SINGLE(CFontManager)->GetFontTotalYSize(m_strText, FT_NORMAL, m_fTextRatio) + static_cast<int>(m_tEdgeSize.cy * m_fRatio * 2);
}

const BOOL CSelectBox::IsBoxOnMouse() const
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc =
	{
		(int)(m_tPoint.x - m_tSize.cx / 2),
		(int)(m_tPoint.y - m_tSize.cy / 2),
		(int)(m_tPoint.x + m_tSize.cx / 2),
		(int)(m_tPoint.y + m_tSize.cy / 2)
	};

	return PtInRect(&rc, pt);
}