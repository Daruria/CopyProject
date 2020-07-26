#include "stdafx.h"
#include "Town.h"
#include "BGObject.h"
#include "BGImage.h"
#include "Player.h"
#include "BigWhiteSkel.h"
#include "Skeleton.h"
#include "GiantBat.h"
#include "Mouse.h"
#include "MiniMap.h"
#include "AnimatedBGObject.h"
#include "Banshee.h"
#include "NPC.h"
#include "Merchant.h"
#include "Commander.h"

CTown::CTown():
	m_pBGSky(nullptr),
	m_pBGMountain(nullptr),
	m_pBGTrees(nullptr),
	
	m_pSecondFloor1(nullptr),
	m_pSecondFloor2(nullptr),
	m_pSecondFloor3(nullptr),
	
	m_pBrokenHouse(nullptr),
	m_pBrokenTemple(nullptr),
	
	m_pSkulSignL(nullptr),
	m_pSkulSignR(nullptr),
	
	m_pMiddleFloor1(nullptr),
	m_pMiddleFloor2(nullptr),
	m_pMiddleFloor3(nullptr),
	m_pMiddleFloor4(nullptr),
	m_pMiddleFloor5(nullptr),
	m_pMiddleFloor6(nullptr),
	
	m_pMiniMap(nullptr),

	m_pEatDungeon(nullptr),
	m_dwReturnToTown(0),
	m_bDunEatSound(false)
{
}


CTown::~CTown()
{
	Release();
}

void CTown::EatDungeonInit()
{
	CPlayer * pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	m_pEatDungeon = new CAnimatedBGObject;
	m_pEatDungeon->Initialize();
	m_pEatDungeon->SetAnimationType(AT_ONCE);
	m_pEatDungeon->SetInfo(pPlayer->GetInfo().fx, pPlayer->GetInfo().fy - 105);
	m_pEatDungeon->SetImage("EatDungeon", L"Image/BGObject/DungeonEat/DungeonEat.bmp");
	m_pEatDungeon->SetMaxFrame(28);
	m_pEatDungeon->SetTotalFrame(28);
	m_pEatDungeon->SetRatio(4.f);
	m_pEatDungeon->SetTimePerFrame(50);
	GET_SINGLE(CSoundMgr)->PlaySound(L"DungeonOut.wav", CSoundMgr::EFFECT);
}

void CTown::Initialize()
{
	GET_SINGLE(CSoundMgr)->PlayBGM(L"Town.wav");
	GET_SINGLE(CSoundMgr)->SetVolume(CSoundMgr::BGM, 0.4f);

	GET_SINGLE(CMouse)->ChangeCursor(1);
	
	int iScrollX = TILEX * TILECX / 2;
	int iScrollY = WINCY;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX((TILEX - 12) * TILECX);
	CScrollManager::SetScrollSizeY(TILECY * 24);
	CScrollManager::SetScrollStartX(static_cast<int>(TILECX * 2.f));
	CScrollManager::SetScrollStartY(static_cast<int>(TILECY * 2.f));

	m_tCenterPos.fx = (float)CScrollManager::GetScrollSizeX() / 2;
	m_tCenterPos.fy = (float)CScrollManager::GetScrollSizeY();

	ImageInitialize();

	INSERT_BITMAP("OptionMapTile", L"Image/OptionMapTile(64).bmp");
	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/Town.dat");
	
	int iSecondFloor2X = int(TILECX * TILEX / 2 - 5.5f * TILECX);
	int iSecondFloor2Y = CScrollManager::GetScrollSizeY() - TILECY * 3 - (TILECY / 2) * 7;

	PlayerSetting();

	CScrollManager::SetScrollX(static_cast<float>(iSecondFloor2X));
	CScrollManager::SetScrollY((iSecondFloor2Y - TILECY * (4.4f)));

	GET_SINGLE(CEffectManager)->AddEffect("ReturnToTownRevive", L"Image/Effect/ReturnToTown/ReturnToTownRevive.bmp",
		static_cast<float>(iSecondFloor2X), (iSecondFloor2Y - TILECY * (5.5f) + 4.f), 69, 76, 17, 4.f);

	m_dwReturnToTown = GetTickCount();

	m_strName = "Village";
	GET_SINGLE(CPlayerUI)->StageTitleComeIn(m_strName);
	
	m_pMiniMap = new CMiniMap;
	m_pMiniMap->Initialize();

	m_pEatDungeon = nullptr;

	list<CObj*>* pNpcList = GET_SINGLE(CObjectManager)->GetObjList(OT_NPC);

	for (auto pNPC : *pNpcList)
		pNPC->SetLife(false);

	CCommander* pCommander = new CCommander;
	pCommander->Initialize();
	pCommander->SetInfo(891.f, 1293.f);
	GET_SINGLE(CObjectManager)->AddObj(OT_NPC, pCommander);
	

}

int CTown::Update()
{
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (!pPlayer->GetBoolRender() && m_dwReturnToTown + 900 <= GetTickCount())
	{
		pPlayer->SetBoolRender(true);
		pPlayer->SetCanAction(true);
	}

	if (m_pEatDungeon)
	{
		if (m_pEatDungeon->Update())
		{
			pPlayer->SetInfo(700.f, 700.f);

			GET_SINGLE(CSoundMgr)->StopSound(CSoundMgr::EFFECT);
			GET_SINGLE(CSoundMgr)->StopAll();

			GET_SINGLE(CSoundMgr)->PlayBGM(L"JailField.wav");
			GET_SINGLE(CSoundMgr)->SetVolume(CSoundMgr::BGM, 0.4f);
			GET_SINGLE(CPlayerUI)->StageTitleComeIn("1F : The Jail");

			return SC_DUNGEON_ENTRANCE;
		}
		GET_SINGLE(CEffectManager)->Update();
		return SC_NONE;
	}

	float fdx = CScrollManager::GetScrollX() - m_tCenterPos.fx;
	float fdy = CScrollManager::GetScrollY() - m_tCenterPos.fy;

	m_pBGMountain->SetInfofx( - fdx * 0.1f);
	m_pBGMountain->SetInfofy( - fdy * 0.2f);

	m_pBGTrees->SetInfofx( - fdx * 0.2f);
	m_pBGTrees->SetInfofy( - fdy * 0.35f);

	if (KEYDOWN(VK_F11))
	{
		pPlayer->SetInfo(700.f, 700.f);
		return SC_DUNGEON_ENTRANCE;
	}
	
	GET_SINGLE(CInventory)->Update();
	GET_SINGLE(CObjectManager)->Update();
	GET_SINGLE(CEffectManager)->Update();
	GET_SINGLE(CPlayerUI)->Update();

	INFO tInfo = pPlayer->GetInfo();

	if (tInfo.fx >= 3400 && tInfo.fx <= 4200 && tInfo.fy >= 1278)
	{
		EatDungeonInit();
	}

	int iResult = GET_SINGLE(CResult)->Update();
	if (iResult)
		return iResult;

	return SC_NONE;
}

void CTown::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CTown::Render(HDC hDC)
{
	m_pBGSky->Render(hDC);
	m_pBGMountain->Render(hDC);
	m_pBGTrees->Render(hDC);

	m_pSecondFloor1->Render(hDC);
	m_pSecondFloor2->Render(hDC);
	m_pSecondFloor3->Render(hDC);

	m_pMiddleFloor1->Render(hDC);
	m_pMiddleFloor2->Render(hDC);
	m_pMiddleFloor3->Render(hDC);
	m_pMiddleFloor4->Render(hDC);
	m_pMiddleFloor5->Render(hDC);
	m_pMiddleFloor6->Render(hDC);

	m_pBrokenTemple->Render(hDC);
	m_pBrokenHouse->Render(hDC);

	m_pSkulSignL->Render(hDC);
	m_pSkulSignR->Render(hDC);



	GET_SINGLE(CTileManager)->Render(hDC);

	GET_SINGLE(CEffectManager)->FirstRender(hDC);

	if (m_pEatDungeon)
	{
		m_pEatDungeon->Render(hDC);
		if (m_pEatDungeon->GetCurFrame() < 9)
			GET_SINGLE(CObjectManager)->Render(hDC);
		else
		{
			if (!m_bDunEatSound)
			{
				m_bDunEatSound = true;
				GET_SINGLE(CSoundMgr)->PlaySound(L"synthetic_explosion_1.wav", CSoundMgr::EFFECT);
				GET_SINGLE(CSoundMgr)->PlaySound(L"DungeonOut.wav", CSoundMgr::EFFECT);
			}
		}
		
	}
	else
		GET_SINGLE(CObjectManager)->Render(hDC);
	
	GET_SINGLE(CEffectManager)->Render(hDC);
	m_pMiniMap->Render(hDC);

	GET_SINGLE(CPlayerUI)->Render(hDC);
	GET_SINGLE(CInventory)->Render(hDC);

}

void CTown::Release()
{
	//GET_SINGLE(CSoundMgr)->StopAll();
	SAFE_DELETE(m_pEatDungeon);
	SAFE_DELETE(m_pMiniMap);

	SAFE_DELETE(m_pMiddleFloor1);
	SAFE_DELETE(m_pMiddleFloor2);
	SAFE_DELETE(m_pMiddleFloor3);
	SAFE_DELETE(m_pMiddleFloor4);
	SAFE_DELETE(m_pMiddleFloor5);
	SAFE_DELETE(m_pMiddleFloor6);

	SAFE_DELETE(m_pSkulSignL);
	SAFE_DELETE(m_pSkulSignR);

	SAFE_DELETE(m_pBrokenHouse);
	SAFE_DELETE(m_pBrokenTemple)

	SAFE_DELETE(m_pSecondFloor1);
	SAFE_DELETE(m_pSecondFloor2);
	SAFE_DELETE(m_pSecondFloor3);

	SAFE_DELETE(m_pBGSky);
	SAFE_DELETE(m_pBGMountain);
	SAFE_DELETE(m_pBGTrees);

	ObjectListClear();
	//DESTROY_SINGLE(CInventory);
	//DESTROY_SINGLE(CPlayerUI);
	//DESTROY_SINGLE(CEffectManager);
	DESTROY_SINGLE(CTileManager);
}

void CTown::ImageInitialize()
{

	m_pBGSky = new CBGObject;
	m_pBGSky->Initialize();
	m_pBGSky->SetImage("SkyDay", L"Image/Town/SkyDay.bmp");
	m_pBGSky->SetInfo(0.f, -48.f, 1280, 720);
	m_pBGSky->SetInterval(0);

	m_pBGMountain = new CBGObject;
	m_pBGMountain->Initialize();
	m_pBGMountain->SetImage("Mountain", L"Image/Town/TownBGDay.bmp");
	m_pBGMountain->SetInfo(0.f, 76.f, 1280, 568);
	m_pBGMountain->SetInterval(0);

	m_pBGTrees = new CBGObject;
	m_pBGTrees->Initialize();
	m_pBGTrees->SetImage("Trees", L"Image/Town/TownLayerDay.bmp");
	m_pBGTrees->SetInfo(0.f, 300.f, 1280, 380);
	m_pBGTrees->SetInterval(0);

	m_pSecondFloor1 = new CBGImage;
	m_pSecondFloor1->SetImage("SecondFloor3", L"Image/Town/SecondFloor3.bmp");
	int iSecondFloor1X = int(TILECX * TILEX / 2 - TILECX * (52.f + 2 / 3.f));
	int iSecondFloor1Y = CScrollManager::GetScrollSizeY() - TILECY * 3 - (TILECY / 2) * 9;
	m_pSecondFloor1->SetInfo((float)iSecondFloor1X, (float)iSecondFloor1Y, 2776, 576);

	m_pSecondFloor2 = new CBGImage;
	m_pSecondFloor2->SetImage("SecondFloor2", L"Image/Town/SecondFloor2.bmp");
	int iSecondFloor2X = int(TILECX * TILEX / 2 - 5.5f * TILECX);
	int iSecondFloor2Y = CScrollManager::GetScrollSizeY() - TILECY * 3 - (TILECY / 2) * 7;
	m_pSecondFloor2->SetInfo((float)iSecondFloor2X, (float)iSecondFloor2Y, 2752, 448);

	m_pSecondFloor3 = new CBGImage;
	m_pSecondFloor3->SetImage("SecondFloor3", L"Image/Town/SecondFloor3.bmp");
	int iSecondFloor3X = int(TILECX * TILEX / 2 + TILECX * (40.f + 2 / 3.f));
	int iSecondFloor3Y = CScrollManager::GetScrollSizeY() - TILECY * 3 - (TILECY / 2) * 9;
	m_pSecondFloor3->SetInfo((float)iSecondFloor3X, (float)iSecondFloor3Y, 2776, 576);

	m_pBrokenHouse = new CBGImage;
	m_pBrokenHouse->SetImage("BrokenHouse0", L"Image/Town/BrokenHouse0.bmp");
	m_pBrokenHouse->SetInfo(iSecondFloor1X + TILECX * 1.5f, iSecondFloor1Y - TILECY * 5.75f, 744, 160);

	m_pBrokenTemple = new CBGImage;
	m_pBrokenTemple->SetImage("BrokenTemple", L"Image/Town/BrokenTemple.bmp");
	m_pBrokenTemple->SetInfo((float)iSecondFloor2X, (iSecondFloor2Y - TILECY * (0.2f + 6.f)), 1020, 348);

	m_pSkulSignL = new CBGImage;
	m_pSkulSignL->SetImage("SkulSignL", L"Image/Town/SkulSignL(Big).bmp");
	m_pSkulSignL->SetInfo(iSecondFloor2X - TILECX * 7.5f, iSecondFloor2Y + TILECY * 2.9f, 84, 87);

	m_pSkulSignR = new CBGImage;
	m_pSkulSignR->SetImage("SkulSignR", L"Image/Town/SkulSignR(Big).bmp");
	m_pSkulSignR->SetInfo(iSecondFloor2X + TILECX * 8.f, iSecondFloor2Y + TILECY * 2.9f, 84, 87);

	m_pMiddleFloor1 = new CBGImage;
	m_pMiddleFloor1->SetImage("MiddleFloor", L"Image/Town/MiddleFloor.bmp");
	m_pMiddleFloor1->SetInfo(iSecondFloor1X + TILECX * (9.7f), (float)iSecondFloor1Y - TILECY, 256, 64);

	m_pMiddleFloor2 = new CBGImage;
	m_pMiddleFloor2->SetImage("MiddleFloor", L"Image/Town/MiddleFloor.bmp");
	m_pMiddleFloor2->SetInfo(iSecondFloor1X + TILECX * 13.7f, (float)iSecondFloor1Y + TILECY, 256, 64);

	m_pMiddleFloor3 = new CBGImage;
	m_pMiddleFloor3->SetImage("MiddleFloor", L"Image/Town/MiddleFloor.bmp");
	m_pMiddleFloor3->SetInfo(iSecondFloor1X + TILECX * 11.7f, iSecondFloor1Y + TILECY * 3.f, 256, 64);

	m_pMiddleFloor4 = new CBGImage;
	m_pMiddleFloor4->SetImage("MiddleFloor", L"Image/Town/MiddleFloor.bmp");
	m_pMiddleFloor4->SetInfo(iSecondFloor2X - TILECX * 0.48f, static_cast<float>(iSecondFloor2Y), 256, 64);

	m_pMiddleFloor5 = new CBGImage;
	m_pMiddleFloor5->SetImage("MiddleFloor", L"Image/Town/MiddleFloor.bmp");
	m_pMiddleFloor5->SetInfo(iSecondFloor2X - TILECX * 3.48f, iSecondFloor2Y + TILECY * 2.f, 256, 64);

	m_pMiddleFloor6 = new CBGImage;
	m_pMiddleFloor6->SetImage("MiddleFloor", L"Image/Town/MiddleFloor.bmp");
	m_pMiddleFloor6->SetInfo(iSecondFloor3X - TILECX * 10.64f,
		static_cast<float>(iSecondFloor3Y + TILECY), 256, 64);

}

void CTown::PlayerSetting()
{
	int iSecondFloor2X = int(TILECX * TILEX / 2 - 5.5f * TILECX);
	int iSecondFloor2Y = CScrollManager::GetScrollSizeY() - TILECY * 3 - (TILECY / 2) * 7;

	CPlayer* pPlayer = nullptr;
	if (GET_SINGLE(CObjectManager)->GetObjList(OT_PLAYER)->empty())
	{
		pPlayer = new CPlayer;
		pPlayer->Initialize();
		pPlayer->SetInfo(static_cast<float>(iSecondFloor2X), (iSecondFloor2Y - TILECY * (5.f)), 128, 128);

		GET_SINGLE(CObjectManager)->AddObj(OT_PLAYER, pPlayer);

		GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);
	}
	else
		pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	pPlayer->SetBoolRender(false);
	pPlayer->SetCanAction(false);
}
