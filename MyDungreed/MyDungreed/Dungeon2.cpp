#include "stdafx.h"
#include "Dungeon2.h"
#include "MiniMap.h"
#include "Player.h"
#include "Thorn.h"
#include "BigWhiteSkel.h"
#include "Skeleton.h"
#include "GiantBat.h"
#include "Banshee.h"
#include "Stele.h"
#include "BGImage.h"
#include "TresureBox.h"
#include "Gun.h"
#include "MapWindow.h"
#include "MapRoom.h"
#include "WormPassage.h"

bool CDungeon2::m_bSpawn = false;
bool CDungeon2::m_bTresureBox = false;

CDungeon2::CDungeon2():
	m_pSecondFloor(nullptr),
	m_pFirstFloor(nullptr),
	m_pMiniMap(nullptr),
	m_pWormPassage(nullptr),
	m_bFastMove(false),
	m_iFastMoveDest(0),
	m_dwFastMoveDelay(0),
	m_dwFastMoveDelayMax(0)
{
}


CDungeon2::~CDungeon2()
{
	Release();
}

void CDungeon2::Initialize()
{

	m_strName = "1F : The Jail";

	GET_SINGLE(CMouse)->ChangeCursor(1);
	int iScrollX = TILECX * 30 / 2;
	int iScrollY = TILECY * 20 / 2;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX(TILECX * 33);
	CScrollManager::SetScrollSizeY(TILECY * 25);
	CScrollManager::SetScrollStartX(TILECY * 2);
	CScrollManager::SetScrollStartY(TILECY * 3);

	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/Dungeon2.dat");

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);

	CThorn* pThorn = new CThorn;
	pThorn->Initialize();
	pThorn->SetInfo(830.f, 1310.f);
	pThorn->SetAttack(10.f);
	pThorn->SetThornCount(14);
	pThorn->SetFieldObjectType(FOT_THORN);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pThorn);

	m_pFirstFloor = new CBGImage;
	m_pFirstFloor->Initialize();
	m_pFirstFloor->SetImage("MiddleFloor", L"Image/Dungeon/MiddleFloor.bmp");
	m_pFirstFloor->SetInfo(1600.f, 1378.f, 256, 64);

	m_pSecondFloor = new CBGImage;
	m_pSecondFloor->Initialize();
	m_pSecondFloor->SetImage("MiddleFloor(Long)", L"Image/Dungeon/MiddleFloor(Long).bmp");
	m_pSecondFloor->SetInfo(770.f, 736.f, 512, 64);

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

	CMapRoom* pMapRoom = const_cast<CMapRoom*>(pMapWindow->FindRoom(SC_STAGE2));

	if (!pMapRoom)
	{
		pMapRoom = new CMapRoom;
		pMapRoom->Initialize();
		pMapRoom->SetStage(SC_STAGE2);
		pMapRoom->SetRatio(4.f);
		pMapRoom->SetPassagePos(1600.f, 640.f);
		pMapRoom->SetLink(Flag_Link_Right | Flag_Link_Up | Flag_Link_Down);
		pMapWindow->AddRoom(pMapRoom, CMapWindow::RIGHT);
	}
	pMapWindow->SetCurStage(pMapRoom);

	m_pWormPassage = new CWormPassage;
	m_pWormPassage->Initialize();
	m_pWormPassage->SetInfo(1600.f, 640.f);
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
		//	임시
		//pPlayer->SetCanAction(true);
		//pPlayer->SetBoolRender(true);
	}

}

int CDungeon2::Update()
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

	if (!m_bSpawn &&
		pPlayer->GetInfo().fx >= 400)
	{
		SpawnMonster();
	}


	if (KEYDOWN(VK_F1))
	{
		SpawnMonster();
	}

	if (GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER)->empty())
	{
		//	문 열기
		list<CObj*>* pFieldObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_FIELD_OBJECT);
		for (auto& pObj : *pFieldObjList)
		{
			CFieldObject* pFieldObj = dynamic_cast<CFieldObject*>(pObj);

			if (pFieldObj->GetFieldObjectType() == FOT_STELE)
				dynamic_cast<CStele*>(pFieldObj)->StateChange(STELE_OPEN);
		}

		//	보물 상자 넣기
		if (m_bSpawn && !m_bTresureBox)
		{
			CTresureBox* pBox = new CTresureBox;

			pBox->Initialize();
			pBox->SetInfo(318.f, 1313.f);
			pBox->SetBox(23, 18, 4.f);
			pBox->SetCloseImage("BasicTresureClosed", L"Image/Tresure/BasicTresureClosed.bmp");
			pBox->SetOpenImage("BasicTresureOpened", L"Image/Tresure/BasicTresureOpened.bmp");

			CGun* pGun = new CGun;
			pGun->Initialize();
			pGun->SetAttack(15);
			pGun->SetName("Revolver");
			pGun->SetIconImage("RevolverIcon", L"Image/Weapon/Gun/Revolver0.bmp");
			pGun->SetImage("Revolver", L"Image/Weapon/Gun/Revolver0.bmp");
			pGun->SetLeftImage("RevolverL", L"Image/Weapon/Gun/Revolver0L.bmp");

			pBox->SetItem(pGun);
			GET_SINGLE(CObjectManager)->AddObj(OT_TRESURE_BOX, pBox);

			m_bTresureBox = true;
		}
	}



	if (KEYDOWN(VK_F11))
	{
		pPlayer->SetInfo(235.f, 1280.f);
		return SC_BOSS1;
	}

	if (KEYDOWN(VK_F10))
	{
		pPlayer->SetInfo(350.f, 1280.f);
		return SC_STAGE3;
	}

	if (pPlayer->GetInfo().fy <= 250.f)
	{
		pPlayer->SetInfo(350.f, 1280.f);
		return SC_STAGE3;
	}

	if (pPlayer->GetInfo().fx <= 200.f)
	{
		pPlayer->SetInfo(2400.f, 512.f);
		return SC_STAGE1;
	}

	if (pPlayer->GetInfo().fx >= 2030.f)
	{
		pPlayer->SetInfo(235.f, 1280.f);
		return SC_BOSS1;
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

	return 0;
}

void CDungeon2::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CDungeon2::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->Render(hDC);

	if (m_pSecondFloor)
		m_pSecondFloor->Render(hDC);
	if (m_pFirstFloor)
		m_pFirstFloor->Render(hDC);

	GET_SINGLE(CEffectManager)->FirstRender(hDC);
	GET_SINGLE(CObjectManager)->Render(hDC);
	GET_SINGLE(CEffectManager)->Render(hDC);
	//m_pMiniMap->Render(hDC);

	if (!GET_SINGLE(CResult)->GetShowResult())
	{
		GET_SINGLE(CPlayerUI)->Render(hDC);
		m_pMiniMap->Render(hDC);
		GET_SINGLE(CInventory)->Render(hDC);
	}
}

void CDungeon2::Release()
{
	DESTROY_SINGLE(CTileManager);
	SAFE_DELETE(m_pFirstFloor);
	SAFE_DELETE(m_pSecondFloor);
	SAFE_DELETE(m_pMiniMap);
	ObjectListClear();
}

void CDungeon2::SpawnMonster()
{
	{
		CBigWhiteSkel* pMonster = new CBigWhiteSkel;
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 715.f, 620.f);
	}

	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_SHORT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1200.f, 620.f);
	}

	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_GREAT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 951.f, 620.f);
	}

	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_BOW);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 708.f, 870.f);

		pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_BOW);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 818.f, 870.f);

		pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_BOW);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1149.f, 940.f);
	}

	{
		CGiantBat* pMonster = new CGiantBat;
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1596.f, 610.f);
	}

	CStele* pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_VL);
	pStele->SetInfo(273.f, 575.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_H);
	pStele->SetInfo(512.f, 360.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_H);
	pStele->SetInfo(1600.f, 1454.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);


	pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_VR);
	pStele->SetInfo(1950.f, 1216.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	m_bSpawn = true;

}
