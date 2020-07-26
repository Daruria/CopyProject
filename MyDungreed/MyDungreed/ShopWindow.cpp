#include "stdafx.h"
#include "ShopWindow.h"
#include "InvenSlot.h"
#include "ShopSlot.h"
#include "Item.h"
#include "Player.h"

CShopWindow::CShopWindow()
{
}


CShopWindow::~CShopWindow()
{
	Release();
}

void CShopWindow::Initialize()
{
	INSERT_BITMAP("DungeonShopBase", L"Image/Shop/DungeonShopBase.bmp");

	m_tSize.cx = 122;
	m_tSize.cy = 188;
	m_fRatio = 4.f;
}

void CShopWindow::Initialize(const string & strKey, const wchar_t * pPath)
{

}

int CShopWindow::Update()
{
	if (!m_bIsOn)
		return 0;

	int iItemPosStartX = static_cast<int>(m_tSize.cx * m_fRatio / 2 - 5);
	int	iItemPosStartY = static_cast<int>(m_tSize.cy * m_fRatio / 2 - 200);

	int iCount = 0;

	for (auto pItemSlot : m_ItemSlotList)
	{
		pItemSlot->SetPoint(static_cast<LONG>(iItemPosStartX), static_cast<LONG>(iItemPosStartY + iCount * 24 * 4.f));
		iCount++;
	}
	
	for (auto iter = m_ItemSlotList.begin(); iter != m_ItemSlotList.end();)
	{
		if ((*iter)->Update())
		{
			if (KEYUP(VK_RBUTTON))
			{
				GET_SINGLE(CInventory)->AddItem((*iter)->GetItem());
				GET_SINGLE(CInventory)->AddGold(-((*iter)->GetItem()->GetPrice()));
				SAFE_DELETE((*iter));
				iter = m_ItemSlotList.erase(iter);
			}
			else
				++iter;
		}
		else
			++iter;
	}

	if (KEYDOWN(VK_ESCAPE))
	{
		WindowOff();
		GET_SINGLE(CObjectManager)->GetPlayer()->SetCanAction(true);
	}

	return 0;
}

void CShopWindow::LateUpdate()
{

}

void CShopWindow::Render(HDC hDC)
{
	if (!m_bIsOn)
		return;

	HDC hMemDC = FIND_BITMAP_DC("DungeonShopBase");

	BOOL bRender = GdiTransparentBlt(hDC, 0, 0,
		static_cast<int>(m_tSize.cx * m_fRatio),
		static_cast<int>(m_tSize.cy * m_fRatio),
		hMemDC,
		0, 0, m_tSize.cx, m_tSize.cy, RGB(255, 0, 255));

	for (auto& pSlot : m_ItemSlotList)
		pSlot->Render(hDC);

	for (auto& pSlot : m_ItemSlotList)
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
}

void CShopWindow::Release()
{
	for (auto& pSlot : m_ItemSlotList)
	{
		delete pSlot->GetItem();
		SAFE_DELETE(pSlot);
	}
	m_ItemSlotList.clear();
}

void CShopWindow::AddItem(CItem * pItem)
{
	if (!pItem || m_ItemSlotList.size() >= 5)
		return;

	int iItemPosStartX = static_cast<int>(m_tSize.cx * m_fRatio / 2 - 5);
	int	iItemPosStartY = static_cast<int>(m_tSize.cy * m_fRatio / 2 - 200);

	int iItemListCount = m_ItemSlotList.size();

	CShopSlot* pSlot = new CShopSlot;
	pSlot->Initialize("ShopItem", L"Image/Shop/ShopItem.bmp", "ShopItem_Selected", L"Image/Shop/ShopItem_Selected.bmp");
	pSlot->SetRatio(4.f);
	pSlot->SetItem(pItem);
	pSlot->SetPoint(static_cast<LONG>(iItemPosStartX), static_cast<LONG>(iItemPosStartY + iItemListCount * 24 * 4.f));
	pSlot->SetSize(static_cast<LONG>(107), static_cast<LONG>(23));

	m_ItemSlotList.push_back(pSlot);
}

void CShopWindow::WindowOn()
{
	m_bIsOn = true;
	GET_SINGLE(CInventory)->OpenInven();
}

void CShopWindow::WindowOff()
{
	m_bIsOn = false;
	GET_SINGLE(CInventory)->CloseInven();
}
