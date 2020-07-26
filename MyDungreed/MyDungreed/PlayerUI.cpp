#include "stdafx.h"
#include "PlayerUI.h"
#include "Player.h"
//#include "FontNumber.h"
#include "Inventory.h"
#include "InvenSlot.h"
#include "UISlot.h"
#include "UIWindow.h"
#include "Weapon.h"

DEFINITION_SINGLE(CPlayerUI);

CPlayerUI::CPlayerUI()
	:m_iLevel(0), 
	m_iHp(0), 
	m_iMaxHp(0), 
	m_iDashCount(0), 
	m_iMaxDash(0),

	m_bLifeWaveOnOff(false),
	m_iLifeWaveFrame(0),
	m_iLifeWaveMaxFrame(0),
	m_dwLifeWaveFrameTime(0),

	m_bSwapAnimation(true),
	m_bSwapCue(false),
	m_bWeapon1Front(true),
	m_dwSwapTime(0),
	m_dwSwapStartTime(0),
	m_tFrontPos(POINT{}),
	m_tBackPos(POINT{}),

	m_pWeapon1(nullptr), 
	m_pWeapon2(nullptr), 

	m_bStageTitle(false),
	m_fStageTitleSpeed(0.f),
	m_ePhase(STP_END),
	m_dwStageTitleStopTime(0),
	m_tStageTitleInfo(INFO{}), 
	m_strStageTitle("")
{
}


CPlayerUI::~CPlayerUI()
{
	Release();
}

void CPlayerUI::Initialize(const string & strKey, const wchar_t * pPath)
{
}

int CPlayerUI::Update()
{
	//	장착 무기 UI 업데이트
	EquipSlotUIUpdate();

	DWORD dwCurTime = GetTickCount();

	//	플레이어 체력 Life Wave 업데이트
	LifeWaveUpdate(dwCurTime);

	//	무기 교체 애니메이션
	WeaponSwapAnimate(dwCurTime);

	//	띄워진 창 업데이트.(ex > 상점 창)
	int iRet = WindowListUpdate();

	//	씬 타이틀 업데이트.
	if (m_bStageTitle)
		StageTitleUpdate();
	
	return iRet;
}

void CPlayerUI::LateUpdate()
{
}

void CPlayerUI::Render(HDC hDC)
{
	//	Player HP Bar 출력
	PlayerHpBarRender(hDC);

	//	Player Dash Bar 출력
	PlayerDashBarRender(hDC);

	//	좌측 하단 플레이어 골드 출력
	PlayerGoldRender(hDC);

	//	우측 하단 플레이어 착용 무기 출력
	EquipSlotUIRender(hDC);

	SkillSlotUIRender(hDC);

	for (auto& pUiWindow : m_WindowList)
		if (pUiWindow->GetIsOn())
			pUiWindow->Render(hDC);

	StageTitleRender(hDC);

}

void CPlayerUI::Release()
{
	SAFE_DELETE(m_pWeapon1);
	SAFE_DELETE(m_pWeapon2);

	for (auto& pWindow : m_WindowList)
		SAFE_DELETE(pWindow);

	m_WindowList.clear();
}

void CPlayerUI::Initialize()
{
	INSERT_BITMAP("DashCount",		L"Image/UI/DashGauge/DashCount.bmp");
	INSERT_BITMAP("DashCountBase",	L"Image/UI/DashGauge/DashCountBase.bmp");
	INSERT_BITMAP("LifeBarFull",	L"Image/UI/PlayerHP/LifeBarFull.bmp");
	INSERT_BITMAP("LifeWave",		L"Image/UI/PlayerHP/LifeWave.bmp");
	INSERT_BITMAP("PlayerLifeBack", L"Image/UI/PlayerHP/PlayerLifeBack.bmp");
	INSERT_BITMAP("PlayerLifeBase", L"Image/UI/PlayerHP/PlayerLifeBase.bmp");
	
	INSERT_BITMAP("CoinIcon", L"Image/Icon/GoldCoinIcon.bmp");
	
	INSERT_BITMAP("SkillUISlot", L"Image/UI/Skill/IconWhite.bmp");
	INSERT_BITMAP("SkillUICool", L"Image/UI/Skill/IconGray.bmp");

	INSERT_BITMAP("Keyboard_Q", L"Image/Keyboard/Keyboard_Q.bmp");

	m_dwLifeWaveFrameTime = GetTickCount();
	m_iLifeWaveMaxFrame = 7;
	m_iLifeWaveFrame = 0;
	
	m_bSwapAnimation = true;
	m_bSwapCue = false;
	m_dwSwapTime = 500;

	m_bWeapon1Front = true;
	m_tFrontPos = { WINCX - 100,	WINCY - 60 };
	m_tBackPos	= { WINCX - 90,		WINCY - 70 };

	m_pWeapon1 = new CUISlot;
	m_pWeapon1->Initialize("EquippedWeaponBase", L"Image/UI/EquipWeapon/EquippedWeaponBase(x3).bmp",
		"EquippedWeaponBase", L"Image/UI/EquipWeapon/EquippedWeaponBase(x3).bmp");
	m_pWeapon1->SetPoint(m_tFrontPos);

	m_pWeapon2 = new CUISlot;
	m_pWeapon2->Initialize("EquippedWeaponBase", L"Image/UI/EquipWeapon/EquippedWeaponBase(x3).bmp",
		"EquippedWeaponBase", L"Image/UI/EquipWeapon/EquippedWeaponBase(x3).bmp");
	m_pWeapon2->SetPoint(m_tBackPos);
}

const bool CPlayerUI::IsThereOpenWindow() const
{
	for (auto& pWindow : m_WindowList)
	{
		if (pWindow->GetIsOn())
			return true;
	}

	return false;
}

void CPlayerUI::ValueUpdate(CObj * pObj)
{
	if (pObj == nullptr)
		return;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	m_iLevel	= pPlayer->GetLevel();
	m_iHp		= pPlayer->GetActiveInfo().iHP;
	m_iMaxHp	= pPlayer->GetActiveInfo().iHPMax;
	m_iDashCount= pPlayer->GetDash().iDashCount;
	m_iMaxDash	= pPlayer->GetDash().iDashMax;
}

void CPlayerUI::StartSwap()
{
	if (m_bSwapAnimation)
	{
		m_dwSwapStartTime = GetTickCount();
		m_bSwapAnimation = false;
		m_bSwapCue = true;
	}
}

void CPlayerUI::AddWindow(CUIWindow * pWindow)
{
	m_WindowList.push_back(pWindow);
}

void CPlayerUI::StageTitleComeIn(const string& strTitle)
{
	m_bStageTitle = true;
	m_strStageTitle = strTitle;

	int iSize = GET_SINGLE(CFontManager)->GetFontTotalXSize(strTitle, FT_EDGE, 4.f);

	m_tStageTitleInfo = {-iSize * 0.5f, WINCY / 2 - 200.f};
	m_fStageTitleSpeed = 60.f;
	m_ePhase = STP_COME_IN;
}

void CPlayerUI::WeaponSlotReset()
{
	m_pWeapon1->SetPoint(m_tFrontPos);
	m_pWeapon2->SetPoint(m_tBackPos);
	m_bWeapon1Front = true;
}

CUIWindow * CPlayerUI::FindUIWindow(const string & strKey)
{
	for (auto pWindow : m_WindowList)
	{
		if (pWindow->GetKey() == strKey)
			return pWindow;
	}
	return nullptr;
}

void CPlayerUI::WindowListClear()
{
	for (auto& pWindow : m_WindowList)
	{
		pWindow->SetLife(false);
	}
}

void CPlayerUI::EquipSlotUIUpdate()
{
	m_pWeapon1->SetItem(GET_SINGLE(CInventory)->GetEquipSlot(IST_WEAPON1)->GetItem());
	m_pWeapon2->SetItem(GET_SINGLE(CInventory)->GetEquipSlot(IST_WEAPON2)->GetItem());
}

void CPlayerUI::LifeWaveUpdate(DWORD dwCurTime)
{
	if (m_dwLifeWaveFrameTime + 100 <= dwCurTime)
	{
		++m_iLifeWaveFrame;
		if (m_iLifeWaveFrame >= m_iLifeWaveMaxFrame)
			m_iLifeWaveFrame = 0;
		m_dwLifeWaveFrameTime = dwCurTime;
	}
}

void CPlayerUI::WeaponSwapAnimate(DWORD dwCurTime)
{
	if (m_bSwapCue)
	{
		LONG lDifference = (dwCurTime - m_dwSwapStartTime) * 10 / m_dwSwapTime;

		if (m_bWeapon1Front)
		{
			m_pWeapon1->SetPoint(m_tFrontPos.x + lDifference, m_tFrontPos.y - lDifference);
			m_pWeapon2->SetPoint(m_tBackPos.x - lDifference, m_tBackPos.y + lDifference);
		}
		else
		{
			m_pWeapon2->SetPoint(m_tFrontPos.x + lDifference, m_tFrontPos.y - lDifference);
			m_pWeapon1->SetPoint(m_tBackPos.x - lDifference, m_tBackPos.y + lDifference);
		}
		if (m_dwSwapTime + m_dwSwapStartTime < dwCurTime)
		{
			if (m_bWeapon1Front)
			{
				m_pWeapon1->SetPoint(m_tBackPos);
				m_pWeapon2->SetPoint(m_tFrontPos);
				m_bWeapon1Front = false;
			}
			else
			{
				m_pWeapon1->SetPoint(m_tFrontPos);
				m_pWeapon2->SetPoint(m_tBackPos);
				m_bWeapon1Front = true;
			}
			m_bSwapAnimation = true;
			m_bSwapCue = false;
		}
	}
}

int CPlayerUI::WindowListUpdate()
{
	int	iRet = 0;

	bool bIsOn = false;
	for (auto& iter = m_WindowList.begin(); iter != m_WindowList.end(); )
	{
		if (!(*iter)->GetLife())
		{
			SAFE_DELETE((*iter));
			iter = m_WindowList.erase(iter);
		}
		else
		{
			if ((*iter)->GetIsOn())
			{
				int iTemp = (*iter)->Update();
				if (iTemp)
					iRet = iTemp;
				bIsOn = true;
			}
			++iter;
		}
	}

	return iRet;
}

void CPlayerUI::StageTitleUpdate()
{
	m_tStageTitleInfo.fx += m_fStageTitleSpeed;

	switch (m_ePhase)
	{
	case STP_COME_IN:
		if (m_fStageTitleSpeed == 60.f)
		{
			if (WINCX / 2 < m_tStageTitleInfo.fx)
				m_fStageTitleSpeed -= 4.5f;
		}
		else if (m_fStageTitleSpeed < 60.f)
		{
			m_fStageTitleSpeed -= 4.5f;
			if (WINCX / 2 > m_tStageTitleInfo.fx)
			{
				m_fStageTitleSpeed = 0.f;
				m_tStageTitleInfo.fx = WINCX / 2;
				m_ePhase = STP_STOP;
				m_dwStageTitleStopTime = GetTickCount();
			}
		}
		break;
	case STP_STOP:
		if (m_dwStageTitleStopTime + 1000 < GetTickCount())
		{
			m_ePhase = STP_GO_OUT;
			m_fStageTitleSpeed = -60.f;
		}
		break;
	case STP_GO_OUT:
		m_fStageTitleSpeed += 4.5f;
		if (m_tStageTitleInfo.fx > WINCX)
		{
			m_ePhase = STP_END;
			m_bStageTitle = false;

		}

		break;
	case STP_END:
		break;
	default:
		break;
	}

}

void CPlayerUI::PlayerHpBarRender(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("PlayerLifeBack");
	SIZE tSize = GET_BITMAP_SIZE("PlayerLifeBack");
	GdiTransparentBlt(hDC, 10, 10, tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	hMemDC = FIND_BITMAP_DC("LifeBarFull");
	tSize = GET_BITMAP_SIZE("LifeBarFull");
	float fHpRatio = float(m_iHp) / m_iMaxHp;
	GdiTransparentBlt(hDC, 98, 22, static_cast<int>(tSize.cx * fHpRatio), tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	if (m_iHp < m_iMaxHp)
	{
		hMemDC = FIND_BITMAP_DC("LifeWave");
		GdiTransparentBlt(hDC, static_cast<int>(fHpRatio * tSize.cx) + 98, 22, 24, 40, hMemDC, 24 * m_iLifeWaveFrame, 0, 24, 40, RGB(255, 0, 255));
	}
	hMemDC = FIND_BITMAP_DC("PlayerLifeBase");
	tSize = GET_BITMAP_SIZE("PlayerLifeBase");
	GdiTransparentBlt(hDC, 10, 10, tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	string strLife = to_string(m_iHp) + "/" + to_string(m_iMaxHp);
	GET_SINGLE(CFontManager)->Render(hDC, 200, 20, FT_EDGE, 3.f, strLife, FPX_CENTER);

	GET_SINGLE(CFontManager)->Render(hDC, 57, 20, FT_EDGE, 3.f, to_string(m_iLevel), FPX_CENTER);

}

void CPlayerUI::PlayerDashBarRender(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("DashCountBase");

	SIZE tSize = GET_BITMAP_SIZE("DashCountBase");
	GdiTransparentBlt(hDC, 10, 80, 4 + (m_iMaxDash - 1) * 40, tSize.cy, hMemDC, 0, 0, 4 + (m_iMaxDash - 1) * 40, tSize.cy, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, 10 + 4 + (m_iMaxDash - 1) * 40, 80, 48, tSize.cy, hMemDC, tSize.cx - 48, 0, 48, tSize.cy, RGB(255, 0, 255));

	hMemDC = FIND_BITMAP_DC("DashCount");

	tSize = GET_BITMAP_SIZE("DashCount");
	int iRemainDash = m_iMaxDash - m_iDashCount;
	for (int i = 0; i < iRemainDash; ++i)
		GdiTransparentBlt(hDC, 18 + i * (tSize.cx + 4), 88, tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

}

void CPlayerUI::PlayerGoldRender(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("CoinIcon");
	SIZE tSize = GET_BITMAP_SIZE("CoinIcon");
	GdiTransparentBlt(hDC, 25, WINCY - 69, tSize.cx * 3, tSize.cy * 3, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
	int iPlayerGold = GET_SINGLE(CInventory)->GetPlayerGold();
	GET_SINGLE(CFontManager)->Render(hDC, 60, WINCY - 75, FT_NORMAL, 2.f, to_string(iPlayerGold));

}

void CPlayerUI::EquipSlotUIRender(HDC hDC)
{
	if (m_bWeapon1Front)
	{
		m_pWeapon2->Render(hDC);
		m_pWeapon1->Render(hDC);
	}
	else
	{
		m_pWeapon1->Render(hDC);
		m_pWeapon2->Render(hDC);
	}
}

void CPlayerUI::SkillSlotUIRender(HDC hDC)
{
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (!pPlayer)
		return;

	CWeapon* pWeapon = pPlayer->GetEquipWeapon();

	if (!pWeapon || !pWeapon->IsThereSkill())
		return;

	POINT tSkillUI = { WINCX - 200, WINCY - 50 };

	HDC hMemDC = FIND_BITMAP_DC("SkillUISlot");

	SIZE tSize = GET_BITMAP_SIZE("SkillUISlot");

	GdiTransparentBlt(hDC, 
		tSkillUI.x - tSize.cx / 2, 
		tSkillUI.y - tSize.cy / 2, 
		tSize.cx, tSize.cy,
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	if (!pWeapon->CanUseSkill())
	{
		hMemDC = FIND_BITMAP_DC("SkillUICool");

		float fCoolRatio = float(GetTickCount() - pWeapon->GetSkillCool()) / pWeapon->GetSkillCoolMax();
		if (fCoolRatio >= 1.f)
			fCoolRatio = 1.f;

		GdiTransparentBlt(hDC, 
			tSkillUI.x - tSize.cx / 2, 
			tSkillUI.y - tSize.cy / 2,
			static_cast<int>(tSize.cx * (1.f - fCoolRatio)), tSize.cy,
			hMemDC, 0, 0, 
			static_cast<int>(tSize.cx * (1.f - fCoolRatio))
			, tSize.cy, RGB(255, 0, 255));
	}

	hMemDC = FIND_BITMAP_DC(pWeapon->GetSkillIcon());
	
	tSize = GET_BITMAP_SIZE(pWeapon->GetSkillIcon());

	GdiTransparentBlt(hDC, 
		static_cast<int>(tSkillUI.x - tSize.cx * 1.5f), 
		static_cast<int>(tSkillUI.y - tSize.cy * 1.5f),
		tSize.cx * 3, tSize.cy * 3, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	hMemDC = FIND_BITMAP_DC("Keyboard_Q");

	tSize = GET_BITMAP_SIZE("Keyboard_Q");

	GdiTransparentBlt(hDC,
		static_cast<int>(tSkillUI.x + tSize.cx / 2),
		static_cast<int>(tSkillUI.y + tSize.cy / 2),
		static_cast<int>(tSize.cx * 2),
		static_cast<int>(tSize.cy * 2),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
}

void CPlayerUI::StageTitleRender(HDC hDC)
{
	if (m_bStageTitle)
	{
		int iSizeX = GET_SINGLE(CFontManager)->GetFontTotalXSize(m_strStageTitle, FT_EDGE, 4.f);

		GET_SINGLE(CFontManager)->Render(hDC,
			m_tStageTitleInfo.fx - iSizeX * 0.5f, m_tStageTitleInfo.fy, FT_EDGE, 4.f, m_strStageTitle);
	}
}
