#include "stdafx.h"
#include "Commander.h"
#include "UIWindow.h"
#include "Conversation.h"
#include "Player.h"
#include "SelectBox.h"
#include "AbilityWindow.h"

CCommander::CCommander():
	m_ePreState(END),
	m_eCurState(END)
{
}


CCommander::~CCommander()
{
	Release();
}

void CCommander::Initialize()
{
	INSERT_BITMAP("Keyboard_F", L"Image/Keyboard/Keyboard_F.bmp");

	INSERT_BITMAP("CommanderIdle", L"Image/NPC/Commander/CommanderIdle.bmp");
	INSERT_BITMAP("CommanderMove", L"Image/NPC/Commander/CommanderMove.bmp");
	
	m_eCurState = IDLE;

	m_strKey = "Coblovina";

	m_iFrame			= 0;
	m_iMaxFrame			= 6;
	m_iFrameDir			= 1;
	m_dwFrameMaxTime	= 100;
	m_dwFrameTime		= GetTickCount();

	m_fRatio = 4.f;

	m_tInfo.icx = 23;
	m_tInfo.icy = 25;

	m_tHitBox = {0.f, 0.f, static_cast<int>(m_tInfo.icx * m_fRatio), static_cast<int>(m_tInfo.icy * m_fRatio) };



	//list<CUIWindow*>* m_pWindowList = GET_SINGLE(CPlayerUI)->GetWindowList();
	//bool bIsAlreadyExist = false;
	//for (auto pWindow : *m_pWindowList)
	//{
	//	if (pWindow->GetKey() == m_strKey)
	//	{
	//		bIsAlreadyExist = true;
	//		m_pAbilityWindow = dynamic_cast<CAbilityWindow*>(pWindow);
	//		break;
	//	}
	//}

	//if (!bIsAlreadyExist)
	//{
	//	m_pAbilityWindow = new CAbilityWindow;
	//	m_pAbilityWindow->Initialize();
	//	m_pAbilityWindow->SetKey(m_strKey);
	//}






}

int CCommander::Update()
{
	CMoveObj::Update();
	ChangeState();

	if (GET_SINGLE(CPlayerUI)->IsThereOpenWindow())
		m_bCanInterAct = false;

	if (m_bCanInterAct && KEYDOWN('F'))
		Interaction();

	return 0;
}

void CCommander::LateUpdate()
{
}

void CCommander::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));


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

void CCommander::Release()
{
}

void CCommander::Interaction()
{
	CConversation* pConversation = nullptr;

	list<CUIWindow*>* pWindowList = GET_SINGLE(CPlayerUI)->GetWindowList();
	for (auto pUIWindow : *pWindowList)
	{
		if (pUIWindow->GetKey() == m_strKey)
		{
			pConversation = dynamic_cast<CConversation*>(pUIWindow);
			break;
		}
	}

	if (nullptr == pConversation)
	{
		pConversation = new CConversation;
		pConversation->Initialize();
		pConversation->SetPoint(0, WINCY - 250);
		pConversation->SetSize(WINCX, 250);
		pConversation->SetKey(m_strKey);
		pConversation->AddTextLine("You're here! are you ready to train?");

		CSelectBox* pSelectBox = new CSelectBox;
		pSelectBox->Initialize("", L"", "TalkChoice", L"Image/UI/Rect/TalkChoice.bmp");
		pSelectBox->BoxSetting("Train", SIZE{ 2, 2 }, 2.f, 2.f, CSelectBox::CENTER, CSelectBox::MIDDLE);
		pSelectBox->SetParentWindow(pConversation);

		CAbilityWindow* pAbilityWindow = dynamic_cast<CAbilityWindow*>(GET_SINGLE(CPlayerUI)->FindUIWindow(m_strKey));

		if (nullptr == pAbilityWindow)
		{
			pAbilityWindow = new CAbilityWindow;
			pAbilityWindow->Initialize();
			pAbilityWindow->SetKey("AbilityWindow");
			pAbilityWindow->SetIsOn(false);
			GET_SINGLE(CPlayerUI)->AddWindow(pAbilityWindow);
		}
		pSelectBox->SetChildWindow(pAbilityWindow);
		pConversation->AddSelectBox(pSelectBox);

		pSelectBox = new CSelectBox;
		pSelectBox->Initialize("", L"", "TalkChoice", L"Image/UI/Rect/TalkChoice.bmp");
		pSelectBox->BoxSetting("Nothing", SIZE{ 2, 2 }, 2.f, 2.f, CSelectBox::CENTER, CSelectBox::MIDDLE);
		pSelectBox->SetParentWindow(pConversation);
		pConversation->AddSelectBox(pSelectBox);

		GET_SINGLE(CPlayerUI)->AddWindow(pConversation);
	}

	pConversation->SetIsOn(true);
	GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(false);

}

void CCommander::ChangeState()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CCommander::IDLE:
			m_iFrame = 0;
			m_iMaxFrame = 6;
			m_dwFrameMaxTime = 100;
			break;
		case CCommander::MOVE:
			m_iFrame = 0;
			m_iMaxFrame = 8;
			m_dwFrameMaxTime = 100;
			break;
		case CCommander::END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

HDC CCommander::MotionDC()
{
	switch (m_eCurState)
	{
	case CCommander::IDLE:	return FIND_BITMAP_DC("CommanderIdle");
	case CCommander::MOVE:	return FIND_BITMAP_DC("CommanderMove");
	case CCommander::END:	return NULL;
	default:				return NULL;
	}
	return NULL;
}
