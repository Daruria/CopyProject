#include "stdafx.h"
#include "WormPassage.h"
#include "MapWindow.h"
#include "Player.h"

CWormPassage::CWormPassage():
	m_bEatEnd(false),
	m_bDisgorgeEnd(false),
	m_ePreState(END),
	m_eCurState(END)
{
}


CWormPassage::~CWormPassage()
{
	Release();
}

void CWormPassage::Initialize()
{
	INSERT_BITMAP("WormPassageIdle", L"Image/MiniMap/Worm/WormPassageIdle.bmp");
	INSERT_BITMAP("WormPassageEat", L"Image/MiniMap/Worm/WormPassageEat.bmp");

	m_eCurState = IDLE;

	m_tInfo.icx = 27;
	m_tInfo.icy = 31;

	m_iFrameDir = 1;
}

int CWormPassage::Update()
{
	ChangeState();

	int iPreFrame = m_iFrame;
	DWORD dwCurTime = GetTickCount();
	if (m_dwFrameTime + m_dwFrameMaxTime < dwCurTime)
	{
		int iFrame = m_iFrame + m_iFrameDir;
		switch (m_eCurState)
		{
		case CWormPassage::IDLE:
			m_iFrame = iFrame >= m_iMaxFrame ? 0 : iFrame < 0 ? m_iMaxFrame - 1 : iFrame;
			m_dwFrameTime = dwCurTime;
			break;
		case CWormPassage::EAT:
			m_iFrame = iFrame;
			m_dwFrameTime = dwCurTime;
			if (m_iFrame >= m_iMaxFrame)
			{
				if (!m_bEatEnd)
					GET_SINGLE(CEffectManager)->AddEffect("DieFX", L"Image/Effect/Die/DieFX.bmp", m_tInfo.fx, m_tInfo.fy, 256, 256, 11);
				m_bEatEnd = true;
			}
			break;
		case CWormPassage::DISGORGE:
			m_iFrame = iFrame;
			m_dwFrameTime = dwCurTime;
			if (m_iFrame <= -1)
			{
				m_bDisgorgeEnd = true;
				GET_SINGLE(CEffectManager)->AddEffect("DieFX", L"Image/Effect/Die/DieFX.bmp", m_tInfo.fx, m_tInfo.fy, 256, 256, 11);
				m_eCurState = CWormPassage::IDLE;
			}
			break;
		case CWormPassage::END:
			break;
		default:
			break;
		}

	}

	if (GET_SINGLE(CPlayerUI)->IsThereOpenWindow())
		m_bCanInterAct = false;
	
	if (m_bCanInterAct && KEYDOWN('F'))
		Interaction();

	return 0;
}

void CWormPassage::LateUpdate()
{
}

void CWormPassage::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		static_cast<int>(tPos.fx - m_tInfo.icx * 0.5f * m_fRatio),
		static_cast<int>(tPos.fy - m_tInfo.icy * 0.5f * m_fRatio),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC, m_tInfo.icx * m_iFrame, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	if (m_bCanInterAct)
	{
		hMemDC = FIND_BITMAP_DC("Keyboard_F");
		SIZE tSize = GET_BITMAP_SIZE("Keyboard_F");

		GdiTransparentBlt(hDC,
			static_cast<int>(tPos.fx - tSize.cx * 0.5f * 3.f),
			static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f - tSize.cy * 0.5f * 3.f),
			static_cast<int>(tSize.cx * 3.f),
			static_cast<int>(tSize.cy * 3.f),
			hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
	}
	
}

void CWormPassage::Release()
{
}

void CWormPassage::Interaction()
{
	CMapWindow* pMapWindow = dynamic_cast<CMapWindow*>(GET_SINGLE(CPlayerUI)->FindUIWindow("Map"));

	if (nullptr == pMapWindow)
	{
		pMapWindow = new CMapWindow;
		pMapWindow->Initialize();
		pMapWindow->SetPoint(WINCX/2, WINCY/2);
		pMapWindow->SetSize(WINCX, WINCY);
		pMapWindow->SetKey("Map");

		GET_SINGLE(CPlayerUI)->AddWindow(pMapWindow);
	}

	pMapWindow->SetIsOn(true);
	GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(false);
}

void CWormPassage::Eat()
{
	m_eCurState = CWormPassage::EAT;
}

void CWormPassage::Disgorge()
{
	m_eCurState = CWormPassage::DISGORGE;
}

void CWormPassage::ChangeState()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CWormPassage::IDLE:
			m_iFrame = 0;
			m_iMaxFrame = 9;
			m_dwFrameMaxTime = 50;
			m_iFrameDir = 1;
			m_dwFrameTime = GetTickCount();
			break;
		case CWormPassage::EAT:
			m_iFrame = 0;
			m_iMaxFrame = 4;
			m_dwFrameMaxTime = 50;
			m_iFrameDir = 1;
			m_dwFrameTime = GetTickCount();
			break;
		case CWormPassage::DISGORGE:
			m_iFrame = 3;
			m_iMaxFrame = 4;
			m_dwFrameMaxTime = 50;
			m_iFrameDir = -1;
			m_dwFrameTime = GetTickCount();
			break;
		case CWormPassage::END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

HDC CWormPassage::MotionDC()
{
	HDC hMemDC = NULL;
	switch (m_eCurState)
	{
	case CWormPassage::IDLE:
		hMemDC = FIND_BITMAP_DC("WormPassageIdle");
		break;
	case CWormPassage::EAT:
		hMemDC = FIND_BITMAP_DC("WormPassageEat");
		break;
	case CWormPassage::DISGORGE:
		hMemDC = FIND_BITMAP_DC("WormPassageEat");
		break;
	case CWormPassage::END:
		break;
	default:
		break;
	}

	return hMemDC;
}
