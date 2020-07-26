#include "stdafx.h"
#include "Inventory.h"
#include "MyButton.h"
#include "Mouse.h"
#include "Item.h"
#include "InvenSlot.h"
#include "ObjectManager.h"
#include "Player.h"
//#include "FontNumber.h"
#include "PlayerUI.h"

DEFINITION_SINGLE(CInventory);

CInventory::CInventory():
	m_bInvenOn(false),
	m_bEquipedWeapon(true),
	m_iPlayerGold(0),
	m_pExitButton(nullptr),
	m_tRect(RECT{}),
	m_pClickedSlot(nullptr)
{
}

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	m_bInvenOn = false;

	INSERT_BITMAP("Inventory", L"Image/Inventory/InventoryBase(x3).bmp");
	INSERT_BITMAP("accessoryZoneWideVer", L"Image/Inventory/accessoryZoneWideVer.bmp");

	INSERT_BITMAP("Keyboard_Tab", L"Image/Keyboard/Keyboard_Tab.bmp");

	SIZE tSize = GET_BITMAP_SIZE("Inventory");
	m_tRect = { WINCX - tSize.cx, 80, WINCX, 80 + tSize.cy };

	//	닫기 버튿 초기화
	m_pExitButton = new CMyButton;
	m_pExitButton->Initialize("InvenExit", L"Image/Inventory/InventoryExit.bmp", "InvenExit", L"Image/Inventory/InventoryExit.bmp");
	m_pExitButton->SetPoint(m_tRect.left - 27, m_tRect.top + 28);
	m_pExitButton->SetSize(54, 56);

	m_vecSlot.assign(15, nullptr);

	//	인벤 슬롯 초기화
	int iSlotStartX = m_tRect.left + 30;
	int iSlotStartY = m_tRect.top + 280;
	for (size_t i = 0; i < 15; ++i)
	{
		int iX = i % 5;
		int iY = i / 5;
		if (!m_vecSlot[i])
		{
			m_vecSlot[i] = new CInvenSlot;
			m_vecSlot[i]->SetSlotType(IST_INVEN);
			m_vecSlot[i]->Initialize("InvenItemSlot", L"Image/Inventory/IconGray.bmp", "InvenItemSlotOn", L"Image/Inventory/IconWhite.bmp");
			tSize = GET_BITMAP_SIZE("InvenItemSlot");
			m_vecSlot[i]->SetPoint(iSlotStartX + (tSize.cx + 10) * iX + tSize.cx / 2, iSlotStartY + (tSize.cy + 8) * iY + tSize.cy / 2);
		}
	}

	INSERT_BITMAP("EquipSlot1", L"Image/Inventory/EquipSlot1(x3).bmp");
	INSERT_BITMAP("EquipSlot1On", L"Image/Inventory/EquipSlot1On(x3).bmp");
	INSERT_BITMAP("EquipSlot2", L"Image/Inventory/EquipSlot2(x3).bmp");
	INSERT_BITMAP("EquipSlot2On", L"Image/Inventory/EquipSlot2On(x3).bmp");

	m_iPlayerGold = 1000;

	//	장비 슬롯 초기화
	EquipSlotSetting();

	m_pClickedSlot = nullptr;
}

int CInventory::Update()
{
	if (KEYDOWN('V'))
		ToggleInven();

	if (!m_bInvenOn)
	{
		GET_SINGLE(CMouse)->ChangeCursor(1);
		return 0;
	}
	else
		GET_SINGLE(CMouse)->ChangeCursor(0);

	if (m_pExitButton->Update())
	{
		m_bInvenOn = false;
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
	}

	//	장비 슬롯 업데이트
	for (auto pEquipSlot : m_vecEquipSlot)
	{
		if (!pEquipSlot)
			continue;
		if (pEquipSlot->Update())
			SlotUpdate(pEquipSlot);
	}

	//	인벤 슬롯 업데이트
	for (auto pInvenSlot : m_vecSlot)
	{
		if (!pInvenSlot)
			continue;
		if (pInvenSlot->Update())
			SlotUpdate(pInvenSlot);
	}

	//	마우스 클릭중이 아닐 때 리셋
	if (!KEYPRESS(VK_LBUTTON))
	{
		GET_SINGLE(CMouse)->ItemNullSet();
		if (m_pClickedSlot)
			m_pClickedSlot->IconOn();
		m_pClickedSlot = nullptr;
	}

	return 0;
}

void CInventory::LateUpdate()
{
	if (!m_bInvenOn)
		return;
}

void CInventory::Render(HDC hDC)
{
	if (!m_bInvenOn)
		return;
	m_pExitButton->Render(hDC);

	HDC hMemDC = FIND_BITMAP_DC("Inventory");

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tRect.right - m_tRect.left, m_tRect.bottom - m_tRect.top, hMemDC, 0, 0, m_tRect.right - m_tRect.left, m_tRect.bottom - m_tRect.top, RGB(255, 0, 255));

	EquipSlotRender(hDC);

	hMemDC = FIND_BITMAP_DC("accessoryZoneWideVer");

	SIZE tSize = GET_BITMAP_SIZE("accessoryZoneWideVer");

	GdiTransparentBlt(hDC, static_cast<int>((m_tRect.left + m_tRect.right) * 0.5f - tSize.cx / 2),
		static_cast<int>(m_tRect.top + 182), tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	for (auto pEquipSlot : m_vecEquipSlot)
	{
		if (!pEquipSlot)
			continue;
		pEquipSlot->Render(hDC);
	}

	for (auto pSlot		: m_vecSlot)
	{
		if (!pSlot)
			continue;
		pSlot->Render(hDC);
	}

	for (auto pEquipSlot : m_vecEquipSlot)
	{
		if (!pEquipSlot)
			continue;
		if (pEquipSlot->GetItem() && pEquipSlot->GetBoolIcon())
			pEquipSlot->GetItem()->IconRender(hDC, static_cast<float>(pEquipSlot->GetPoint().x), static_cast<float>(pEquipSlot->GetPoint().y));
	}

	for (auto pSlot : m_vecSlot)
	{
		if (!pSlot)
			continue;
		if (pSlot->GetItem() && pSlot->GetBoolIcon())
			pSlot->GetItem()->IconRender(hDC, static_cast<float>(pSlot->GetPoint().x), static_cast<float>(pSlot->GetPoint().y));
	}


	int iLog = static_cast<int>(log10f(static_cast<float>(m_iPlayerGold)));

	GET_SINGLE(CFontManager)->Render(hDC, static_cast<float>(m_tRect.right - 80 - iLog * 7),
		static_cast<float>(m_tRect.bottom - 62), FT_NORMAL, 2.f, to_string(m_iPlayerGold));

	for (auto& pEquipSlot : m_vecEquipSlot)
	{
		if (!pEquipSlot)
			continue;
		if (pEquipSlot->GetOnMouse() && pEquipSlot->GetItem()) 
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			pEquipSlot->GetItem()->DiscRender(hDC, static_cast<float>(pt.x), static_cast<float>(pt.y));
		}
	}

	for (auto& pSlot : m_vecSlot)
	{
		if (!pSlot)
			continue;
		if (pSlot->GetOnMouse() && pSlot->GetItem())
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			pSlot->GetItem()->DiscRender(hDC, static_cast<float>(pt.x), static_cast<float>(pt.y));
		}
	}
	//CFontNumber::RenderNumber(hDC, to_string(m_iPlayerGold), static_cast<float>(m_tRect.right - 65 - iLog * 7),
	//	static_cast<float>(m_tRect.bottom - 58), FONT_STYLE_NORMAL | FONT_STYLE_X2);
}

void CInventory::Release()
{
	SAFE_DELETE(m_pExitButton);

	for (auto& pEquipSlot : m_vecEquipSlot)
		SAFE_DELETE(pEquipSlot);
	

	for (auto& pInvenSlot : m_vecSlot)
		SAFE_DELETE(pInvenSlot);

	m_vecEquipSlot.clear();
	m_vecSlot.clear();
}

void CInventory::ToggleInven()
{
	m_bInvenOn = !m_bInvenOn;
	GET_SINGLE(CSoundMgr)->PlaySound(L"OpenInventory.wav", CSoundMgr::EFFECT);
	if (m_bInvenOn)
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(false);
	else
	{
		if (!GET_SINGLE(CPlayerUI)->IsThereOpenWindow())
			GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
	}
}

void CInventory::OpenInven()
{
	m_bInvenOn = true;
	GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(false);
}

void CInventory::CloseInven()
{
	m_bInvenOn = false;
	if (!GET_SINGLE(CPlayerUI)->IsThereOpenWindow())
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
}

void CInventory::EquipSlotUpdate(vector<CItem*>* pVecPlayerEquip)
{
	//	플레이어 장비 정보 받아오기
	for (size_t i = 0; i < ES_END; ++i)
	{
		if (!(*pVecPlayerEquip)[i])
			continue;
		m_vecEquipSlot[i]->SetItem((*pVecPlayerEquip)[i]);
	}
}

bool CInventory::AddItem(CItem* pItem)
{
	for (auto pSlot : m_vecSlot)
	{
		if (!pSlot->GetItem())
		{
			pSlot->SetItem(pItem);
			pSlot->IconOn();
			GET_SINGLE(CSoundMgr)->PlaySound(L"PickUpItem.wav", CSoundMgr::EFFECT);
			return true;
		}
	}
	return false;
}

CItem * CInventory::FindInventoryItem(const string & strName)
{
	for (auto& pSlot : m_vecEquipSlot)
	{
		if (!pSlot->GetItem())
			continue;

		if (pSlot->GetItem()->GetName() == strName)
			return pSlot->GetItem();
	}

	for (auto& pSlot : m_vecSlot)
	{
		if (!pSlot->GetItem())
			continue;

		if (pSlot->GetItem()->GetName() == strName)
			return pSlot->GetItem();
	}

	return nullptr;
}

CInvenSlot * CInventory::FindItemSlot(const string & strName)
{
	for (auto& pSlot : m_vecEquipSlot)
	{
		if (!pSlot->GetItem())
			continue;

		if (pSlot->GetItem()->GetName() == strName)
			return pSlot;
	}

	for (auto& pSlot : m_vecSlot)
	{
		if (!pSlot->GetItem())
			continue;

		if (pSlot->GetItem()->GetName() == strName)
			return pSlot;
	}

	return nullptr;
}

void CInventory::EquipSlotSetting()
{
	m_vecEquipSlot.assign(IST_INVEN, nullptr);

	int iY = m_tRect.top + 105;

	m_vecEquipSlot[0] = new CInvenSlot;
	m_vecEquipSlot[0]->SetSlotType(IST_WEAPON1);
	m_vecEquipSlot[0]->Initialize("EquipWeaponSlot", L"Image/Inventory/EquipWeaponSlot(x3).bmp", 
		"EquipWeaponSlotOn", L"Image/Inventory/EquipWeaponSlotOn(x3).bmp");
	SIZE tSize = GET_BITMAP_SIZE("EquipWeaponSlot");
	m_vecEquipSlot[0]->SetPoint(m_tRect.left + 49 + tSize.cx / 2, iY + tSize.cy / 2);
	m_vecEquipSlot[0]->IconOn();

	m_vecEquipSlot[1] = new CInvenSlot;
	m_vecEquipSlot[1]->SetSlotType(IST_SHIELD1);
	m_vecEquipSlot[1]->Initialize("EquipShieldSlot", L"Image/Inventory/EquipShieldSlot(x3).bmp", 
		"EquipShieldSlotOn", L"Image/Inventory/EquipShieldSlotOn(x3).bmp");
	tSize = GET_BITMAP_SIZE("EquipShieldSlot");
	m_vecEquipSlot[1]->SetPoint(m_tRect.left + 49 + tSize.cx / 2 + tSize.cx + 9, iY + tSize.cy / 2);
	m_vecEquipSlot[1]->IconOn();

	m_vecEquipSlot[2] = new CInvenSlot;
	m_vecEquipSlot[2]->SetSlotType(IST_WEAPON2);
	m_vecEquipSlot[2]->Initialize("EquipWeaponSlot", L"Image/Inventory/EquipWeaponSlot(x3).bmp", 
		"EquipWeaponSlotOn", L"Image/Inventory/EquipWeaponSlotOn(x3).bmp");
	tSize = GET_BITMAP_SIZE("EquipWeaponSlot");
	m_vecEquipSlot[2]->SetPoint(m_tRect.right - 159 + tSize.cx / 2, iY + tSize.cy / 2);
	m_vecEquipSlot[2]->IconOn();

	m_vecEquipSlot[3] = new CInvenSlot;
	m_vecEquipSlot[3]->SetSlotType(IST_SHIELD2);
	m_vecEquipSlot[3]->Initialize("EquipShieldSlot", L"Image/Inventory/EquipShieldSlot(x3).bmp", 
		"EquipShieldSlotOn", L"Image/Inventory/EquipShieldSlotOn(x3).bmp");
	tSize = GET_BITMAP_SIZE("EquipShieldSlot");
	m_vecEquipSlot[3]->SetPoint(m_tRect.right - 159 + tSize.cx / 2 + tSize.cx + 9, iY + tSize.cy / 2);
	m_vecEquipSlot[3]->IconOn();

	tSize = GET_BITMAP_SIZE("accessoryZoneWideVer");

	int iAccSlotStartX = static_cast<int>((m_tRect.left + m_tRect.right) * 0.5f - tSize.cx / 2);
	int iAccSlotStartY = m_tRect.top + 185;
	for (size_t i = IST_ACCESSORIES1; i < IST_INVEN; ++i)
	{
		if (!m_vecEquipSlot[i])
		{
			m_vecEquipSlot[i] = new CInvenSlot;
			m_vecEquipSlot[i]->SetSlotType(static_cast<INVEN_SLOT_TYPE>(i));
			m_vecEquipSlot[i]->Initialize("accessory", L"Image/Inventory/accessory.bmp", "accessorySelect", L"Image/Inventory/accessorySelect.bmp");
			tSize = GET_BITMAP_SIZE("InvenItemSlot");
			m_vecEquipSlot[i]->SetPoint(iAccSlotStartX + tSize.cx / 2 + 9 + (tSize.cx + 4) * (i - IST_ACCESSORIES1), iAccSlotStartY + tSize.cx / 2 + 6);
			m_vecEquipSlot[i]->IconOn();
		}
	}

}

void CInventory::EquipSlotRender(HDC hDC)
{
	HDC hMemDC1, hMemDC2;
	SIZE tSize1, tSize2;
	if (m_bEquipedWeapon)	//	EquipSlot1
	{
		hMemDC1 = FIND_BITMAP_DC("EquipSlot1On");
		hMemDC2 = FIND_BITMAP_DC("EquipSlot2");
		tSize1 = GET_BITMAP_SIZE("EquipSlot1On");
		tSize2 = GET_BITMAP_SIZE("EquipSlot2");
	}
	else
	{
		hMemDC1 = FIND_BITMAP_DC("EquipSlot1");
		hMemDC2 = FIND_BITMAP_DC("EquipSlot2On");
		tSize1 = GET_BITMAP_SIZE("EquipSlot1");
		tSize2 = GET_BITMAP_SIZE("EquipSlot2On");
	}
	GdiTransparentBlt(hDC, m_tRect.left + 40, m_tRect.top + 81, tSize1.cx, tSize1.cy, hMemDC1,
		0, 0, tSize1.cx, tSize1.cy, RGB(255, 0, 255));
	GdiTransparentBlt(hDC, m_tRect.right - 168, m_tRect.top + 81, tSize2.cx, tSize2.cy, hMemDC2,
		0, 0, tSize2.cx, tSize2.cy, RGB(255, 0, 255));

	int iX = (m_tRect.left + 40 + m_tRect.right - 168 + tSize1.cx)/2;


	hMemDC1 = FIND_BITMAP_DC("Keyboard_Tab");

	tSize1 = GET_BITMAP_SIZE("Keyboard_Tab");

	GdiTransparentBlt(hDC, iX - tSize1.cx * 3 / 4, m_tRect.top + 101,
		tSize1.cx * 3/2, tSize1.cy * 3/2, hMemDC1, 0, 0, tSize1.cx, tSize1.cy, RGB(255, 0, 255));
}

void CInventory::ChangeSlotItem(CInvenSlot *& pSlot)
{
	if (!m_pClickedSlot)	return;

	CItem* pClickedItem = nullptr;
	CItem* pSlotItem = nullptr;

	if (m_pClickedSlot)	pClickedItem = m_pClickedSlot->GetItem();
	if (pSlot)			pSlotItem = pSlot->GetItem();

	if (!pClickedItem)	return;

	switch (pSlot->GetSlotType())
	{
	case IST_WEAPON1:
	case IST_WEAPON2:
		if(pClickedItem->GetType() != IT_WEAPON)
			return;
		break;
	case IST_SHIELD1:
	case IST_SHIELD2:
		if (pClickedItem->GetType() != IT_SHIELD)
			return;
		break;
	case IST_ACCESSORIES1:
	case IST_ACCESSORIES2:
	case IST_ACCESSORIES3:
	case IST_ACCESSORIES4:
		if (pClickedItem->GetType() != IT_ACCESSORIES)
			return;
		break;
	default:
		break;
	}

	m_pClickedSlot->SetItem(pSlotItem);
	m_pClickedSlot->IconOn();
	pSlot->SetItem(pClickedItem);
	pSlot->IconOn();
	ApplyPlayerEquip();
}

void CInventory::SlotUpdate(CInvenSlot *& pSlot)
{
	if (KEYDOWN(VK_LBUTTON))
	{
		pSlot->IconOff();
		GET_SINGLE(CMouse)->SetItem(pSlot->GetItem());
		m_pClickedSlot = pSlot;
		GET_SINGLE(CSoundMgr)->PlaySound(L"PickUpItem.wav", CSoundMgr::EFFECT);
	}
	else if (KEYUP(VK_LBUTTON))
	{
		ChangeSlotItem(pSlot);
		GET_SINGLE(CSoundMgr)->PlaySound(L"PickUpItem.wav", CSoundMgr::EFFECT);
	}

}

void CInventory::ApplyPlayerEquip()
{
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	if (!pPlayer)
		return;

	for (size_t i = 0; i < ES_END; ++i)
		pPlayer->Equip(m_vecEquipSlot[i]->GetItem(), static_cast<EQUIP_SLOT>(i));
}
