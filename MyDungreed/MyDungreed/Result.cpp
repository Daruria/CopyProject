#include "stdafx.h"
#include "Result.h"
#include "Scene.h"
#include "Player.h"
#include "InvenSlot.h"
#include "Item.h"
#include "GoldCoin.h"
#include "ShortSword.h"
#include "MagicWand.h"
#include "AnimatedBGObject.h"
#include "BGImage.h"
#include "Dungeon1.h"
#include "Dungeon2.h"
#include "Dungeon3.h"
#include "BossRoom1.h"

DEFINITION_SINGLE(CResult);

CResult::CResult() :
	m_bAdventureSuccess(false),
	m_bPlayStart(false),
	m_bShowResult(false),
	m_bSlotCopy(false),
	m_iMonsterCount(0),
	m_iPlayTime(0),
	m_iExp(0),
	m_iAddMonster(0),
	m_iEatCoinCool(0),
	m_fItemEatSpeed(0.f),
	m_ePhase(RP_END),
	m_dwPlayTime(0),
	m_dwPhaseCheck(0),
	m_dwPrevTime(0),
	m_tTitleInfo(INFO{}),
	m_pEatItem(nullptr),
	m_pExitButton(nullptr),
	m_bLevelUp(false),
	m_bLevelImgStop(false),
	m_fLevelUpImgRatio(0.f),
	m_fLevelUpRatioAcc(0.f),
	m_dwLevelUpImgStop(0),
	m_tLevelUpImgInfo(INFO{})
{
}

CResult::~CResult()
{
	Release();
}

void CResult::Initialize()
{
	INSERT_BITMAP("ExplorationFailureEng", L"Image/UI/Result/ExplorationFailureEng.bmp");
	INSERT_BITMAP("ExplorationSuccessEng", L"Image/UI/Result/ExplorationSuccessEng.bmp");
	INSERT_BITMAP("Result", L"Image/UI/Result/Result.bmp");
	INSERT_BITMAP("ResultSuccess", L"Image/UI/Result/ResultSuccess.bmp");
	INSERT_BITMAP("LevelUpBar", L"Image/UI/Result/LevelUpBar.bmp");

	INSERT_BITMAP("WindowBack", L"Image/UI/Rect/WindowBack.bmp");

	m_tTitleInfo = { WINCX * 0.5f, WINCY * 0.5f, 320, 36 };

	m_pEatItem = new CAnimatedBGObject;
	m_pEatItem->Initialize();
	m_pEatItem->SetImage("EatItem", L"Image/UI/Result/EatItem.bmp");
	m_pEatItem->SetAnimationType(AT_ONCE);
	m_pEatItem->SetMaxFrame(10);
	m_pEatItem->SetTotalFrame(51);
	m_pEatItem->SetTimePerFrame(50);
	m_pEatItem->SetRatio(4.f);
	m_pEatItem->SetRenderType(RT_SCREEN);
	m_pEatItem->SetInfo(WINCX / 2 + 100.f, WINCY - 200.f, 127, 85);

	m_pExitButton = new CMyButton;
	m_pExitButton->Initialize("ButtonDisable", L"Image/UI/Button/ButtonDisable.bmp",
		"UIButtonOn", L"Image/UI/Button/ButtonOn.bmp");
	m_pExitButton->SetRatio(4.f);
	m_pExitButton->SetPoint(WINCX - 130, WINCY - 80);

	m_fLevelUpImgRatio = 4.f;
	m_fLevelUpRatioAcc = 1.f;

	m_tLevelUpImgInfo = { WINCX * 0.5f, WINCY * 0.5f, 113, 45 };
	INSERT_BITMAP("LevelUp", L"Image/UI/Result/LevelUp.bmp");
}

int CResult::Update()
{
	if (m_bPlayStart)
	{
		if (!m_bShowResult)
			TimeUpdate();
		else
		{
			GET_SINGLE(CMouse)->ChangeCursor(0);
			DWORD dwCurTime = GetTickCount();
			switch (m_ePhase)
			{
			case RP_END_OF_ADV:		EndOfAdventure(dwCurTime);
				break;
			case RP_TITLE_MOVE_UP:	TitleMoveUp();
				break;
			case RP_RESULT:			Result();
				break;
			case RP_LEVEL:			Leveling();
				break;
			case RP_EAT_ITEM:		EatItems();
				break;
			case RP_EXIT:	return  Exit();
			case RP_END:
				m_ePhase = RP_END_OF_ADV;
				m_dwPhaseCheck = dwCurTime;
				if (m_bAdventureSuccess)
					GET_SINGLE(CSoundMgr)->PlaySound(L"fanfare.wav", CSoundMgr::EFFECT);
				break;
			default:
				break;
			}
		}
		m_dwPrevTime = GetTickCount();
	}
	return SC_NONE;
}

void CResult::Render(HDC hDC)
{
	if (!m_bShowResult)
		return;


	if (m_ePhase > RP_TITLE_MOVE_UP)
	{
		HDC hMemDC = FIND_BITMAP_DC("WindowBack");

		SIZE tSize = GET_BITMAP_SIZE("WindowBack");
		GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
	}

	TitleRender(hDC);

	if (m_ePhase > RP_TITLE_MOVE_UP)
	{
		ResultRender(hDC);
		LevelBarRender(hDC);
	}

	if (m_ePhase == RP_EXIT)
	{
		m_pExitButton->Render(hDC);
		GET_SINGLE(CFontManager)->Render(hDC, static_cast<float>(WINCX - 150), static_cast<int>(WINCY - 95), FT_NORMAL, 2.f, "Exit");
	}
}

void CResult::Release()
{
	SAFE_DELETE(m_pEatItem);
	SAFE_DELETE(m_pExitButton);

	for (auto& pSlot : m_ItemSlot)
		SAFE_DELETE(pSlot);

	for (auto& pCoin : m_CoinList)
		SAFE_DELETE(pCoin);

	m_ItemSlot.clear();

}

//	결과 보여주는 단계
void CResult::ShowResult()
{
	m_bShowResult = true;

	if (!m_bSlotCopy)
	{
		vector<CInvenSlot*>* pVecInvenSlot = GET_SINGLE(CInventory)->GetEquipSlot();
		int iPosX = 70;
		int iPosY = WINCY / 2 + 150;

		int iItemCount = 0;

		SIZE tSize = GET_BITMAP_SIZE("InvenItemSlot");

		for (auto& pSlot : *pVecInvenSlot)
		{
			if (pSlot->GetItem())
			{
				CInvenSlot* pNewSlot = new CInvenSlot;
				pNewSlot->SetSlotType(pSlot->GetSlotType());
				pNewSlot->Initialize("InvenItemSlot", L"Image/Inventory/IconGray.bmp", "InvenItemSlotOn", L"Image/Inventory/IconWhite.bmp");
				pNewSlot->SetPoint(iPosX + (tSize.cx + 10) * (iItemCount % 6), iPosY + (tSize.cy + 8) * (iItemCount / 6));
				pNewSlot->SetItem(pSlot->GetItem());
				m_ItemSlot.push_back(pNewSlot);
				pSlot->SetItem(NULL);
				++iItemCount;
			}
		}

		GET_SINGLE(CInventory)->ApplyPlayerEquip();
		pVecInvenSlot = GET_SINGLE(CInventory)->GetInvenSlot();

		for (auto& pSlot : *pVecInvenSlot)
		{
			if (pSlot->GetItem())
			{
				CInvenSlot* pNewSlot = new CInvenSlot;
				pNewSlot->SetSlotType(pSlot->GetSlotType());
				pNewSlot->Initialize("InvenItemSlot", L"Image/Inventory/IconGray.bmp", "InvenItemSlotOn", L"Image/Inventory/IconWhite.bmp");
				pNewSlot->SetPoint(iPosX + (tSize.cx + 10) * (iItemCount % 6), iPosY + (tSize.cy + 8) * (iItemCount / 6));
				pNewSlot->SetItem(pSlot->GetItem());
				m_ItemSlot.push_back(pNewSlot);
				pSlot->SetItem(NULL);
				++iItemCount;
			}
		}

		m_bSlotCopy = true;
	}

}

void CResult::AddActivities(const string & strActivity)
{
	string strText = strActivity;
	m_ActivitiesList.push_back(strText);
}

void CResult::TimeUpdate()
{
	DWORD dwCurTime = GetTickCount();
	if (m_dwPlayTime + 1000 < dwCurTime)
	{
		m_iPlayTime++;
		m_dwPlayTime = dwCurTime;
	}
}

void CResult::EndOfAdventure(DWORD dwCurTime)
{
	if (m_dwPhaseCheck + 2000 < dwCurTime)
	{
		m_ePhase = RP_TITLE_MOVE_UP;
		m_dwPhaseCheck = dwCurTime;
	}
	else
	{

	}
}

void CResult::TitleMoveUp()
{
	if (m_tTitleInfo.fy - m_tTitleInfo.icy * 3.2f * 0.5f <= 0.f)
	{
		m_tTitleInfo.fy = m_tTitleInfo.icy * 3.2f * 0.5f;
		m_ePhase = RP_RESULT;
		m_dwPhaseCheck = GetTickCount();
	}
	else
		m_tTitleInfo.fy -= 10.f;

}

void CResult::Result()
{
	if (m_dwPhaseCheck + 1000 < GetTickCount())
	{
		m_ePhase = RP_LEVEL;
		m_dwPhaseCheck = GetTickCount();
	}
	else
	{

	}
}

void CResult::Leveling()
{
	if (m_iAddMonster >= m_iMonsterCount && !m_bLevelUp)
	{
		m_iAddMonster = 0;
		m_ePhase = RP_EAT_ITEM;
		return;
	}

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	m_iExp++;
	if (m_iExp % 10 == 0)
	{
		pPlayer->GainExp(10);
		if (m_iExp % 50 == 0)
			++m_iAddMonster;
	}
	m_fLevelRatio = float(m_iExp) / (pPlayer->GetExpMax() * (1.f + pPlayer->GetLevel() * 0.1f));
	if (m_fLevelRatio >= 1.f)
	{
		pPlayer->LevelUp();
		GET_SINGLE(CSoundMgr)->PlaySound(L"Fantasy_Game_UI_Fire_Confirm_ability.wav", CSoundMgr::EFFECT);
		m_iExp = 0;
		m_bLevelUp = true;
		m_fLevelUpRatioAcc = 0.7f;
		m_dwLevelUpImgStop = GetTickCount();

	}

	if (m_bLevelUp)
	{
		if (!m_bLevelImgStop)
		{
			m_fLevelUpImgRatio += m_fLevelUpRatioAcc;
			m_fLevelUpRatioAcc -= 0.05f;
			if (m_fLevelUpImgRatio <= 4.f)
			{
				m_fLevelUpImgRatio = 4.f;
				m_bLevelImgStop = true;
			}
		}
		else
		{
			if (m_dwLevelUpImgStop + 1000 <= GetTickCount())
			{
				m_bLevelUp = false;
				m_bLevelImgStop = false;
			}
		}
	}
}

void CResult::EatItems()
{
	if (m_pEatItem->Update())
	{
		if (m_CoinList.empty() && m_ItemSlot.empty() && m_pEatItem->GetCurFrame() >= 50)
		{
			m_ePhase = RP_EXIT;
			m_dwPhaseCheck = GetTickCount();
			return;
		}

		m_pEatItem->SetStartFrame(10);
		m_pEatItem->SetMaxFrame(24);
		m_pEatItem->SetAnimationType(AT_REPEAT);
	}

	if (m_CoinList.empty() && m_ItemSlot.empty())
	{
		m_pEatItem->SetMaxFrame(51);
		m_pEatItem->SetAnimationType(AT_ONCE);
	}

	if (m_pEatItem->GetCurFrame() >= 10)
	{
		m_fItemEatSpeed = m_fItemEatSpeed >= 10.f ? 10.f : m_fItemEatSpeed + 0.1f;

		for (auto iter = m_ItemSlot.begin(); iter != m_ItemSlot.end();)
		{
			float fdx = m_pEatItem->GetInfo().fx - 20.f - (*iter)->GetPoint().x;
			float fdy = m_pEatItem->GetInfo().fy + 100.f - (*iter)->GetPoint().y;

			if (fabs(fdx) < 5.f && fabs(fdy) < 5.f)
			{
				SAFE_DELETE(*iter);
				iter = m_ItemSlot.erase(iter);

				continue;
			}

			CMath::NormalizeXY(fdx, fdy);
			(*iter)->AddPoint(static_cast<LONG>(fdx * m_fItemEatSpeed), static_cast<LONG>(fdy * m_fItemEatSpeed));

			++iter;
		}
		int iLevel = GET_SINGLE(CObjectManager)->GetPlayer()->GetLevel();

		++m_iEatCoinCool;

		if (m_iEatCoinCool % 7 == 0 && GET_SINGLE(CInventory)->GetPlayerGold() > 1000 + (iLevel - 1) * 100)
		{
			m_iEatCoinCool = 1;
			CGoldCoin* pCoin = new CGoldCoin;
			pCoin->Initialize();
			pCoin->SetIconImage("CoinIcon", L"Image/Icon/GoldCoinIcon(x4).bmp");
			pCoin->SetRatio(4.f);
			pCoin->SetGold(10);
			pCoin->SetInfo(WINCX / 2 - 200.f, WINCY / 2 - 50.f);

			m_CoinList.push_front(pCoin);
			GET_SINGLE(CInventory)->AddGold(-10);
		}

		for (auto iter = m_CoinList.begin(); iter != m_CoinList.end();)
		{
			float fdx = m_pEatItem->GetInfo().fx - 20.f - (*iter)->GetInfo().fx;
			float fdy = m_pEatItem->GetInfo().fy + 100.f - (*iter)->GetInfo().fy;

			if (fabs(fdx) < 5.f && fabs(fdy) < 5.f)
			{
				SAFE_DELETE(*iter);
				iter = m_CoinList.erase(iter);

				continue;
			}

			CMath::NormalizeXY(fdx, fdy);
			(*iter)->AddInfofx(static_cast<float>(fdx * m_fItemEatSpeed));
			(*iter)->AddInfofy(static_cast<float>(fdy * m_fItemEatSpeed));

			++iter;
		}

	}

}

int CResult::Exit()
{
	if (m_pExitButton->Update())
	{
		//DESTROY_SINGLE(CInventory);
		//GET_SINGLE(CInventory)->Initialize();


		CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

		pPlayer->SetInfo(3808.f, 720.f);

		pPlayer->Resetting();

		CShortSword* pShortSword = new CShortSword;
		pShortSword->Initialize();

		pPlayer->Equip(pShortSword, ES_WEAPON1);

		CMagicWand* pMagicWand = new CMagicWand;
		pMagicWand->Initialize();
		pPlayer->Equip(pMagicWand, ES_WEAPON2);

		GET_SINGLE(CPlayerUI)->WeaponSlotReset();

		m_bAdventureSuccess = false;
		m_bPlayStart = false;
		m_bShowResult = false;
		m_bSlotCopy = false;
		m_iMonsterCount = 0;
		m_iPlayTime = 0;
		m_iExp = 0;
		m_iAddMonster = 0;
		m_iEatCoinCool = 0;
		m_fLevelRatio = 0.f;
		m_fItemEatSpeed = 0.f;
		m_ePhase = RP_END;
		m_dwPlayTime = 0;
		m_dwPhaseCheck = 0;
		m_dwPrevTime = 0;
		m_tTitleInfo.fx = WINCX / 2;
		m_tTitleInfo.fy = WINCY / 2;
		m_ActivitiesList.clear();

		GET_SINGLE(CPlayerUI)->WindowListClear();

		CDungeon1::SpawnReset();
		CDungeon2::SpawnReset();
		CDungeon3::SpawnReset();
		CBossRoom1::SpawnReset();

		return SC_TOWN;

	}
	return SC_NONE;
}

void CResult::TitleRender(HDC hDC)
{
	HDC hMemDC = m_bAdventureSuccess ? FIND_BITMAP_DC("ExplorationSuccessEng") : FIND_BITMAP_DC("ExplorationFailureEng");

	GdiTransparentBlt(hDC,
		static_cast<int>(m_tTitleInfo.fx - m_tTitleInfo.icx * 3.2f * 0.5),
		static_cast<int>(m_tTitleInfo.fy - m_tTitleInfo.icy * 3.2f * 0.5),
		static_cast<int>(m_tTitleInfo.icx * 3.2f),
		static_cast<int>(m_tTitleInfo.icy * 3.2f),
		hMemDC,
		0, 0, m_tTitleInfo.icx, m_tTitleInfo.icy, RGB(255, 0, 255));

}

void CResult::ResultRender(HDC hDC)
{
	HDC hMemDC = m_bAdventureSuccess ? FIND_BITMAP_DC("ResultSuccess") : FIND_BITMAP_DC("Result");

	GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, 320, 180, RGB(255, 0, 255));

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 440.f, WINCY / 2 - 190, FT_NORMAL, 2.f, "Time", FPX_CENTER);

	int iMinute = (m_iPlayTime / 60) % 60;
	int iHour = (m_iPlayTime / 60) / 60;
	int iSecond = m_iPlayTime % 60;
	string strText = to_string(iHour) + "h " + to_string(iMinute) + "m " + to_string(iSecond) + "s";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 50.f, WINCY / 2 - 190, FT_NORMAL, 2.f, strText, FPX_RIGHT);

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 440.f, WINCY / 2 - 120, FT_NORMAL, 2.f, "Place", FPX_CENTER);

	strText = GET_SINGLE(CSceneManager)->GetScene()->GetName();
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 50.f, WINCY / 2 - 120, FT_NORMAL, 2.f, strText, FPX_RIGHT);

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 440.f, WINCY / 2 - 50.f, FT_NORMAL, 2.f, "Gold", FPX_CENTER);

	int iGold = GET_SINGLE(CInventory)->GetPlayerGold();
	strText = to_string(iGold) + "G";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 50.f, WINCY / 2 - 50.f, FT_NORMAL, 2.f, strText, FPX_RIGHT);

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 - 297.f, WINCY / 2 + 45.f, FT_NORMAL, 2.f, "Items");

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 + 218.f, WINCY / 2 - 190.f, FT_NORMAL, 2.f, "Activities");

	strText = to_string(m_iMonsterCount) + " Monsters Killed";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 + 50.f, WINCY / 2 - 120.f, FT_NORMAL, 3.f, strText);

	float fActRenderY = WINCY / 2 - 80.f;
	int iCount = 0;
	for (auto& strAct : m_ActivitiesList)
	{
		GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 + 50.f, fActRenderY + iCount * 40.f, FT_NORMAL, 3.f, strAct);
		++iCount;
	}

	if (!m_bAdventureSuccess)
	{
		GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 + 200.f, WINCY / 2 + 125.f, FT_NORMAL, 2.f, "Defeated by");

		strText = GET_SINGLE(CObjectManager)->GetPlayer()->GetAttackMonster();
		GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 + 270.f, WINCY / 2 + 195.f, FT_NORMAL, 2.f, strText, FPX_CENTER);
	}

	if (m_ePhase == RP_EAT_ITEM)
		EatItemRender(hDC);

	for (auto& pSlot : m_ItemSlot)
		pSlot->Render(hDC);
}

void CResult::EatItemRender(HDC hDC)
{
	m_pEatItem->Render(hDC);

	for (auto& pCoin : m_CoinList)
		pCoin->IconRender(hDC, pCoin->GetInfo().fx, pCoin->GetInfo().fy);
}

void CResult::LevelBarRender(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("LevelUpBar");

	GdiTransparentBlt(hDC, 0, WINCY - 10,
		static_cast<int>(m_fLevelRatio * WINCX), 10, hMemDC, 0, 0, 54, 5, RGB(255, 0, 255));

	if (m_ePhase == RP_LEVEL)
	{
		if (m_bLevelUp)
		{
			hMemDC = FIND_BITMAP_DC("LevelUp");

			GdiTransparentBlt(hDC,
				static_cast<int>(m_tLevelUpImgInfo.fx - m_tLevelUpImgInfo.icx * 0.5f * m_fLevelUpImgRatio),
				static_cast<int>(m_tLevelUpImgInfo.fy - m_tLevelUpImgInfo.icy * 0.5f * m_fLevelUpImgRatio),
				static_cast<int>(m_tLevelUpImgInfo.icx * m_fLevelUpImgRatio),
				static_cast<int>(m_tLevelUpImgInfo.icy * m_fLevelUpImgRatio),
				hMemDC, 0, 0, m_tLevelUpImgInfo.icx, m_tLevelUpImgInfo.icy, RGB(255, 0, 255));

			string strText = to_string(GET_SINGLE(CObjectManager)->GetPlayer()->GetLevel());
			int iSizeY = GET_SINGLE(CFontManager)->GetFontTotalYSize(strText, FT_EDGE, m_fLevelUpImgRatio);

			GET_SINGLE(CFontManager)->Render(hDC, m_tLevelUpImgInfo.fx, m_tLevelUpImgInfo.fy - iSizeY * 0.5f, FT_EDGE, m_fLevelUpImgRatio, strText, FPX_CENTER);
		}
	}
}
