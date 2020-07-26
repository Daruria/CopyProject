#include "stdafx.h"
#include "AbilityWindow.h"
#include "BGImage.h"
#include "MyButton.h"
#include "Player.h"
#include "AbilityPerk.h"

CAbilityWindow::CAbilityWindow():
	m_pWrathButton(nullptr),
	m_pSwiftnessButton(nullptr),
	m_pPatienceButton(nullptr),
	m_pMysticButton(nullptr),
	m_pGreedButton(nullptr)
{
}


CAbilityWindow::~CAbilityWindow()
{
	Release();
}

void CAbilityWindow::Initialize()
{
	INSERT_BITMAP("AbilityTextBar", L"Image/UI/Ability/AbilityTextBar.bmp");
	INSERT_BITMAP("WindowBack", L"Image/UI/Rect/WindowBack.bmp");
	INSERT_BITMAP("Keyboard_R", L"Image/Keyboard/Keyboard_R.bmp");
	INSERT_BITMAP("a5", L"Image/UI/Ability/Icon/Level/a5.bmp");
	INSERT_BITMAP("a10", L"Image/UI/Ability/Icon/Level/a10.bmp");
	INSERT_BITMAP("a20", L"Image/UI/Ability/Icon/Level/a20.bmp");

	ImageListSetting();
	ButtonSetting();
	PerkSetting();
	m_bLife = true;
}

void CAbilityWindow::Initialize(const string & strKey, const wchar_t * pPath)
{
}

int CAbilityWindow::Update()
{
	if (m_bIsOn)
		GET_SINGLE(CMouse)->ChangeCursor(0);

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (nullptr == pPlayer)
		return 0;

	int iUsedPoint = pPlayer->GetUsedPoint();
	int iPlayerLevel = pPlayer->GetLevel();

	m_iAbilityPoint = iPlayerLevel - iUsedPoint;

	ButtonsUpdate();

	PerkUpdate();

	if (KEYDOWN('R'))
	{
		GET_SINGLE(CObjectManager)->GetPlayer()->StatusReset();
	}

	if (KEYDOWN(VK_ESCAPE))
	{
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
		GET_SINGLE(CMouse)->ChangeCursor(1);
		m_bIsOn = false;
	}
	
	return 0;
}

void CAbilityWindow::LateUpdate()
{
}

void CAbilityWindow::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("WindowBack");

	SIZE tSize = GET_BITMAP_SIZE("WindowBack");

	GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	for (auto pImage : m_BGList)
		pImage->RenderWithoutScroll(hDC);

	m_pWrathButton->Render(hDC);
	m_pSwiftnessButton->Render(hDC);
	m_pPatienceButton->Render(hDC);
	m_pMysticButton->Render(hDC);
	m_pGreedButton->Render(hDC);

	AbilityTextRender(hDC);

	for (auto pPerk : m_vecPerk)
		pPerk->Render(hDC);

	hMemDC = FIND_BITMAP_DC("Keyboard_R");
	tSize = GET_BITMAP_SIZE("Keyboard_R");

	GdiTransparentBlt(hDC,
		static_cast<int>(WINCX / 2 - tSize.cx * 3.f),
		static_cast<int>(WINCY - 5 - tSize.cy * 3.f),
		static_cast<int>(tSize.cx * 3.f),
		static_cast<int>(tSize.cy * 3.f),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 2 + 50.f, WINCY - 5 - tSize.cy * 1.5f, FT_EDGE, 2.f, "Reset", FPX_CENTER, FPY_MIDDLE);

	RemainPointRender(hDC);
}

void CAbilityWindow::Release()
{
	for (auto& pPerk : m_vecPerk)
		SAFE_DELETE(pPerk);

	m_vecPerk.clear();

	SAFE_DELETE(m_pWrathButton);
	SAFE_DELETE(m_pSwiftnessButton);
	SAFE_DELETE(m_pPatienceButton);
	SAFE_DELETE(m_pMysticButton);
	SAFE_DELETE(m_pGreedButton);

	for (auto& pImage : m_BGList)
		SAFE_DELETE(pImage);

	m_BGList.clear();
}

void CAbilityWindow::WindowOn()
{
	CUIWindow::WindowOn();
}

void CAbilityWindow::WindowOff()
{
	CUIWindow::WindowOff();
}

void CAbilityWindow::ImageListSetting()
{
	INSERT_BITMAP("AbilityTItle", L"Image/UI/Ability/AbilityBackground/AbilityTItle.bmp");

	INSERT_BITMAP("AbilityBackground_Wrath",	L"Image/UI/Ability/AbilityBackground/AbilityBackground_Wrath.bmp");
	INSERT_BITMAP("AbilityBackground_Swiftness", L"Image/UI/Ability/AbilityBackground/AbilityBackground_Swiftness.bmp");
	INSERT_BITMAP("AbilityBackground_Patience", L"Image/UI/Ability/AbilityBackground/AbilityBackground_Patience.bmp");
	INSERT_BITMAP("AbilityBackground_Arcane",	L"Image/UI/Ability/AbilityBackground/AbilityBackground_Arcane.bmp");
	INSERT_BITMAP("AbilityBackground_Greed",	L"Image/UI/Ability/AbilityBackground/AbilityBackground_Greed.bmp");


	AddBGImage("AbilityTItle",					L"Image/UI/Ability/AbilityBackground/AbilityTItle.bmp",					WINCX / 2.f, 16 * 3.2f, 320, 32, 3.2f);
	
	AddBGImage("AbilityBackground_Wrath",		L"Image/UI/Ability/AbilityBackground/AbilityBackground_Wrath.bmp",		WINCX / 5 * 0.53f,	WINCY / 2.f - 117 * 1.6f + 120, 66, 117, 3.2f);
	AddBGImage("AbilityBackground_Swiftness",	L"Image/UI/Ability/AbilityBackground/AbilityBackground_Swiftness.bmp",	WINCX / 5 * 1.53f,	WINCY / 2.f - 119 * 1.6f + 120, 60, 119, 3.2f);
	AddBGImage("AbilityBackground_Patience",	L"Image/UI/Ability/AbilityBackground/AbilityBackground_Patience.bmp",	WINCX / 5 * 2.53f,	WINCY / 2.f - 111 * 1.6f + 120, 58, 111, 3.2f);
	AddBGImage("AbilityBackground_Arcane",		L"Image/UI/Ability/AbilityBackground/AbilityBackground_Arcane.bmp",		WINCX / 5 * 3.53f,	WINCY / 2.f - 114 * 1.6f + 120, 58, 114, 3.2f);
	AddBGImage("AbilityBackground_Greed",		L"Image/UI/Ability/AbilityBackground/AbilityBackground_Greed.bmp",		WINCX / 5 * 4.53f,	WINCY / 2.f - 116 * 1.6f + 120, 62, 116, 3.2f);
}

void CAbilityWindow::ButtonSetting()
{
	m_pWrathButton = new CMyButton;
	m_pWrathButton->Initialize("AbilityBackgroundButtonDisable_Wrath", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButtonDisable_Wrath.bmp",
		"AbilityBackgroundButton_Wrath", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButton_Wrath.bmp");
	m_pWrathButton->SetPoint(static_cast<int>(WINCX / 5 * 0.53f), WINCY / 2 + 87);
	m_pWrathButton->SetRatio(3.2f);

	m_pSwiftnessButton = new CMyButton;
	m_pSwiftnessButton->Initialize("AbilityBackgroundButtonDisable_Swiftness", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButtonDisable_Swiftness.bmp",
		"AbilityBackgroundButton_Swiftness", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButton_Swiftness.bmp");
	m_pSwiftnessButton->SetPoint(static_cast<int>(WINCX / 5 * 1.53f), WINCY / 2 + 87);
	m_pSwiftnessButton->SetRatio(3.2f);

	m_pPatienceButton = new CMyButton;
	m_pPatienceButton->Initialize("AbilityBackgroundButtonDisable_Patience", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButtonDisable_Patience.bmp",
		"AbilityBackgroundButton_Patience", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButton_Patience.bmp");
	m_pPatienceButton->SetPoint(static_cast<int>(WINCX / 5 * 2.53f), WINCY / 2 + 87);
	m_pPatienceButton->SetRatio(3.2f);

	m_pMysticButton = new CMyButton;
	m_pMysticButton->Initialize("AbilityBackgroundButtonDisable_Arcane", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButtonDisable_Arcane.bmp",
		"AbilityBackgroundButton_Arcane", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButton_Arcane.bmp");
	m_pMysticButton->SetPoint(static_cast<int>(WINCX / 5 * 3.53f), WINCY / 2 + 87);
	m_pMysticButton->SetRatio(3.2f);

	m_pGreedButton = new CMyButton;
	m_pGreedButton->Initialize("AbilityBackgroundButtonDisable_Greed", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButtonDisable_Greed.bmp",
		"AbilityBackgroundButton_Greed", L"Image/UI/Ability/AbilityBackground/Buttons/AbilityBackgroundButton_Greed.bmp");
	m_pGreedButton->SetPoint(static_cast<int>(WINCX / 5 * 4.53f), WINCY / 2 + 87);
	m_pGreedButton->SetRatio(3.2f);
}

void CAbilityWindow::PerkSetting()
{
	m_vecPerk.reserve(15);

	//	Wrath Perk
	CAbilityPerk* pPerk = new CAbilityPerk;
	pPerk->Initialize("Wrath0", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon0_0_Disabled.bmp", "Wrath0On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon0_0_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 0.53f - 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::FIVE);
	pPerk->AddDescription("Deal up to 8 to surrounding enemies when jumping.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Wrath1", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon0_1_Disabled.bmp", "Wrath1On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon0_1_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 0.53f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TEN);
	pPerk->AddDescription("Killing an enemy increases power By 10 for 15 seconds.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Wrath2", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon0_2_Disabled.bmp", "Wrath2On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon0_2_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 0.53f + 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TWENTY);
	pPerk->AddDescription("Deals maximum weapon damage when HP is below 60%.");
	pPerk->AddDescription("Dash charges + 1");
	m_vecPerk.push_back(pPerk);

	//	Swiftness Perk
	pPerk = new CAbilityPerk;
	pPerk->Initialize("Swiftness0", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon1_0_Disabled.bmp", "Swiftness0On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon1_0_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 1.53f - 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::FIVE);
	pPerk->AddDescription("Gain the ability to do a double jump.");
	pPerk->AddDescription("Dash charges + 1");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Swiftness1", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon1_1_Disabled.bmp", "Swiftness1On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon1_1_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 1.53f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TEN);
	pPerk->AddDescription("Attack speed increases by 10% when HP is above 80%.");
	pPerk->AddDescription("Dash recharge speed increases");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Swiftness2", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon1_2_Disabled.bmp", "Swiftness2On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon1_2_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 1.53f + 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TWENTY);
	pPerk->AddDescription("Become Invincible for 0.2 seconds when Dashing.");
	pPerk->AddDescription("Dash charges + 1");
	m_vecPerk.push_back(pPerk);

	//	Patience Perk

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Patience0", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon2_0_Disabled.bmp", "Patience0On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon2_0_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 2.53f - 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::FIVE);
	pPerk->AddDescription("Receive magic shield.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Patience1", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon2_1_Disabled.bmp", "Patience1On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon2_1_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 2.53f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TEN);
	pPerk->AddDescription("When receiving deadly damage, grant invincibility 4 seconds.");
	pPerk->AddDescription("(Only Activates Once in the dungeon.)");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Patience2", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon2_2_Disabled.bmp", "Patience2On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon2_2_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 2.53f + 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TWENTY);
	pPerk->AddDescription("When HP is below 30%, gain HP regeneration and Increases defense.");
	pPerk->AddDescription("Dash charges + 1");
	m_vecPerk.push_back(pPerk);

	//	Mystic Perk
	pPerk = new CAbilityPerk;
	pPerk->Initialize("Mystic0", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon3_0_Disabled.bmp", "Mystic0On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon3_0_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 3.53f - 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::FIVE);
	pPerk->AddDescription("Items in the shop are 30% cheaper.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Mystic1", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon3_1_Disabled.bmp", "Mystic1On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon3_1_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 3.53f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TEN);
	pPerk->AddDescription("Cool down time of item skills decrease by 40%.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Mystic2", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon3_2_Disabled.bmp", "Mystic2On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon3_2_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 3.53f + 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TWENTY);
	pPerk->AddDescription("When slain, allows the adventurer to take one item back.");
	pPerk->AddDescription("Dash charges + 1");
	m_vecPerk.push_back(pPerk);

	//	Greed Perk
	pPerk = new CAbilityPerk;
	pPerk->Initialize("Greed0", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon4_0_Disabled.bmp", "Greed0On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon4_0_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 4.53f - 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::FIVE);
	pPerk->AddDescription("Earn 20% more gold.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Greed1", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon4_1_Disabled.bmp", "Greed1On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon4_1_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 4.53f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TEN);
	pPerk->AddDescription("Increase Maximum satiety level by 25 allowing you to eat more.");
	m_vecPerk.push_back(pPerk);

	pPerk = new CAbilityPerk;
	pPerk->Initialize("Greed2", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon4_2_Disabled.bmp", "Greed2On", L"Image/UI/Ability/Icon/BMP/AbilityPerkIcon4_2_On.bmp");
	pPerk->SetPoint(static_cast<int>(WINCX / 5 * 4.53f + 14 * 3.2f), static_cast<int>(WINCY / 2 + 30));
	pPerk->SetRatio(3.2f);
	pPerk->SetPerkLevel(CAbilityPerk::TWENTY);
	pPerk->AddDescription("Satiety levels are decreased by up to 10% when yor eating food.");
	pPerk->AddDescription("Gain one more accessory slot.");
	pPerk->AddDescription("Dash charges + 1");
	m_vecPerk.push_back(pPerk);
}

void CAbilityWindow::AddBGImage(const string& strKey, const wchar_t* pPath, float fx, float fy, int icx, int icy, float fRatio)
{
	CBGImage* pBGImage = new CBGImage;
	pBGImage->Initialize();
	pBGImage->SetImage(strKey, pPath);
	pBGImage->SetRatio(fRatio);
	pBGImage->SetInfo(fx, fy, icx, icy);
	m_BGList.push_back(pBGImage);
}

void CAbilityWindow::ButtonsUpdate()
{
	if (m_pWrathButton->Update())
	{
		if (m_iAbilityPoint > 0)
		{
			--m_iAbilityPoint;
			GET_SINGLE(CObjectManager)->GetPlayer()->WrathUp();
			GET_SINGLE(CSoundMgr)->PlaySound(L"Fantasy_Game_UI_Fire_Select_ability.wav", CSoundMgr::EFFECT);
		}
	}

	if (m_pSwiftnessButton->Update())
	{
		if (m_iAbilityPoint > 0)
		{
			--m_iAbilityPoint;
			GET_SINGLE(CObjectManager)->GetPlayer()->SwiftnessUp();
			GET_SINGLE(CSoundMgr)->PlaySound(L"Fantasy_Game_UI_Fire_Select_ability.wav", CSoundMgr::EFFECT);
		}
	}

	if (m_pPatienceButton->Update())
	{
		if (m_iAbilityPoint > 0)
		{
			--m_iAbilityPoint;
			GET_SINGLE(CObjectManager)->GetPlayer()->PatienceUp();
			GET_SINGLE(CSoundMgr)->PlaySound(L"Fantasy_Game_UI_Fire_Select_ability.wav", CSoundMgr::EFFECT);
		}
	}

	if (m_pMysticButton->Update())
	{
		if (m_iAbilityPoint > 0)
		{
			--m_iAbilityPoint;
			GET_SINGLE(CObjectManager)->GetPlayer()->MysticUp();
			GET_SINGLE(CSoundMgr)->PlaySound(L"Fantasy_Game_UI_Fire_Select_ability.wav", CSoundMgr::EFFECT);
		}
	}

	if (m_pGreedButton->Update())
	{
		if (m_iAbilityPoint > 0)
		{
			--m_iAbilityPoint;
			GET_SINGLE(CObjectManager)->GetPlayer()->GreedUp();
			GET_SINGLE(CSoundMgr)->PlaySound(L"Fantasy_Game_UI_Fire_Select_ability.wav", CSoundMgr::EFFECT);
		}
	}
}

void CAbilityWindow::PerkUpdate()
{
	for (auto& pPerk : m_vecPerk)
		pPerk->Update();

	STATUS tStatus = GET_SINGLE(CObjectManager)->GetPlayer()->GetStatus();

	PerkActiveUpdate(tStatus.iWrath, 0);
	PerkActiveUpdate(tStatus.iSwiftness, 3);
	PerkActiveUpdate(tStatus.iPatience, 6);
	PerkActiveUpdate(tStatus.iMystic, 9);
	PerkActiveUpdate(tStatus.iGreed, 12);
}

void CAbilityWindow::PerkActiveUpdate(int iPerk, int iStartPerk)
{
	int iArr[3] = { 5, 10, 20 };

	for (int i = 0; i < 3; ++i)
	{
		if (iPerk >= iArr[i])
			m_vecPerk[iStartPerk + i]->SetActive(true);
		else
			m_vecPerk[iStartPerk + i]->SetActive(false);
	}
}

void CAbilityWindow::AbilityTextRender(HDC hDC)
{
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 0.53f, WINCY / 2 - 145, FT_NORMAL, 2.5f, "Wrath",		FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 1.53f, WINCY / 2 - 145, FT_NORMAL, 2.5f, "Swiftness", FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 2.53f, WINCY / 2 - 145, FT_NORMAL, 2.5f, "Patience",	FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 3.53f, WINCY / 2 - 145, FT_NORMAL, 2.5f, "Mystic",	FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 4.53f, WINCY / 2 - 145, FT_NORMAL, 2.5f, "Greed",		FPX_CENTER, FPY_MIDDLE);

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	STATUS tStatus = pPlayer->GetStatus();

	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 0.53f, WINCY / 2 - 100, FT_FIELD, 3.f, to_string(tStatus.iWrath),	FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 1.53f, WINCY / 2 - 100, FT_FIELD, 3.f, to_string(tStatus.iSwiftness),FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 2.53f, WINCY / 2 - 100, FT_FIELD, 3.f, to_string(tStatus.iPatience), FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 3.53f, WINCY / 2 - 100, FT_FIELD, 3.f, to_string(tStatus.iMystic),	FPX_CENTER, FPY_MIDDLE);
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 4.53f, WINCY / 2 - 100, FT_FIELD, 3.f, to_string(tStatus.iGreed),	FPX_CENTER, FPY_MIDDLE);

	float fRatio = 1.5;

	int iPower = tStatus.iWrath * 2;
	string strText = "+" + to_string(iPower) + " Power";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 0.53f, WINCY / 2 - 50, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

	int iSpeedInt = tStatus.iSwiftness / 2;
	int iSpeedDot = (tStatus.iSwiftness % 2) * 5;
	strText = "+" + to_string(iSpeedInt) + "." + to_string(iSpeedDot) + "% Move Speed";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 1.53f, WINCY / 2 - 50, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

	strText = "+" + to_string(iSpeedInt) + "." + to_string(iSpeedDot) + "% Atk Speed";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 1.53f, WINCY / 2 - 30, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

	int iDefenseInt = tStatus.iPatience * 3 / 2;
	int iDefenseDot = ((tStatus.iPatience * 3) % 2) * 5;

	strText = "+" + to_string(iDefenseInt) + "." + to_string(iDefenseDot) + " Defense";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 2.53f, WINCY / 2 - 50, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

	int iMysticInt = tStatus.iMystic / 2;
	int iMysticDot = (tStatus.iMystic % 2) * 5;
	strText = "+" + to_string(iMysticInt) + "." + to_string(iMysticDot) + "% Critical";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 3.53f, WINCY / 2 - 50, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

	strText = "+" + to_string(iMysticInt) + "." + to_string(iMysticDot) + "% Evasion";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 3.53f, WINCY / 2 - 30, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

	int iHp = tStatus.iGreed * 2;
	strText = "+" + to_string(iHp) + " HPMax";
	GET_SINGLE(CFontManager)->Render(hDC, WINCX / 5 * 4.53f, WINCY / 2 - 50, FT_NORMAL, fRatio, strText, FPX_CENTER, FPY_MIDDLE);

}

void CAbilityWindow::RemainPointRender(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("AbilityTextBar");
	SIZE tSize = GET_BITMAP_SIZE("AbilityTextBar");
	string strText = " Remaining Points : " + to_string(m_iAbilityPoint) + " ";

	int iFontSizeX = GET_SINGLE(CFontManager)->GetFontTotalXSize(strText, FT_NORMAL, 2.f);
	int iFontSizeY = GET_SINGLE(CFontManager)->GetFontTotalYSize(strText, FT_NORMAL, 2.f);
	int iEdge = 4;
	float fRatio = 3.f;
	
	int iX = static_cast<int>(WINCX - iFontSizeX - 4 * fRatio * 2);
	int iY = static_cast<int>(WINCY - iFontSizeY - 4 * fRatio * 2);

	int iArrCX[3] = {static_cast<int>(4 * fRatio), iFontSizeX, static_cast<int>(4 * fRatio)};
	int iArrCY[3] = {static_cast<int>(4 * fRatio), iFontSizeY, static_cast<int>(4 * fRatio)};
	int iArrX[3] = {iX, iX + iArrCX[0], iX + iArrCX[0] + iArrCX[1]};
	int iArrY[3] = {iY, iY + iArrCY[0], iY + iArrCY[0] + iArrCY[1]};

	int iArrSrcCX[3] = {4, tSize.cx - 4 * 2, 4};
	int iArrSrcCY[3] = {4, tSize.cy - 4 * 2, 4};
	int iArrSrcX[3] = {0, iArrSrcCX[0], iArrSrcCX[0] + iArrSrcCX[1]};
	int iArrSrcY[3] = {0, iArrSrcCY[0], iArrSrcCY[0] + iArrSrcCY[1]};

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
			GdiTransparentBlt(hDC, iArrX[x], iArrY[y], iArrCX[x], iArrCY[y], hMemDC,
				iArrSrcX[x], iArrSrcY[y], iArrSrcCX[x], iArrSrcCY[y], RGB(255, 0, 255));
	}

	GET_SINGLE(CFontManager)->Render(hDC, iArrX[1] + iArrCX[1] * 0.5f, iArrY[1] + iArrCY[1] * 0.5f,
		FT_NORMAL, 2.f, strText, FPX_CENTER, FPY_MIDDLE);

}
