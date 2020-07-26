#include "stdafx.h"
#include "Merchant.h"
#include "UIWindow.h"
#include "ShopWindow.h"
#include "Item.h"
#include "ShortSword.h"
#include "Rod.h"
#include "Player.h"
#include "Conversation.h"
#include "SelectBox.h"

CMerchant::CMerchant():
	m_bShowItem(false)
{
}

CMerchant::~CMerchant()
{
	Release();
}

void CMerchant::Initialize()
{
	INSERT_BITMAP("Keyboard_F", L"Image/Keyboard/Keyboard_F.bmp");

	INSERT_BITMAP("GiantR", L"Image/NPC/Giant/GiantR.bmp");
	INSERT_BITMAP("GiantL", L"Image/NPC/Giant/GiantL.bmp");

	m_bDir = false;
	m_fRatio = 4.f;
	m_tInfo.icx = 49;
	m_tInfo.icy = 48;
	m_tHitBox = { 0.f, 0.f, static_cast<int>(m_tInfo.icx * m_fRatio), static_cast<int>(m_tInfo.icy * m_fRatio) };

	m_iFrame = 0;
	m_iMaxFrame = 4;
	m_iFrameDir = 1;
	m_dwFrameMaxTime = 100;
	m_dwFrameTime = GetTickCount();

	list<CUIWindow*>* pUIWindowList = GET_SINGLE(CPlayerUI)->GetWindowList();

	bool bDupCheck = false;

	for (auto pUIWindow : *pUIWindowList)
	{
		if (pUIWindow->GetKey() == m_strKey)
		{
			bDupCheck = true;
			m_pShopWindow = dynamic_cast<CShopWindow*>(pUIWindow);
			break;
		}
	}
	
	if (!bDupCheck)
	{
		m_pShopWindow = new CShopWindow;
		m_pShopWindow->Initialize();
		m_pShopWindow->SetKey(m_strKey);

		CShortSword* pShortSword = new CShortSword;
		pShortSword->Initialize();
		m_pShopWindow->AddItem(pShortSword);

		CRod* pQuarterStaff = new CRod;
		pQuarterStaff->Initialize();
		m_pShopWindow->AddItem(pQuarterStaff);

		m_pShopWindow->SetRatio(4.f);
		m_pShopWindow->SetIsOn(false);

		GET_SINGLE(CPlayerUI)->AddWindow(m_pShopWindow);
	}
}

int CMerchant::Update()
{
	CMoveObj::Update();
	
	//if (m_bCanInterAct && KEYDOWN('F'))
	//{
	//	m_pShopWindow->SetIsOn(true);
	//	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	//	pPlayer->SetCanAction(false);
	//	GET_SINGLE(CInventory)->OpenInven();
	//}

	//if (m_pShopWindow->GetIsOn())
	//	Interaction();

	if (GET_SINGLE(CPlayerUI)->IsThereOpenWindow())
		m_bCanInterAct = false;

	if (m_bCanInterAct && KEYDOWN('F'))
	{
		Interaction();
	}

	return 0;
}

void CMerchant::LateUpdate()
{
}

void CMerchant::Render(HDC hDC)
{
	HDC hMemDC = m_bDir ? FIND_BITMAP_DC("GiantR") : FIND_BITMAP_DC("GiantL");
	
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx,
		0,
		m_tInfo.icx,
		m_tInfo.icy,
		RGB(255, 0, 255));

	if (m_bCanInterAct)
	{
		hMemDC = FIND_BITMAP_DC("Keyboard_F");
		SIZE tSize = GET_BITMAP_SIZE("Keyboard_F");

		GdiTransparentBlt(hDC,
			static_cast<int>(tPos.fx - tSize.cx * 0.5f * 3.f),
			static_cast<int>(tPos.fy - tSize.cy * 0.5f * 3.f),
			static_cast<int>(tSize.cx * 3.f),
			static_cast<int>(tSize.cy * 3.f),
			hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
	}
		 
#ifdef _DEBUG
	tPos = CScrollManager::WorldToScreen(GetHitBoxWorldInfo());
	CMath::DrawBox(hDC, static_cast<int>(tPos.fx - m_tHitBox.icx / 2), static_cast<int>(tPos.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);
#endif // _DEBUG

}

void CMerchant::Release()
{

}

void CMerchant::Interaction()
{
	//if (KEYDOWN(VK_ESCAPE))
	//{
	//	m_bShowItem = false;
	//	m_pShopWindow->SetIsOn(false);
	//	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	//	pPlayer->SetCanAction(true);
	//}

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
		pConversation->AddTextLine("Hello?");

		CSelectBox* pSelectBox = new CSelectBox;
		pSelectBox->Initialize("", L"", "TalkChoice", L"Image/UI/Rect/TalkChoice.bmp");
		pSelectBox->BoxSetting("Shop", SIZE{ 2, 2 }, 3.f, 2.f, CSelectBox::CENTER, CSelectBox::MIDDLE);
		pSelectBox->SetParentWindow(pConversation);
		pSelectBox->SetChildWindow(m_pShopWindow);
		pConversation->AddSelectBox(pSelectBox);

		pSelectBox = new CSelectBox;
		pSelectBox->Initialize("", L"", "TalkChoice", L"Image/UI/Rect/TalkChoice.bmp");
		pSelectBox->BoxSetting("Nothing", SIZE{ 2, 2 }, 3.f, 2.f, CSelectBox::CENTER, CSelectBox::MIDDLE);
		pSelectBox->SetParentWindow(pConversation);
		pConversation->AddSelectBox(pSelectBox);

		GET_SINGLE(CPlayerUI)->AddWindow(pConversation);
	}

	pConversation->SetIsOn(true);
	GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(false);



}
