#include "stdafx.h"
#include "UISlot.h"
#include "Item.h"
#include "Weapon.h"
#include "RangedWeapon.h"

CUISlot::CUISlot()
{
}


CUISlot::~CUISlot()
{
	Release();
}

void CUISlot::Initialize(const string & strKey, const wchar_t * pPath, const string & strOnKey, const wchar_t * pOnPath)
{
	CMyButton::Initialize(strKey, pPath, strOnKey, pOnPath);
	m_bIconRender = true;
	m_pItem = nullptr;

	INSERT_BITMAP("EquippedWeaponReloadBase(x3)", L"Image/UI/EquipWeapon/EquippedWeaponReloadBase(x3).bmp");
}

int CUISlot::Update()
{
	m_bIsOnMouse = IsOnMouse() ? true : false;
	return m_bIsOnMouse;
}

void CUISlot::LateUpdate()
{
	CMyButton::LateUpdate();
}

void CUISlot::Render(HDC hDC)
{
	CMyButton::Render(hDC);
	if (m_pItem && m_bIconRender && !GET_SINGLE(CResult)->GetShowResult())
	{
		CWeapon* pWeapon = dynamic_cast<CWeapon*>(m_pItem);
		if (pWeapon->GetWeaponType() == WT_RANGED)
		{
			CRangedWeapon* pRangedWeapon = dynamic_cast<CRangedWeapon*>(pWeapon);
			if (pRangedWeapon->GetIsReloading())
			{
				float fRatio = pRangedWeapon->GetReloadRatio();
				HDC hMemDC = FIND_BITMAP_DC("EquippedWeaponReloadBase(x3)");

				SIZE tSize = GET_BITMAP_SIZE("EquippedWeaponReloadBase(x3)");

				GdiTransparentBlt(hDC, 
					static_cast<int>(m_tPoint.x - tSize.cx * 0.5f), 
					static_cast<int>(m_tPoint.y - tSize.cy * 0.5f),
					static_cast<int>(tSize.cx * fRatio), tSize.cy, hMemDC, 0, 0, static_cast<int>(tSize.cx * fRatio), tSize.cy, RGB(255, 0, 255));
			}
		}
		m_pItem->IconRender(hDC, static_cast<float>(m_tPoint.x), static_cast<float>(m_tPoint.y));
		if (pWeapon->GetWeaponType() == WT_RANGED)
		{
			CRangedWeapon* pRangedWeapon = dynamic_cast<CRangedWeapon*>(pWeapon);
			string strText = to_string(pRangedWeapon->GetRemainBullet());
			strText += " / ";
			strText += to_string(pRangedWeapon->GetMagazine());
			GET_SINGLE(CFontManager)->Render(hDC,
				static_cast<float>(m_tPoint.x - m_tSize.cx * 0.3f),
				static_cast<float>(m_tPoint.y + m_tSize.cy * 0.f),
				FT_NORMAL, 2.f, strText);
		}
	}
}

void CUISlot::Release()
{
}
