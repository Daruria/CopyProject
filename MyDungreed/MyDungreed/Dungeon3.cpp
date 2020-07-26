#include "stdafx.h"
#include "Dungeon3.h"
#include "MiniMap.h"
#include "Player.h"
#include "BigWhiteSkel.h"
#include "Skeleton.h"
#include "GiantBat.h"
#include "Banshee.h"
#include "Stele.h"
#include "BGImage.h"
#include "MapWindow.h"
#include "MapRoom.h"

bool CDungeon3::m_bSpawn = false;

CDungeon3::CDungeon3():
	m_Floor(nullptr),
	m_pMiniMap(nullptr)
{
}


CDungeon3::~CDungeon3()
{
	Release();
}

void CDungeon3::Initialize()
{
	m_strName = "1F : The Jail";

	GET_SINGLE(CMouse)->ChangeCursor(1);
	int iScrollX = TILECX * 30 / 2;
	int iScrollY = TILECY * 20 / 2;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX(TILECX * 34);
	CScrollManager::SetScrollSizeY(TILECY * 25);
	CScrollManager::SetScrollStartX(static_cast<int>(TILECX * 2.f));
	CScrollManager::SetScrollStartY(static_cast<int>(TILECY * 2.f));

	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/Dungeon3.dat");

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);

	m_Floor = new CBGImage;
	m_Floor->Initialize();
	m_Floor->SetInfo(385.f, 1376.f, 256, 64);
	m_Floor->SetImage("Floor", L"Image/Dungeon/MiddleFloor.bmp");

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

	CMapRoom* pMapRoom = const_cast<CMapRoom*>(pMapWindow->FindRoom(SC_STAGE3));

	if (!pMapRoom)
	{
		pMapRoom = new CMapRoom;
		pMapRoom->Initialize();
		pMapRoom->SetStage(SC_STAGE3);
		pMapRoom->SetRatio(4.f);
		pMapRoom->SetLink(Flag_Link_Right);
		pMapWindow->AddRoom(pMapRoom, CMapWindow::UP);
	}
	pMapWindow->SetCurStage(pMapRoom);
}

int CDungeon3::Update()
{
	GET_SINGLE(CInventory)->Update();
	GET_SINGLE(CObjectManager)->Update();
	GET_SINGLE(CEffectManager)->Update();
	int iUIRet = GET_SINGLE(CPlayerUI)->Update();

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();


	if (!m_bSpawn && pPlayer->GetInfo().fx >= 400.f)
	{
		SpawnMonster();
	}

	if (GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER)->empty())
	{
		list<CObj*>* pFieldObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_FIELD_OBJECT);
		for (auto& pObj : *pFieldObjList)
		{
			CFieldObject* pFieldObj = dynamic_cast<CFieldObject*>(pObj);

			if (pFieldObj->GetFieldObjectType() == FOT_STELE)
				dynamic_cast<CStele*>(pFieldObj)->StateChange(STELE_OPEN);
		}
	}


	if (KEYDOWN(VK_F11))
	{
		pPlayer->SetInfo(350.f, 1280.f);
		return SC_SHOP;
	}


	if (pPlayer->GetInfo().fy >= 1500.f)
	{
		pPlayer->SetInfo(512.f, 512.f);
		return SC_STAGE2;
	}

	if (pPlayer->GetInfo().fx >= 2100.f)
	{
		pPlayer->SetInfo(350.f, 1280.f);
		return SC_SHOP;
	}

	int iResult = GET_SINGLE(CResult)->Update();

	if (iUIRet)
		return iUIRet;

	if (iResult)
		return iResult;

	return 0;
}

void CDungeon3::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CDungeon3::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->Render(hDC);
	m_Floor->Render(hDC);

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

void CDungeon3::Release()
{
	DESTROY_SINGLE(CTileManager);
	SAFE_DELETE(m_Floor);
	SAFE_DELETE(m_pMiniMap);
	ObjectListClear();
}

void CDungeon3::SpawnMonster()
{
	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_SHORT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 640.f, 1210.f);

		pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_SHORT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 824.f, 1015.f);
	}

	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_GREAT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1222.f, 913.f);

		pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_GREAT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1456.f, 870.f);
	}

	{
		CBanshee* pMonster = new CBanshee;
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 459.f, 869.f);
	}

	CStele* pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_H);
	pStele->SetInfo(386.f, 1457.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_VR);
	pStele->SetInfo(2000.f, 760.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	m_bSpawn = true;
}
