#include "stdafx.h"
#include "MapWindow.h"
#include "MapRoom.h"
#include "Player.h"
#include "MyButton.h"

CMapWindow::CMapWindow():
	m_bIsOnIcon(false),
	m_pCurRoom(nullptr),
	m_fScrollX(0.f),
	m_fScrollY(0.f),
	m_bPrevFastMove(false),
	m_bIsScrolling(false),
	m_pExitButton(nullptr),
	m_bFastMove(true)
{
	ZeroMemory(&m_tClickedPos, sizeof(m_tClickedPos));
	ZeroMemory(&m_tScrollWhenClick, sizeof(m_tScrollWhenClick));
	ZeroMemory(&m_tFastMoveDest, sizeof(m_tFastMoveDest));
}

CMapWindow::~CMapWindow()
{
	Release();
}

void CMapWindow::Initialize()
{
	INSERT_BITMAP("WindowBack", L"Image/UI/Rect/WindowBack.bmp");
	INSERT_BITMAP("MapTitle",	L"Image/UI/Map/MapBaseTitle.bmp");
	INSERT_BITMAP("MapBase",	L"Image/UI/Map/MapBase 1_1.bmp");

	m_strKey = "Map";

	m_fScrollX = WINCX * 0.5f;
	m_fScrollY = WINCY * 0.5f;

	m_pExitButton = new CMyButton;
	m_pExitButton->Initialize("FullWindowExitButton", 
		L"Image/UI/Map/FullWindowExitButton.bmp", 
		"FullWindowExitButton_Selected", 
		L"Image/UI/Map/FullWindowExitButton_Selected.bmp");
	m_pExitButton->SetPoint(WINCX - 50, 50);
	m_pExitButton->SetRatio(3.2f);

}

void CMapWindow::Initialize(const string & strKey, const wchar_t * pPath)
{
}

int CMapWindow::Update()
{
	if (m_bIsOn)
		GET_SINGLE(CMouse)->ChangeCursor(0);

	m_bIsOnIcon = false;
	for (auto& pRoom : m_RoomList)
	{
		if (pRoom->Update())
		{
			if (m_bFastMove && m_pCurRoom != pRoom)
			{
				m_bIsOnIcon = true;
				m_tOnInfo = pRoom->GetInfo();
				if (KEYDOWN(VK_LBUTTON))
				{
					m_bIsOn = false;
					m_tFastMoveDest = pRoom->GetPassagePos();
					m_bPrevFastMove = true;
					//GET_SINGLE(CObjectManager)->GetPlayer()->SetInfo(tPos.fx, tPos.fy);
					GET_SINGLE(CObjectManager)->GetPlayer()->SetBoolRender(false);
					GET_SINGLE(CSoundMgr)->PlaySound(L"DungeonOut.wav", CSoundMgr::EFFECT);
					return pRoom->GetStage();
				}
			}
		}
	}

	if (m_pExitButton->Update())
	{
		m_bIsOn = false;
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
	}

	Scrolling();


	if (KEYDOWN(VK_ESCAPE))
	{
		m_bIsOn = false;
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
	}

	return 0;
}

void CMapWindow::LateUpdate()
{
	
}

void CMapWindow::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("WindowBack");
	SIZE tSize = GET_BITMAP_SIZE("WindowBack");

	GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	hMemDC =	FIND_BITMAP_DC("MapTitle");
	tSize =		GET_BITMAP_SIZE("MapTitle");

	float fRatio = 3.2f;

	GdiTransparentBlt(hDC, 0, 
		static_cast<int>(0), 
		static_cast<int>(tSize.cx * fRatio),
		static_cast<int>(tSize.cy * fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	for (auto& pRoom : m_RoomList)
		pRoom->Render(hDC, m_fScrollX, m_fScrollY);

	hMemDC	= FIND_BITMAP_DC("MapBase");
	tSize	= GET_BITMAP_SIZE("MapBase");

	GdiTransparentBlt(hDC,
		static_cast<int>(WINCX / 2 - tSize.cx * fRatio * 0.5f),
		static_cast<int>(WINCY / 2 - tSize.cy * fRatio * 0.5f),
		static_cast<int>(tSize.cx * fRatio),
		static_cast<int>(tSize.cy * fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255,0,255));


	if (m_bIsOnIcon)
	{
		INFO tStart = m_pCurRoom->GetInfo();

		CMath::DrawLine(hDC, tStart.fx + m_fScrollX, tStart.fy + m_fScrollY,
			m_tOnInfo.fx + m_fScrollX, m_tOnInfo.fy + m_fScrollY, RGB(0, 255, 0));
	}

	m_pExitButton->Render(hDC);
}

void CMapWindow::Release()
{
	SAFE_DELETE(m_pExitButton);

	for (auto* pRoom : m_RoomList)
		SAFE_DELETE(pRoom);
	m_RoomList.clear();
}

void CMapWindow::AddRoom(CMapRoom * m_pRoom, ROOM_POS ePos)
{
	if (!m_pCurRoom)
	{
		m_pRoom->SetInfo(0.f, 0.f);
		m_RoomList.push_back(m_pRoom);
		return;
	}

	INFO tInfo = m_pCurRoom->GetInfo();
	float fRatio = m_pCurRoom->GetRatio();

	float fArrX[CMapWindow::END] = {0, 0, -20.f - tInfo.icx * fRatio, 20.f + tInfo.icx * fRatio };
	float fArrY[CMapWindow::END] = {- 20.f - tInfo.icy * fRatio, 20.f + tInfo.icy * fRatio, 0.f, 0.f};

	m_pRoom->SetInfo(tInfo.fx + fArrX[ePos], tInfo.fy + fArrY[ePos]);

	m_RoomList.push_back(m_pRoom);

}

const SCENE_CHANGE CMapWindow::GetCurStage() const
{
	return m_pCurRoom->GetStage();
}

const CMapRoom * CMapWindow::FindRoom(SCENE_CHANGE eStage) const
{
	for (auto& pRoom : m_RoomList)
	{
		if (pRoom->GetStage() == eStage)
			return pRoom;
	}

	return nullptr;
}

void CMapWindow::Scrolling()
{
	if (KEYDOWN(VK_LBUTTON))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		m_tClickedPos.fx = static_cast<float>(pt.x);
		m_tClickedPos.fy = static_cast<float>(pt.y);

		m_tScrollWhenClick.fx = m_fScrollX;
		m_tScrollWhenClick.fy = m_fScrollY;

		m_bIsScrolling = true;
	}

	if (m_bIsScrolling)
	{
		if (KEYPRESS(VK_LBUTTON))
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			float fdx = static_cast<int>(pt.x - m_tClickedPos.fx);
			float fdy = static_cast<int>(pt.y - m_tClickedPos.fy);

			m_fScrollX = m_tScrollWhenClick.fx + fdx;
			m_fScrollY = m_tScrollWhenClick.fy + fdy;
		}
	}

	if (KEYUP(VK_LBUTTON))
	{
		m_bIsScrolling = false;
	}
}
