#include "stdafx.h"
#include "BossRoom1.h"
#include "MiniMap.h"
#include "Player.h"
#include "BGImage.h"
#include "BossSkel.h"
#include "BossUI.h"
#include "TresureBox.h"
#include "Item.h"
#include "ShortSword.h"
#include "Stele.h"

bool CBossRoom1::m_bBoss = false;

CBossRoom1::CBossRoom1() :
	m_bBossName(false),
	m_bFlash(false),
	m_pLongFloor(nullptr),
	m_pMiddleFloor(nullptr),
	m_pBossLifeUI(nullptr),
	m_pMiniMap(nullptr),
	m_eBossScene(BOSS_SCENE_END)
{
}


CBossRoom1::~CBossRoom1()
{
	Release();
}

void CBossRoom1::Initialize()
{
	GET_SINGLE(CSoundMgr)->StopAll();
	m_strName = "1F : The Belial's Jail";

	GET_SINGLE(CMouse)->ChangeCursor(1);
	int iScrollX = TILECX * 30 / 2;
	int iScrollY = TILECY * 20 / 2;

	CScrollManager::SetScrollX((float)iScrollX);
	CScrollManager::SetScrollY((float)iScrollY);
	CScrollManager::SetScrollSizeX(TILECX * 26);
	CScrollManager::SetScrollSizeY(TILECY * 30);
	CScrollManager::SetScrollStartX(static_cast<int>(TILECX * 2.f));
	CScrollManager::SetScrollStartY(static_cast<int>(TILECY * 2.f));
	
	ImageSetting();

	GET_SINGLE(CTileManager)->Initialize();
	GET_SINGLE(CTileManager)->LoadData(L"Data/BossRoom1.dat");

	INSERT_BITMAP("CutSceneCrop", L"Image/CutSceneCrop.bmp");

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	
	GET_SINGLE(CPlayerUI)->ValueUpdate(pPlayer);
	m_pMiniMap = new CMiniMap;
	m_pMiniMap->Initialize();

	m_tUpCut = {WINCX * 0.5f, -250.f, 1024, 500};
	m_tDownCut = { WINCX * 0.5f, WINCY + 250.f, 1024, 500 };
}

int CBossRoom1::Update()
{

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (!m_bBoss && pPlayer->GetInfo().fx >= 830.f)			//	보스 생성
	{
		CBossSkel* pBoss = new CBossSkel;
		pBoss->Initialize();
		pBoss->SetInfo(870.f, 800.f);
		pBoss->HandPosition();
		pBoss->BackImageSetting();
		GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BOSS, pBoss);

		m_pBossLifeUI = new CBossUI;
		m_pBossLifeUI->Initialize();

		pPlayer->SetCanAction(false);
		m_dwSceneStartTime = GetTickCount();
		m_dwPrevTime = m_dwSceneStartTime;
		m_dwSceneEndTime = 5000;
		m_eBossScene = BOSS_SCENE_APPEAR;

		m_bBoss = true;

		GET_SINGLE(CSoundMgr)->PlayBGM(L"JailBoss.wav");
		GET_SINGLE(CSoundMgr)->SetVolume(CSoundMgr::BGM, 0.4f);
		GET_SINGLE(CSoundMgr)->PlaySound(L"Skeletonking.wav", CSoundMgr::MONSTER);
		GET_SINGLE(CSoundMgr)->SetVolume(CSoundMgr::MONSTER, 0.6f);
	}

	switch (m_eBossScene)
	{
	case BOSS_SCENE_APPEAR:
		if (m_dwSceneStartTime + m_dwSceneEndTime <= GetTickCount())	//	보스 등장씬 종료.
		{
			pPlayer->SetCanAction(true);
			m_eBossScene = BOSS_SCENE_END;
			
			CBossSkel* pBoss = dynamic_cast<CBossSkel*>((*GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER_BOSS)).front());
			
			if (pBoss)
				pBoss->SetStartPattern(true);

			CStele* pStele = new CStele;
			pStele->Initialize();
			pStele->SetSteleType(STELE_VL);
			pStele->SetInfo(225.f, 1215.f);
			pStele->StateChange(STELE_CLOSE);
			pStele->SetFieldObjectType(FOT_STELE);
			GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);

			pStele = new CStele;
			pStele->Initialize();
			pStele->SetSteleType(STELE_VR);
			pStele->SetInfo(1565.f, 1215.f);
			pStele->StateChange(STELE_CLOSE);
			pStele->SetFieldObjectType(FOT_STELE);
			GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_OBJECT, pStele);
		}
		else													//	보스 등장 씬 진행.
		{		
			float fScrollDestX = 870.f;
			float fScrollDestY = 800.f;

			float fDeltaTime = static_cast<float>(GetTickCount() - m_dwPrevTime);
			m_dwPrevTime = GetTickCount();

			float fDirX = (fScrollDestX - CScrollManager::GetScrollX()) * fDeltaTime / 2000;
			float fDirY = (fScrollDestY - CScrollManager::GetScrollY()) * fDeltaTime / 2000;

			CScrollManager::SetScrollX(CScrollManager::GetScrollX() + fDirX);
			CScrollManager::SetScrollY(CScrollManager::GetScrollY() + fDirY);

			if (m_tUpCut.fy <= -100)
				m_tUpCut.fy += 1.f;

			if (m_tDownCut.fy >= WINCY + 100)
				m_tDownCut.fy -= 1.f;
			else
				m_bBossName = true;
		}
		break;
	case BOSS_SCENE_DISAPPEAR:
		if (m_dwSceneStartTime + m_dwSceneEndTime <= GetTickCount())	//	보스 사망 완료.
		{
			m_eBossScene = BOSS_SCENE_END;
			pPlayer->SetCanAction(true);

			//	보스 보물 상자 넣기
			CTresureBox* pBox = new CTresureBox;

			pBox->Initialize();
			pBox->SetInfo(870.f, 1160.f);
			pBox->SetBox(43, 30, 4.f);
			pBox->SetCloseImage("BossTresureBoxClose", L"Image/Tresure/BossTresureClosed.bmp");
			pBox->SetOpenImage("BossTresureBoxOpen", L"Image/Tresure/BossTresureOpened.bmp");

			CShortSword* pShortSword = new CShortSword;
			pShortSword->Initialize();

			pBox->SetItem(pShortSword);
			GET_SINGLE(CObjectManager)->AddObj(OT_TRESURE_BOX, pBox);


			//	문 열기
			list<CObj*>* pFieldObjectList = GET_SINGLE(CObjectManager)->GetObjList(OT_FIELD_OBJECT);

			for (auto& pFieldObject : *pFieldObjectList)
			{
				CFieldObject* pFieldObj = dynamic_cast<CFieldObject*>(pFieldObject);
				if (pFieldObj->GetFieldObjectType() == FOT_STELE)
				{
					dynamic_cast<CStele*>(pFieldObj)->StateChange(STELE_OPEN);
				}
			}

		} 
		else								//	보스 사망 씬 진행.
		{
			float fScrollDestX = 870.f;
			float fScrollDestY = 800.f;

			float fDeltaTime = static_cast<float>(GetTickCount() - m_dwPrevTime);
			m_dwPrevTime = GetTickCount();

			float fDirX = (fScrollDestX - CScrollManager::GetScrollX()) * fDeltaTime / 1000;
			float fDirY = (fScrollDestY - CScrollManager::GetScrollY()) * fDeltaTime / 1000;

			CScrollManager::SetScrollX(CScrollManager::GetScrollX() + fDirX);
			CScrollManager::SetScrollY(CScrollManager::GetScrollY() + fDirY);
		}
		break;
	case BOSS_SCENE_END:
		break;
	default:
		break;
	}

	GET_SINGLE(CInventory)->Update();
	GET_SINGLE(CObjectManager)->Update();
	GET_SINGLE(CEffectManager)->Update();

	int iUIRet = GET_SINGLE(CPlayerUI)->Update();

	if (m_pBossLifeUI)
		if (m_pBossLifeUI->Update())	//	보스 사망
		{
			SAFE_DELETE(m_pBossLifeUI);

			m_dwSceneStartTime = GetTickCount();
			m_dwPrevTime = m_dwSceneStartTime;
			m_dwSceneEndTime = 6000;
			m_eBossScene = BOSS_SCENE_DISAPPEAR;
			m_bFlash = true;
			pPlayer->SetCanAction(false);
		}

	if (pPlayer->GetInfo().fx <= 160.f)
	{
		pPlayer->SetInfo(1880.f, 1280.f);
		return SC_STAGE2;
	}

	if (pPlayer->GetInfo().fx >= 1600.f)
	{
		GET_SINGLE(CResult)->ShowResult();
		GET_SINGLE(CResult)->AdventureSuccess();
		pPlayer->SetCanAction(false);
	}



	TorchUpdate();

	int iResult = GET_SINGLE(CResult)->Update();

	if (iUIRet)
		return iUIRet;

	if (iResult)
		return iResult;

	return SC_NONE;
}

void CBossRoom1::LateUpdate()
{
	GET_SINGLE(CObjectManager)->LateUpdate();
	GET_SINGLE(CEffectManager)->LateUpdate();
}

void CBossRoom1::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->Render(hDC);

	BGImageRender(hDC);

	GET_SINGLE(CEffectManager)->FirstRender(hDC);
	GET_SINGLE(CObjectManager)->Render(hDC);
	GET_SINGLE(CEffectManager)->Render(hDC);
	//m_pMiniMap->Render(hDC);

	if (m_eBossScene == BOSS_SCENE_END)
	{
		if (!GET_SINGLE(CResult)->GetShowResult())
		{
			GET_SINGLE(CPlayerUI)->Render(hDC);
			m_pMiniMap->Render(hDC);

			if (m_pBossLifeUI)
				m_pBossLifeUI->Render(hDC);
		}
	}

	GET_SINGLE(CInventory)->Render(hDC);

	if (m_eBossScene == BOSS_SCENE_APPEAR)
	{
		HDC hMemDC = FIND_BITMAP_DC("CutSceneCrop");
		BitBlt(hDC, 
			static_cast<int>(m_tUpCut.fx - m_tUpCut.icx * 0.5f), 
			static_cast<int>(m_tUpCut.fy - m_tUpCut.icy * 0.5), m_tUpCut.icx, m_tUpCut.icy, hMemDC, 0, 0, SRCCOPY);
		BitBlt(hDC, 
			static_cast<int>(m_tDownCut.fx - m_tDownCut.icx * 0.5f), 
			static_cast<int>(m_tDownCut.fy - m_tDownCut.icy * 0.5f), m_tDownCut.icx, m_tDownCut.icy, hMemDC, 0, 0, SRCCOPY);

		if (m_bBossName)
		{
			GET_SINGLE(CFontManager)->Render(hDC, 10, WINCY - 200, FT_EDGE, 3.f, "Belial");
			GET_SINGLE(CFontManager)->Render(hDC, 10, WINCY - 230, FT_EDGE, 2.f, "The guardian of prison");
		}
	}

	if (m_eBossScene == BOSS_SCENE_DISAPPEAR)
	{
		if (m_bFlash)
		{
			Rectangle(hDC, -1, -1, WINCX, WINCY);
			if (m_dwSceneStartTime + 500 <= GetTickCount())
				m_bFlash = false;;
		}
	}


}

void CBossRoom1::Release()
{
	DESTROY_SINGLE(CTileManager);
	SAFE_DELETE(m_pLongFloor);

	for (auto& pFloor : m_ShortFloorList)
		SAFE_DELETE(pFloor);
	m_ShortFloorList.clear();

	SAFE_DELETE(m_pMiddleFloor);
	
	for (auto& pTorch : m_TorchList)
		SAFE_DELETE(pTorch);
	m_ShortFloorList.clear();

	SAFE_DELETE(m_pBossLifeUI);
	
	SAFE_DELETE(m_pMiniMap);

	ObjectListClear();
}

void CBossRoom1::ImageSetting()
{
	m_pLongFloor = new CBGImage;
	m_pLongFloor->SetImage("LongFloor", L"Image/Dungeon/MiddleFloor(Long).bmp");
	m_pLongFloor->SetInfo(898.f, 1248.f, 512, 64);


	float fOffset = 416.f;
	CBGImage* pShortFloor = new CBGImage;
	pShortFloor->SetImage("ShortFloor", L"Image/Dungeon/MiddleFloor(Short).bmp");
	pShortFloor->SetInfo(898.f - fOffset + TILECX, 1248.f - TILECY * 3.f, 192, 64);
	m_ShortFloorList.push_back(pShortFloor);

	pShortFloor = new CBGImage;
	pShortFloor->SetImage("ShortFloor", L"Image/Dungeon/MiddleFloor(Short).bmp");
	pShortFloor->SetInfo(898.f - fOffset, 1248.f - TILECY * 6.f, 192, 64);
	m_ShortFloorList.push_back(pShortFloor);

	pShortFloor = new CBGImage;
	pShortFloor->SetImage("ShortFloor", L"Image/Dungeon/MiddleFloor(Short).bmp");
	pShortFloor->SetInfo(898.f - fOffset, 1248.f - TILECY * 9.f, 192, 64);
	m_ShortFloorList.push_back(pShortFloor);

	pShortFloor = new CBGImage;
	pShortFloor->SetImage("ShortFloor", L"Image/Dungeon/MiddleFloor(Short).bmp");
	pShortFloor->SetInfo(898.f + fOffset - TILECX, 1248.f - TILECY * 3.f, 192, 64);
	m_ShortFloorList.push_back(pShortFloor);

	pShortFloor = new CBGImage;
	pShortFloor->SetImage("ShortFloor", L"Image/Dungeon/MiddleFloor(Short).bmp");
	pShortFloor->SetInfo(898.f + fOffset, 1248.f - TILECY * 6.f, 192, 64);
	m_ShortFloorList.push_back(pShortFloor);

	pShortFloor = new CBGImage;
	pShortFloor->SetImage("ShortFloor", L"Image/Dungeon/MiddleFloor(Short).bmp");
	pShortFloor->SetInfo(898.f + fOffset, 1248.f - TILECY * 9.f, 192, 64);
	m_ShortFloorList.push_back(pShortFloor);

	m_pMiddleFloor = new CBGImage;
	m_pMiddleFloor->SetImage("MiddleFloor", L"Image/Dungeon/MiddleFloor.bmp");
	m_pMiddleFloor->SetInfo(898.f, 1248.f - TILECY * 11.f, 256, 64);


	//	====================== Torch ========================
	fOffset -= 130.f;
	CBGImage* pTorch = new CBGImage;
	pTorch->SetImage("Torch", L"Image/Dungeon/Torch/TorchSprite.bmp");
	pTorch->SetInfo(898.f - fOffset, 1248.f - TILECY * 9.f - 32, 32, 32);
	pTorch->SetMaxFrame(8);
	pTorch->SetRatio(4.f);
	m_TorchList.push_back(pTorch);

	pTorch = new CBGImage;
	pTorch->SetImage("Torch", L"Image/Dungeon/Torch/TorchSprite.bmp");
	pTorch->SetInfo(898.f + fOffset, 1248.f - TILECY * 9.f - 32, 32, 32);
	pTorch->SetMaxFrame(8);
	pTorch->SetRatio(4.f);
	m_TorchList.push_back(pTorch);

	pTorch = new CBGImage;
	pTorch->SetImage("Torch", L"Image/Dungeon/Torch/TorchSprite.bmp");
	pTorch->SetInfo(898.f - fOffset, 1248.f - TILECY * 3.f + 32, 32, 32);
	pTorch->SetMaxFrame(8);
	pTorch->SetRatio(4.f);
	m_TorchList.push_back(pTorch);

	pTorch = new CBGImage;
	pTorch->SetImage("Torch", L"Image/Dungeon/Torch/TorchSprite.bmp");
	pTorch->SetInfo(898.f + fOffset, 1248.f - TILECY * 3.f + 32, 32, 32);
	pTorch->SetMaxFrame(8);
	pTorch->SetRatio(4.f);
	m_TorchList.push_back(pTorch);
}

void CBossRoom1::BGImageRender(HDC hDC)
{
	m_pLongFloor->Render(hDC);
	for (auto& pFloor : m_ShortFloorList)
		pFloor->Render(hDC);
	m_pMiddleFloor->Render(hDC);

	for (auto& pTorch : m_TorchList)
		pTorch->Render(hDC);
}

void CBossRoom1::TorchUpdate()
{
	for (auto& pTorch : m_TorchList)
		pTorch->Update();
}
