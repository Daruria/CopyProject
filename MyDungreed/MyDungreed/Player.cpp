#include "stdafx.h"
#include "Player.h"
#include "ScrollManager.h"
#include "EffectManager.h"
#include "Item.h"
#include "Weapon.h"
#include "MagicShield.h"
#include "ShortSword.h"
#include "Rod.h"
#include "Gun.h"
#include "MagicWand.h"
#include "Mouse.h"
#include "Inventory.h"
#include "InvenSlot.h"
#include "MapWindow.h"
#include "UIWindow.h"

#define BASIC_SPEED 5.f;

CPlayer::CPlayer():
	m_bGodMode(false),
	m_bIsActiveUpdate(false),
	m_bHittable(true),
	m_bRender(true),
	m_bCanAction(true),
	m_bInvincibility(false),
	m_bDir(true),
	m_bDust(true),
	m_bWeaponChange(true),
	m_bEquipedWeapon(true),
	m_bWeaponBack(true),
	m_bDoubleJump(false),
	m_bLongJump(false),
	m_iLongJumpCheck(false),
	m_fLongJumpForce(0.f),
	m_fLongJumpAccel(0.f),
	m_iLevel(0),
	m_iGold(0),
	m_dwHitTime(0),
	m_dwHitBlankTime(0),
	m_iExp(0),
	m_iExpMax(100)
{
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	INSERT_BITMAP("PlayerBasicIdleR", L"Image/Player/BasicSkin/PlayerIdleR.bmp");
	INSERT_BITMAP("PlayerBasicDieR", L"Image/Player/BasicSkin/PlayerDieR.bmp");
	INSERT_BITMAP("PlayerBasicRunR", L"Image/Player/BasicSkin/PlayerRunR.bmp");
	INSERT_BITMAP("PlayerBasicJumpR", L"Image/Player/BasicSkin/PlayerJumpR.bmp");

	INSERT_BITMAP("PlayerBasicHand", L"Image/Player/BasicSkin/PlayerHand.bmp");

	INSERT_BITMAP("PlayerBasicIdleL", L"Image/Player/BasicSkin/PlayerIdleL.bmp");
	INSERT_BITMAP("PlayerBasicDieL", L"Image/Player/BasicSkin/PlayerDieL.bmp");
	INSERT_BITMAP("PlayerBasicRunL", L"Image/Player/BasicSkin/PlayerRunL.bmp");
	INSERT_BITMAP("PlayerBasicJumpL", L"Image/Player/BasicSkin/PlayerJumpL.bmp");

	m_dwMotion = IDLE;
	SetDirRight();
	m_tHitBox = { 0.f, 26.5f, 50, 74 };
	m_dwFrameTime = GetTickCount();
	m_dwFrameMaxTime = 100;
	m_bJump = false;
	m_bPhysics = true;
	m_bPassible = false;
	m_bLongJump = false;
	m_iLongJumpCheck = 0;
	m_fLongJumpForce = 0.f;
	m_fLongJumpAccel = 0.5f;
	m_fSpeed = 5.f;
	m_fJumpForce = 13.f + m_fSpeed + 1.f;

	m_bLife = true;
	m_iLevel = 1;
	m_iGold = 1000;
	m_tCharInfo.fAttack = 3.f;
	m_tCharInfo.fArmor = 5.f;
	m_tCharInfo.iHPMax = 80;
	m_tCharInfo.iHP = m_tCharInfo.iHPMax;

	m_tCharActiveInfo.fAttack = 3.f;
	m_tCharActiveInfo.fArmor = 5.f;
	m_tCharActiveInfo.iHPMax = 80;
	m_tCharActiveInfo.iHP = m_tCharActiveInfo.iHPMax;

	m_tDash.fDashTime = 0.f;
	m_tDash.fDashSpeed = 30.f;
	m_tDash.iDashMax = 2;
	m_tDash.iDashCount = 0;
	m_tDash.fDashCoolDown = 0.f;
	m_tDash.fDashCoolTime = 10.f;

	m_bWeaponChange = true;
	m_bEquipedWeapon = true;
	m_bWeaponBack = true;
	m_sAttackBoxLifeTime = 3;

	m_vecEquip.assign(ES_END, nullptr);

	CShortSword* pShortSword = new CShortSword;
	pShortSword->Initialize();

	Equip(pShortSword, ES_WEAPON1);

	m_bInvincibility = false;
	m_bRender = true;
	m_dwHitBlankTime = GetTickCount();

	m_iExp = 0;
	m_iExpMax = 100;

	m_tStatus = { 0, 0, 0, 0, 0 };
}

int CPlayer::Update()
{
	if (m_bIsActiveUpdate)
		ActiveInfoUpdate();

	if (m_tCharActiveInfo.iHP <= 0)
	{
		m_tCharActiveInfo.iHP = 0;
		if(!m_bGodMode && m_dwMotion != DIE)
			Die();
	}
	if (!m_bLife)
	{
		if (m_bPhysics) Gravity();
		return 0;
	}

	if(!m_bJump && !m_bFalling && !m_tDash.bDash && m_dwMotion != DIE)
		Idle();

	if (m_bCanAction)
	{
		Direction() ? SetDirRight() : SetDirLeft();
		KeyInput();
	}

	//	인벤토리를 열었을 때 장비 변환이 가능하게 하기 위해 밖으로 뺐음.
	if (KEYDOWN(VK_TAB) && m_bWeaponChange)
	{
		m_bEquipedWeapon = !m_bEquipedWeapon;
		m_bWeaponChange = false;
		GET_SINGLE(CPlayerUI)->StartSwap();
		m_bIsActiveUpdate = true;
	}

	if (!m_bWeaponChange)
		m_bWeaponChange = GET_SINGLE(CPlayerUI)->IsPossibleToSwap();

	GET_SINGLE(CInventory)->EquipSlotUpdate(&m_vecEquip);
	GET_SINGLE(CInventory)->EquipedWeaponUpdate(m_bEquipedWeapon);

	if (m_bJump)				Jump();
	if (m_bPhysics)				Gravity();
	if (m_tDash.bDash)			Dashing();
	if (m_tDash.iDashCount > 0)	DashCool();

	EquipUpdate();

	CMoveObj::Update();	//	Animate();

	Blinking();

	if (m_bCanAction)
	{
		CScrollManager::SetScrollX(m_tInfo.fx);
		CScrollManager::SetScrollY(m_tInfo.fy);
	}
	CScrollManager::ScrollLock();

	return 0;
}

void CPlayer::LateUpdate()
{
	CMoveObj::LateUpdate();
}

void CPlayer::Render(HDC hDC)
{
	if (!m_bRender)
		return;

	EQUIP_SLOT eSlot = m_bEquipedWeapon ? ES_WEAPON1 : ES_WEAPON2;

	if (m_vecEquip[eSlot] && dynamic_cast<CWeapon*>(m_vecEquip[eSlot])->GetRenderBack())
		EquipRender(hDC);

	HDC hMemDC = MotionDC();
	
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	BOOL bRender = false;
	if (hMemDC)
		bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx / 2), 
			static_cast<int>(tPos.fy - m_tInfo.icy / 2),
			m_tInfo.icx, m_tInfo.icy, hMemDC,
			m_tInfo.icx * m_iFrame, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, static_cast<int>(tPos.fx - m_tInfo.icx / 2), static_cast<int>(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);

	if (m_vecEquip[eSlot] && !dynamic_cast<CWeapon*>(m_vecEquip[eSlot])->GetRenderBack())
		EquipRender(hDC);

	hMemDC = FIND_BITMAP_DC("PlayerBasicHand");
	SIZE tSize = GET_BITMAP_SIZE("PlayerBasicHand");
	tPos = CScrollManager::WorldToScreen(GetHitBoxWorldInfo());
	bRender = GdiTransparentBlt(hDC, 
		static_cast<int>(tPos.fx + 25 -(m_bDir * 50) - tSize.cx / 2), 
		static_cast<int>(tPos.fy + 15 - tSize.cy / 2),
		tSize.cx, tSize.cy, hMemDC,
		0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	eSlot = m_bEquipedWeapon ? ES_SHIELD1 : ES_SHIELD2;

	if (m_vecEquip[eSlot])
		m_vecEquip[eSlot]->Render(hDC);

#ifdef _DEBUG
	tPos = CScrollManager::WorldToScreen(GetHitBoxWorldInfo());
	CMath::DrawBox(hDC, static_cast<int>(tPos.fx - m_tHitBox.icx / 2), static_cast<int>(tPos.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);
	//if (m_bAttack)
	//{
	//	tPos = CScrollManager::WorldToScreen(m_tAttackBox);
	//	CMath::DrawBox(hDC, static_cast<int>(tPos.fx - m_tAttackBox.icx / 2), static_cast<int>(tPos.fy - m_tAttackBox.icy / 2),
	//		m_tAttackBox.icx, m_tAttackBox.icy);
	//}
#endif // _DEBUG

#ifdef _DEBUG 

	string strText = "Player X : ";
	INFO tInfo = m_tInfo;
	strText += to_string(tInfo.fx);
	TextOutA(hDC, 10, 500, strText.c_str(), strText.size());

	strText = "Player Y : ";
	strText += to_string(tInfo.fy);
	TextOutA(hDC, 10, 520, strText.c_str(), strText.size());

	//bool bPhysics = dynamic_cast<CMoveObj*>(CObjectManager::GetInst()->GetObjList(OT_PLAYER)->back())->GetPhysics();
	//bool bFalling = dynamic_cast<CMoveObj*>(CObjectManager::GetInst()->GetObjList(OT_PLAYER)->back())->GetFalling();
	//bool bJump = ((CPlayer*)(CObjectManager::GetInst()->GetObjList(OT_PLAYER)->back()))->GetJump();

	//strText = "Player bPhysics : ";
	//strText += bPhysics ? "True" : "False";
	//TextOutA(hDC, 10, 540, strText.c_str(), strText.size());

	//strText = "Player bFalling : ";
	//strText += bFalling ? "True" : "False";
	//TextOutA(hDC, 10, 560, strText.c_str(), strText.size());

	//strText = "Player bJump : ";
	//strText += bJump ? "True" : "False";
	//TextOutA(hDC, 10, 580, strText.c_str(), strText.size());


#endif

}

void CPlayer::Release()
{
	//for (auto pItem : m_vecEquip)
	//	SAFE_DELETE(pItem);
}

CWeapon * CPlayer::GetEquipWeapon()
{
	if (m_bEquipedWeapon)
		return dynamic_cast<CWeapon*>(m_vecEquip[ES_WEAPON1]);
	else
		return dynamic_cast<CWeapon*>(m_vecEquip[ES_WEAPON2]);
}

bool CPlayer::Equip(CItem * pItem)
{
	return false;
}

bool CPlayer::Equip(CItem * pItem, EQUIP_SLOT eSlot)
{
	if (!pItem)
	{
		m_vecEquip[eSlot] = nullptr;
		m_bIsActiveUpdate = true;
		return true;
	}
	if ((eSlot == ES_WEAPON1 || eSlot == ES_WEAPON2) && pItem->GetType() == IT_WEAPON)
	{
		m_vecEquip[eSlot] = pItem;
		m_bIsActiveUpdate = true;
		return true;
	}
	else if ((eSlot >= ES_ACCESSORIES1 && eSlot < ES_END) && pItem->GetType() == IT_ACCESSORIES)
	{
		m_vecEquip[eSlot] = pItem;
		m_bIsActiveUpdate = true;
		return true;
	}
	else if ((eSlot == ES_SHIELD1 || eSlot == ES_SHIELD2) && pItem->GetType() == IT_SHIELD)
	{
		m_vecEquip[eSlot] = pItem;
		m_bIsActiveUpdate = true;
		return true;
	}


	return false;
}

void CPlayer::Unequip(EQUIP_SLOT eSlot)
{
}

void CPlayer::Resetting()
{
	Recovery();

	m_bEquipedWeapon = true;
	m_bCanAction = true;
	m_bInvincibility = false;
	m_bHittable = true;

	Idle();
}

DWORD CPlayer::CurrentMotion()
{
	return m_dwMotion;
}

HDC CPlayer::MotionDC()
{
	if (m_bDir)
		switch (m_dwMotion)
		{
		case IDLE:	return FIND_BITMAP_DC("PlayerBasicIdleR");
		case DIE:	return FIND_BITMAP_DC("PlayerBasicDieR");
		case RUN:	return FIND_BITMAP_DC("PlayerBasicRunR");
		case JUMP:	return FIND_BITMAP_DC("PlayerBasicJumpR");
		}
	else
		switch (m_dwMotion)
		{
		case IDLE:	return FIND_BITMAP_DC("PlayerBasicIdleL");
		case DIE:	return FIND_BITMAP_DC("PlayerBasicDieL");
		case RUN:	return FIND_BITMAP_DC("PlayerBasicRunL");
		case JUMP:	return FIND_BITMAP_DC("PlayerBasicJumpL");
		}
	return nullptr;
}

bool CPlayer::Direction()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	return tPos.fx < pt.x;
}

void CPlayer::EquipUpdate()
{
	if (m_bEquipedWeapon)
	{
		if (m_vecEquip[ES_WEAPON1])
		{
			m_vecEquip[ES_WEAPON1]->SetInfo(m_tInfo.fx + m_tHitBox.fx, m_tInfo.fy + m_tHitBox.fy);
			m_vecEquip[ES_WEAPON1]->Update();
		}
		if (m_vecEquip[ES_SHIELD1])
		{
			m_vecEquip[ES_SHIELD1]->SetInfo(m_tInfo.fx + m_tHitBox.fx, m_tInfo.fy + m_tHitBox.fy);
			m_vecEquip[ES_SHIELD1]->Update();
		}
	}
	else
	{
		if (m_vecEquip[ES_WEAPON2])
		{
			m_vecEquip[ES_WEAPON2]->SetInfo(m_tInfo.fx + m_tHitBox.fx, m_tInfo.fy + m_tHitBox.fy);
			m_vecEquip[ES_WEAPON2]->Update();
		}
		if (m_vecEquip[ES_SHIELD2])
		{
			m_vecEquip[ES_SHIELD2]->SetInfo(m_tInfo.fx + m_tHitBox.fx, m_tInfo.fy + m_tHitBox.fy);
			m_vecEquip[ES_SHIELD2]->Update();
		}
	}

	for (size_t i = ES_ACCESSORIES1; i < ES_END; ++i)
		if (m_vecEquip[i]) m_vecEquip[i]->Update();
}

void CPlayer::EquipRender(HDC hDC)
{
	if (m_bEquipedWeapon)
	{
		if (m_vecEquip[ES_WEAPON1])
			m_vecEquip[ES_WEAPON1]->Render(hDC);
		
		if (m_vecEquip[ES_SHIELD1])
			m_vecEquip[ES_SHIELD1]->Render(hDC);
	}
	else
	{
		if (m_vecEquip[ES_WEAPON2])
			m_vecEquip[ES_WEAPON2]->Render(hDC);
		
		if (m_vecEquip[ES_SHIELD2])
			m_vecEquip[ES_SHIELD2]->Render(hDC);
	}
}

void CPlayer::MoveLeft()
{
	if (!m_bFalling && !m_tDash.bDash && !m_bJump && m_dwMotion != RUN)
	{
		m_dwMotion = RUN;
		m_iMaxFrame = 8;
	}
	m_iFrameDir = m_bDir ? -1 : 1;

	if (m_iFrame % 2)
	{
		if (!m_bDust) MakeDust();
	}
	else
		m_bDust = false;

	m_tInfo.fx -= m_fSpeed;
}

void CPlayer::MoveRight()
{
	if (!m_bFalling && !m_tDash.bDash && !m_bJump && m_dwMotion != RUN)
	{
		m_dwMotion = RUN;
		m_iMaxFrame = 8;
	}
	m_iFrameDir = m_bDir ? 1 : -1;

	if (m_iFrame % 2) 
	{ 
		if(!m_bDust) MakeDust(); 
	}
	else
		m_bDust = false;

	m_tInfo.fx += m_fSpeed;
}

void CPlayer::Idle()
{
	m_dwMotion = IDLE;
	m_iMaxFrame = 5;
}

void CPlayer::MakeDust()
{
	float fx = m_tInfo.fx;
	fx += m_bDir ? - m_tHitBox.icx / 2 : m_tHitBox.icx / 2;
	string strKey = m_bDir ? "DustR" : "DustL";
	wstring wstrPath = m_bDir ? L"Image/Effect/Dust/DustR.bmp" : L"Image/Effect/Dust/DustL.bmp";
	GET_SINGLE(CEffectManager)->AddEffect(strKey, wstrPath.c_str(),
		fx, m_tInfo.fy + m_tInfo.icy / 4, 64, 64, 6);
	m_bDust = true;
	GET_SINGLE(CSoundMgr)->PlaySound(L"step_lth1.wav", CSoundMgr::EFFECT);
}

void CPlayer::Attack()
{
	if (m_dwAttackCool + m_dwAttackCoolMax < GetTickCount())
	{
		int iWeapon = m_bEquipedWeapon ? ES_WEAPON1 : ES_WEAPON2;

		if (m_vecEquip[iWeapon])
		{
			m_bWeaponBack = dynamic_cast<CWeapon*>(m_vecEquip[iWeapon])->Attack(m_tCharActiveInfo.fAttack);
			m_tAttackBox = dynamic_cast<CWeapon*>(m_vecEquip[iWeapon])->GetAttackBox();
			m_bAttack = true;
		}
		m_dwAttackCool = GetTickCount();
	}
}

void CPlayer::Die()
{
	m_dwMotion = DIE;
	m_iFrame = 0;
	m_iMaxFrame = 1;
	m_iFrameDir = 1;
	m_bHittable = false;
	m_bCanAction = false;
	//m_bLife = false;
	//GET_SINGLE(CResult)->StopPlay();
	GET_SINGLE(CResult)->ShowResult();
	GET_SINGLE(CSoundMgr)->StopSound(CSoundMgr::BGM);
	GET_SINGLE(CSoundMgr)->PlaySound(L"dead.wav", CSoundMgr::EFFECT);
}

void CPlayer::Dash()
{
	if (m_tDash.iDashCount >= m_tDash.iDashMax)
		return;

	m_tDash.bDash		= true;
	m_bJump				= false;
	m_bPhysics			= false;
	if (m_tStatus.iSwiftness >= 20)
		m_bHittable		= false;
	m_bPassible			= true;
	m_bFalling			= true;
	m_fGravityTime		= 0.f;
	m_tDash.iDashCount++;

	POSITION tWorldMouse = CScrollManager::ScreenToWorld(CMouse::GetInst()->GetPoint());

	float fDirX = tWorldMouse.fx - m_tInfo.fx;
	float fDirY = tWorldMouse.fy - m_tInfo.fy;

	CMath::NormalizeXY(fDirX, fDirY);

	m_tDash.fDashX = fDirX;
	m_tDash.fDashY = fDirY;

	m_dwMotion = JUMP;
	m_iMaxFrame = 1;

	GET_SINGLE(CSoundMgr)->PlaySound(L"Jumping.wav", CSoundMgr::EFFECT);
}

void CPlayer::Dashing()
{
	m_tInfo.fx += m_tDash.fDashX * m_tDash.fDashSpeed;
	m_tInfo.fy += m_tDash.fDashY * m_tDash.fDashSpeed;
	m_tDash.fDashTime += 0.1f;
	if (m_tDash.fDashTime > 1.f)
	{
		m_tDash.bDash		= false;
		m_bPhysics			= true;
		if (m_tStatus.iSwiftness >= 20)
			m_bHittable		= true;
		m_bPassible			= false;
		m_bFalling			= true;
		m_tDash.fDashTime	= 0.f;
	}

	string strKey = m_bDir ? "PlayerSpectrumR" : "PlayerSpectrumL";
	wstring wstrPath = m_bDir ? L"Image/Player/BasicSkin/Dash/CharDashR.bmp" : L"Image/Player/BasicSkin/Dash/CharDashL.bmp";

	GET_SINGLE(CEffectManager)->AddEffectToPret(strKey, wstrPath.c_str(), m_tInfo.fx, m_tInfo.fy, 32, 32, 6, 30, 4.f);
}

void CPlayer::DashCool()
{
	m_tDash.fDashCoolDown += 0.1f;
	if (m_tDash.fDashCoolDown > m_tDash.fDashCoolTime)
	{
		--m_tDash.iDashCount;
		m_tDash.fDashCoolDown = 0.f;
	}
}

void CPlayer::LongJump()
{
	if (m_iLongJumpCheck < 20)
	{
		m_iLongJumpCheck++;
		m_fLongJumpAccel -= 0.01f;

		if (m_fLongJumpAccel <= 0)
			m_fLongJumpAccel = 0.f;

		m_fLongJumpForce += m_fLongJumpAccel;
		m_bLongJump = true;
	}
}

void CPlayer::KeyInput()
{
	if (KEYPRESS('S') && KEYDOWN(VK_SPACE))
	{
		m_bPhysics = true;
		m_bFalling = true;
		m_bPassible = true;
		m_dwMotion = JUMP;
		m_iMaxFrame = 1;
	}
	else //if (KEYUP(VK_SPACE))
		m_bPassible = false;

	if (!m_bJump && !m_bFalling && (KEYDOWN(VK_SPACE) || KEYDOWN('W')))
	{
		m_bJump = true;
		m_dwMotion = JUMP;
		m_iMaxFrame = 1;
		GET_SINGLE(CEffectManager)->AddEffect("JumpFX", L"Image/Effect/Jump/JumpFX.bmp",
			m_tInfo.fx, m_tInfo.fy + m_tInfo.icy * 0.3f, 9, 10, 5, 4.f);
		GET_SINGLE(CSoundMgr)->PlaySound(L"Jumping.wav", CSoundMgr::EFFECT);
	}

	if (m_bJump &&  KEYPRESS(VK_SPACE))
		LongJump();

	if (m_tStatus.iSwiftness >= 5 && m_bJump && m_bDoubleJump && KEYDOWN(VK_SPACE))
		DoubleJump();

	if (KEYPRESS('A'))							MoveLeft();
	if (KEYPRESS('D'))							MoveRight();
	if (KEYPRESS(VK_LBUTTON))					Attack();
	if (KEYDOWN(VK_RBUTTON) && !m_tDash.bDash)	Dash();

	EQUIP_SLOT eSlot = m_bEquipedWeapon ? ES_WEAPON1 : ES_WEAPON2;

	CWeapon* pWeapon = dynamic_cast<CWeapon*>(m_vecEquip[eSlot]);

	if (pWeapon && pWeapon->IsThereSkill() && KEYDOWN('Q'))
		pWeapon->UseSkill();
	

	if (KEYDOWN('M'))
	{
		CUIWindow* pUIWindow = GET_SINGLE(CPlayerUI)->FindUIWindow("Map");
		if (pUIWindow)
		{
			CMapWindow* pMapWindow = dynamic_cast<CMapWindow*>(pUIWindow);
			if (pMapWindow)
			{
				pMapWindow->SetIsOn(true);
				pMapWindow->SetbFastMove(false);
			}
		}
	}

	if (KEYUP('M'))
	{
		CUIWindow* pUIWindow = GET_SINGLE(CPlayerUI)->FindUIWindow("Map");
		if (pUIWindow)
		{
			CMapWindow* pMapWindow = dynamic_cast<CMapWindow*>(pUIWindow);
			if (pMapWindow)
			{
				pMapWindow->SetIsOn(false);
				pMapWindow->SetbFastMove(true);
			}
		}
	}

	if (KEYDOWN(VK_F5))
	{
		if (!GET_SINGLE(CInventory)->FindItemSlot("Lala's Magic Wand"))
		{
			CMagicWand* pWand = new CMagicWand;
			pWand->Initialize();

			GET_SINGLE(CInventory)->AddItem(pWand);
		}
	}
		 
	if (KEYDOWN(VK_F6))
	{
		if (m_iLevel > 1)
			--m_iLevel;
	}
	if (KEYDOWN(VK_F7))
	{
		++m_iLevel;
	}

	if (KEYDOWN(VK_F8))
	{
		m_bGodMode = !m_bGodMode;
		string strText = m_bGodMode ? "Can't Die!" : "Can Die!";
		GET_SINGLE(CEffectManager)->AddFontEffect(m_tInfo.fx, m_tInfo.fy, m_tInfo.fx, m_tInfo.fy - 100, 1.f, 2.f, strText, FT_EDGE);
	}
}

void CPlayer::Blinking()
{
	if (m_bInvincibility)
	{
		if (m_dwHitTime + 1000 < GetTickCount())
		{
			m_bInvincibility = false;
			m_bRender = true;
		}
		else
		{
			if (m_dwHitBlankTime + 100 < GetTickCount())
			{
				m_bRender = !m_bRender;
				m_dwHitBlankTime = GetTickCount();
			}
		}
	}
}

void CPlayer::DoubleJump()
{
	GET_SINGLE(CEffectManager)->AddEffect("DoubleJumpFX", L"Image/Effect/DoubleJump/DoubleJumpFX.bmp",
		m_tInfo.fx, m_tInfo.fy + m_tInfo.icy * 0.3f, 9, 9, 6, 4.f);
	m_bFalling = false;
	m_bLongJump = false;
	m_iLongJumpCheck = 0;
	m_fLongJumpForce = 0.f;
	m_fLongJumpAccel = 0.5f;
	m_fGravityTime = 0.f;
	m_bDoubleJump = false;
}

void CPlayer::ActiveInfoUpdate()
{
	EQUIP_SLOT eSlot = m_bEquipedWeapon ? ES_WEAPON1 : ES_WEAPON2;

	int iEquipWeaponAttack = 0;
	if (m_vecEquip[eSlot])
		iEquipWeaponAttack = GetEquipWeapon()->GetAttack();
	m_tCharActiveInfo.fAttack = m_tCharInfo.fAttack + iEquipWeaponAttack + m_tStatus.iWrath * 2;

	m_tCharActiveInfo.fArmor = m_tCharInfo.fArmor + m_tStatus.iPatience * 1.5f;


	float fHPRatio = float(m_tCharActiveInfo.iHP) / m_tCharActiveInfo.iHPMax;
	m_tCharActiveInfo.iHPMax = m_tCharInfo.iHPMax + m_tStatus.iGreed * 2;
	m_tCharActiveInfo.iHP = static_cast<int>(fHPRatio * m_tCharActiveInfo.iHPMax);
	
	m_fSpeed = (1.f + m_tStatus.iSwiftness * 0.005f) * BASIC_SPEED;
	
	DWORD dwItemAttackCoolMax = 100;

	if (m_vecEquip[eSlot])
		dwItemAttackCoolMax = GetEquipWeapon()->GetAttackCoolMax();

	if (m_tStatus.iSwiftness >= 10 && fHPRatio >= 0.8f)
		dwItemAttackCoolMax *= 0.9f;

	m_dwAttackCoolMax = static_cast<DWORD>(dwItemAttackCoolMax * (1.f - m_tStatus.iSwiftness * 0.005f));

	m_tDash.fDashCoolTime = m_tStatus.iSwiftness >= 10 ? 5.f : 10.f;

	int iDashPlus = 0;
	if (m_tStatus.iWrath		>= 20)	++iDashPlus;
	if (m_tStatus.iSwiftness	>= 5)	++iDashPlus;
	if (m_tStatus.iSwiftness	>= 20)	++iDashPlus;
	if (m_tStatus.iPatience		>= 20)	++iDashPlus;
	if (m_tStatus.iMystic		>= 20)	++iDashPlus;
	if (m_tStatus.iGreed		>= 20)	++iDashPlus;
	if (iDashPlus > 3)					iDashPlus = 3;
	
	m_tDash.iDashMax = 2 + iDashPlus;

	if (m_tStatus.iPatience >= 5)
	{
		CItem* pItem = nullptr;
		pItem = GET_SINGLE(CInventory)->FindInventoryItem("MagicShield");
		if (nullptr == pItem)
		{
			CMagicShield* pMagicShield = new CMagicShield;
			pMagicShield->Initialize();
			
			if (!m_vecEquip[ES_SHIELD1])
				Equip(pMagicShield, ES_SHIELD1);
			else if (!m_vecEquip[ES_SHIELD2])
				Equip(pMagicShield, ES_SHIELD2);
			else
				GET_SINGLE(CInventory)->AddItem(pMagicShield);
		}
	}
	else
	{
		CInvenSlot* pSlot = nullptr;
		pSlot = GET_SINGLE(CInventory)->FindItemSlot("MagicShield");

		if (pSlot)
		{
			CItem* pItem = pSlot->GetItem();
			pSlot->SetItem(nullptr);
			SAFE_DELETE(pItem);
			GET_SINGLE(CInventory)->ApplyPlayerEquip();
		}
	}


	m_bIsActiveUpdate = false;
}

void CPlayer::Stop()
{
	if (m_bJump)
		MakeDust();
	m_bJump			= false; 
	m_bFalling		= false; 
	m_bLongJump		= false;
	m_bDoubleJump	= true;
	m_iLongJumpCheck= 0;
	m_fLongJumpForce= 0.f;
	m_fLongJumpAccel= 0.5f;
	m_fGravityTime	= 0.f;
}

void CPlayer::Jump()
{
	m_tInfo.fy -= (m_fJumpForce + m_bLongJump * m_fLongJumpForce);
}

void CPlayer::Hit(const float fAttack, const INFO & tAttackerInfo)
{
	if (!m_bInvincibility)
	{
		CMoveObj::Hit(fAttack, tAttackerInfo);
		float fDamage = fAttack - m_tCharActiveInfo.fArmor;
		if (fDamage <= 0.f) fDamage = 1.f;
		m_tCharActiveInfo.iHP -= static_cast<int>(fDamage);
		m_bInvincibility = true;
		m_dwHitTime = GetTickCount();
		m_dwHitBlankTime = GetTickCount();
		m_bRender = false;
		GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Player.wav", CSoundMgr::EFFECT);
	}
}

void CPlayer::AddAttackEffect(float fx, float fy)
{
	if (m_bEquipedWeapon)	//
		dynamic_cast<CWeapon*>(m_vecEquip[ES_WEAPON1])->AddAttackEffect(fx, fy);
	else
		dynamic_cast<CWeapon*>(m_vecEquip[ES_WEAPON2])->AddAttackEffect(fx, fy);
}

void CPlayer::WrathUp()
{
	if (m_tStatus.iWrath < 20)
		++m_tStatus.iWrath;
	m_bIsActiveUpdate = true;
}

void CPlayer::SwiftnessUp()
{
	if (m_tStatus.iSwiftness < 20)
		++m_tStatus.iSwiftness;
	m_bIsActiveUpdate = true;
}

void CPlayer::PatienceUp()
{
	if (m_tStatus.iPatience < 20)
		++m_tStatus.iPatience;
	m_bIsActiveUpdate = true;
}

void CPlayer::MysticUp()
{
	if (m_tStatus.iMystic < 20)
		++m_tStatus.iMystic;
	m_bIsActiveUpdate = true;
}

void CPlayer::GreedUp()
{
	if (m_tStatus.iGreed < 20)
		++m_tStatus.iGreed;
	m_bIsActiveUpdate = true;
}

void CPlayer::StatusReset()
{
	ZeroMemory(&m_tStatus, sizeof(m_tStatus));

	m_bIsActiveUpdate = true;
}

const int CPlayer::GetUsedPoint()
{
	int iUsedPoint = 0;

	iUsedPoint += m_tStatus.iWrath;
	iUsedPoint += m_tStatus.iSwiftness;
	iUsedPoint += m_tStatus.iPatience;
	iUsedPoint += m_tStatus.iMystic;
	iUsedPoint += m_tStatus.iGreed;

	return iUsedPoint;
}
