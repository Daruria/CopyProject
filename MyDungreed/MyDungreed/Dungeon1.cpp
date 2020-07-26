#include "stdafx.h"
#include "Dungeon1.h"
#include "MiniMap.h"
#include "Player.h"
#include "BigWhiteSkel.h"
#include "Skeleton.h"
#include "GiantBat.h"
#include "Banshee.h"
#include "Stele.h"
#include "MapWindow.h"
#include "MapRoom.h"

bool CDungeon1::m_bSpawn = false;

CDungeon1::CDungeon1():
	m_pMiniMap(nullptr)
{
}


CDungeon1::~CDungeon1()
{
	Release();
}

void CDungeon1::Initialize()
{
	m_strName = "1F : The Jail";

	GET_SINGLE(CMouse)->ChangeCursor(1);
	int iScrollX = TILECX * 30 / 2;
	int iScrollY = TILECY * 20 / 2;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX(TILECX * 40);
	CScrollManager::SetScrollSizeY(TILECY * 20);
	CScrollManager::SetScrollStartX(static_cast<int>(TILECX * 2.f));
	CScrollManager::SetScrollStartY(static_cast<int>(TILECY * 2.f));

	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/Dungeon1.dat");

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);

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

	CMapRoom* pMapRoom = const_cast<CMapRoom*>(pMapWindow->FindRoom(SC_STAGE1));

	if (!pMapRoom)
	{
		pMapRoom = new CMapRoom;
		pMapRoom->Initialize();
		pMapRoom->SetStage(SC_STAGE1);
		pMapRoom->SetRatio(4.f);
		pMapRoom->SetLink(Flag_Link_Right);
		pMapWindow->AddRoom(pMapRoom, CMapWindow::RIGHT);
	}
	pMapWindow->SetCurStage(pMapRoom);
}

int CDungeon1::Update()
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
		pPlayer->SetInfo(335.f, 640.f);
		return SC_STAGE2;
	}

	
	if (pPlayer->GetInfo().fx <= 270.f)
	{
		pPlayer->SetInfo(1800.f, 704.f);
		return SC_DUNGEON_ENTRANCE;
	}

	if (pPlayer->GetInfo().fx >= 2500.f)
	{
		pPlayer->SetInfo(335.f, 640.f);
		return SC_STAGE2;
	}

	int iResult = GET_SINGLE(CResult)->Update();

	if (iUIRet)
		return iUIRet;

	if (iResult)
		return iResult;

	return SC_NONE;
}

void CDungeon1::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CDungeon1::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->Render(hDC);

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

void CDungeon1::Release()
{
	DESTROY_SINGLE(CTileManager);
	SAFE_DELETE(m_pMiniMap);
	ObjectListClear();
}

void CDungeon1::SpawnMonster()
{
	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_SHORT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 665.f, 535.f);

		pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_SHORT);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 990.f, 535.f);
	}

	{
		CSkeleton* pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_BOW);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1355.f, 535.f);

		pMonster = new CSkeleton;
		pMonster->SetSkelType(ST_BOW);
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 1780.f, 535.f);
	}

	{
		CGiantBat* pMonster = new CGiantBat;
		GET_SINGLE(CObjectManager)->AddMonster(pMonster, 928.f, 373.f);
	}

	CStele* pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_VL);
	pStele->SetInfo(228.f, 448.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	pStele = new CStele;
	pStele->Initialize();
	pStele->SetSteleType(STELE_VR);
	pStele->SetInfo(2356.f, 448.f);
	pStele->StateChange(STELE_CLOSE);
	pStele->SetFieldObjectType(FOT_STELE);
	GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

	m_bSpawn = true;
}
