#include "stdafx.h"
#include "DungeonShop.h"
#include "MiniMap.h"
#include "Player.h"
#include "Merchant.h"
#include "BGImage.h"
#include "MapRoom.h"
#include "MapWindow.h"
#include "WormPassage.h"

CDungeonShop::CDungeonShop():
	m_pDisplay(nullptr),
	m_pMiniMap(nullptr)
{
}


CDungeonShop::~CDungeonShop()
{
	Release();
}

void CDungeonShop::Initialize()
{
	m_strName = "1F : Dungeon Shop";

	GET_SINGLE(CMouse)->ChangeCursor(1);
	int iScrollX = TILECX * 30 / 2;
	int iScrollY = TILECY * 20 / 2;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX(TILECX * 34);
	CScrollManager::SetScrollSizeY(TILECY * 25);
	CScrollManager::SetScrollStartX(static_cast<int>(TILECX * 3.f));
	CScrollManager::SetScrollStartY(static_cast<int>(TILECY * 2.f));

	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/DungoenShop.dat");

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);

	CMerchant* pMerchant = new CMerchant;
	pMerchant->Initialize();
	pMerchant->SetInfo(1269.f, 1440.f);
	pMerchant->SetKey("1F : Dungeon Shop Merchant");
	GET_SINGLE(CObjectManager)->AddObj(OT_NPC, pMerchant);

	m_pDisplay = new CBGImage;
	m_pDisplay->Initialize();
	m_pDisplay->SetInfo(1000.f, 1482.f, 101, 27);
	m_pDisplay->SetRatio(4.f);
	m_pDisplay->SetImage("InDungeonShop", L"Image/Shop/InDungeonShop.bmp");

	m_pMiniMap = new CMiniMap;
	m_pMiniMap->Initialize();

	CMapWindow* pMapWindow = dynamic_cast<CMapWindow*>(GET_SINGLE(CPlayerUI)->FindUIWindow("Map"));
	if (!pMapWindow)
	{
		pMapWindow = new CMapWindow;
		pMapWindow->Initialize();
		pMapWindow->SetIsOn(false);
		GET_SINGLE(CPlayerUI)->AddWindow(pMapWindow);
	}

	CMapRoom* pMapRoom = const_cast<CMapRoom*>(pMapWindow->FindRoom(SC_SHOP));

	if (!pMapRoom)
	{
		pMapRoom = new CMapRoom;
		pMapRoom->Initialize();
		pMapRoom->SetStage(SC_SHOP);
		pMapRoom->SetRatio(4.f);
		pMapRoom->SetPassagePos(1280.f, 1024.f);
		pMapWindow->AddRoom(pMapRoom, CMapWindow::RIGHT);
	}
	pMapWindow->SetCurStage(pMapRoom);

	m_pWormPassage = new CWormPassage;
	m_pWormPassage->Initialize();
	m_pWormPassage->SetInfo(1280.f, 1024.f);
	m_pWormPassage->SetRatio(4.f);
	GET_SINGLE(CObjectManager)->AddObj(OT_NPC, m_pWormPassage);
	pMapRoom->AddWormIcon();

	m_dwFastMoveDelayMax = 1000;

	if (pMapWindow->GetPrevFastMove())
	{
		pPlayer->SetInfo(pMapWindow->GetFastMoveDest().fx, pMapWindow->GetFastMoveDest().fy);
		pMapWindow->SetPrevFastMove(false);
		m_pWormPassage->Disgorge();
		CScrollManager::SetScrollX(pMapWindow->GetFastMoveDest().fx);
		CScrollManager::SetScrollY(pMapWindow->GetFastMoveDest().fy);
		//	юс╫ц
		//pPlayer->SetCanAction(true);
		//pPlayer->SetBoolRender(true);
	}

}

int CDungeonShop::Update()
{
	GET_SINGLE(CInventory)->Update();
	GET_SINGLE(CObjectManager)->Update();
	GET_SINGLE(CEffectManager)->Update();
	int iUIRet = GET_SINGLE(CPlayerUI)->Update();

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (m_pWormPassage->GetDisgorgeEnd())
	{
		//GET_SINGLE(CEffectManager)->AddEffect("DieFX", L"Image/Effect/Die/DieFX.bmp",
		//	m_pWormPassage->GetInfo().fx, m_pWormPassage->GetInfo().fx, 256, 256, 11);
		pPlayer->SetBoolRender(true);
		pPlayer->SetCanAction(true);
		m_pWormPassage->SetDisgorgeEnd(false);
	}

	if (pPlayer->GetInfo().fx <= TILECX * 5.f)
	{
		pPlayer->SetInfo(2000.f, 832.f);
		return SC_STAGE3;
	}


	int iResult = GET_SINGLE(CResult)->Update();

	if (iUIRet)
	{
		m_bFastMove = true;
		m_pWormPassage->Eat();
		m_iFastMoveDest = iUIRet;
		m_dwFastMoveDelay = GetTickCount();
	}

	if (m_bFastMove)
	{
		if (m_dwFastMoveDelay + m_dwFastMoveDelayMax < GetTickCount())
			return m_iFastMoveDest;
	}

	if (iResult)
		return iResult;

	return SC_NONE;
}

void CDungeonShop::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CDungeonShop::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->Render(hDC);
	m_pDisplay->Render(hDC);
	GET_SINGLE(CEffectManager)->FirstRender(hDC);
	GET_SINGLE(CObjectManager)->Render(hDC);
	GET_SINGLE(CEffectManager)->Render(hDC);

	if (!GET_SINGLE(CResult)->GetShowResult())
	{
		GET_SINGLE(CPlayerUI)->Render(hDC);
		m_pMiniMap->Render(hDC);
		GET_SINGLE(CInventory)->Render(hDC);
	}
	
}

void CDungeonShop::Release()
{
	DESTROY_SINGLE(CTileManager);
	SAFE_DELETE(m_pDisplay);
	SAFE_DELETE(m_pMiniMap);
	ObjectListClear();
}
