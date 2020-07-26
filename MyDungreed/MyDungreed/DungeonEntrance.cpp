#include "stdafx.h"
#include "DungeonEntrance.h"
#include "MiniMap.h"
#include "Player.h"
#include "AnimatedBGObject.h"
#include "BigWhiteSkel.h"
#include "Skeleton.h"
#include "GiantBat.h"
#include "UIWindow.h"
#include "MapWindow.h"
#include "MapRoom.h"
#include "WormPassage.h"

CDungeonEntrance::CDungeonEntrance():
	m_pDoor(nullptr),
	m_pMiniMap(nullptr),
	m_pWormPassage(nullptr),
	m_bFastMove(false),
	m_iFastMoveDest(0),
	m_dwFastMoveDelay(0),
	m_dwFastMoveDelayMax(0)
{
}


CDungeonEntrance::~CDungeonEntrance()
{
	Release();
}

void CDungeonEntrance::Initialize()
{
	m_strName = "1F : The Jail";
	
	GET_SINGLE(CMouse)->ChangeCursor(1);
	int iScrollX = TILECX * 30 / 2;
	int iScrollY = TILECY * 20 / 2;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX(TILECX * 30);
	CScrollManager::SetScrollSizeY(TILECY * 20);
	CScrollManager::SetScrollStartX(static_cast<int>(TILECX * 2.f));
	CScrollManager::SetScrollStartY(static_cast<int>(TILECY * 2.f));

	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/DungeonEnter.dat");
	
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);

	m_pDoor = new CAnimatedBGObject;
	m_pDoor->Initialize();
	m_pDoor->SetInfo(700.f, 638.f);
	m_pDoor->SetImage("Door", L"Image/BGObject/Door/Door.bmp");
	m_pDoor->SetAnimationType(AT_ONCE);
	m_pDoor->SetMaxFrame(10);
	m_pDoor->SetTotalFrame(10);
	m_pDoor->SetRatio(4.f);

	GET_SINGLE(CResult)->StartPlay();

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

	CMapRoom* pMapRoom = const_cast<CMapRoom*>(pMapWindow->FindRoom(SC_DUNGEON_ENTRANCE));

	if (!pMapRoom)
	{
		pMapRoom = new CMapRoom;
		pMapRoom->Initialize();
		pMapRoom->SetStage(SC_DUNGEON_ENTRANCE);
		pMapRoom->SetRatio(4.f);
		pMapRoom->SetLink(Flag_Link_Right);
		pMapRoom->SetPassagePos(1400.f, 704.f);
		pMapWindow->AddRoom(pMapRoom, CMapWindow::END);
	}
	pMapWindow->SetCurStage(pMapRoom);

	m_pWormPassage = new CWormPassage;
	m_pWormPassage->Initialize();
	m_pWormPassage->SetInfo(1400.f, 704.f);
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
	}
}

int CDungeonEntrance::Update()
{
	m_pDoor->Update();

	GET_SINGLE(CInventory)->Update();
	GET_SINGLE(CObjectManager)->Update();
	GET_SINGLE(CEffectManager)->Update();
	int iUIRet = GET_SINGLE(CPlayerUI)->Update();

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (m_pWormPassage->GetDisgorgeEnd())
	{
		m_pWormPassage->SetDisgorgeEnd(false);
		pPlayer->SetBoolRender(true);
		pPlayer->SetCanAction(true);
		
	}



	if (KEYDOWN(VK_F11))
	{
		pPlayer->SetInfo(400.f, 512.f);
		return SC_STAGE1;
	}

	if (pPlayer->GetInfo().fx >= 1850.f)
	{
		pPlayer->SetInfo(400.f, 512.f);
		return SC_STAGE1;
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

void CDungeonEntrance::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CDungeonEntrance::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->Render(hDC);

	m_pDoor->Render(hDC);

	GET_SINGLE(CEffectManager)->FirstRender(hDC);
	GET_SINGLE(CObjectManager)->Render(hDC);
	GET_SINGLE(CEffectManager)->Render(hDC);
	m_pMiniMap->Render(hDC);

	if (!GET_SINGLE(CResult)->GetShowResult())
	{
		GET_SINGLE(CPlayerUI)->Render(hDC);
		GET_SINGLE(CInventory)->Render(hDC);
	}
}

void CDungeonEntrance::Release()
{
	SAFE_DELETE(m_pDoor);
	SAFE_DELETE(m_pMiniMap);
	ObjectListClear();
	DESTROY_SINGLE(CTileManager);
}
