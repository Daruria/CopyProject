#include "stdafx.h"
#include "MagicShield.h"
#include "Player.h"

CMagicShield::CMagicShield():
	m_fArmor(0.f)
{
	ZeroMemory(&m_tOffset, sizeof(m_tOffset));
}

CMagicShield::~CMagicShield()
{
	Release();
}

void CMagicShield::Initialize()
{
	SetName("MagicShield");
	SetIconImage("MagicShieldIcon", L"Image/Item/Shield/MagicShield.bmp");

	INSERT_BITMAP("MagicShield", L"Image/Item/Shield/MagicShield.bmp");

	m_tInfo.icx = 9;
	m_tInfo.icy = 9;
	m_fRatio = 4.f;

	m_fArmor = 8;

	m_eType = IT_SHIELD;
	m_eShieldType = CShield::SHIELD;

	m_tOffset = {-20.f, 0.f};
}

int CMagicShield::Update()
{
	m_bDir = GET_SINGLE(CObjectManager)->GetPlayer()->GetDir();
	
	return 0;
}

void CMagicShield::LateUpdate()
{
}

void CMagicShield::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("MagicShield");

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	int iDir = m_bDir ? 1 : -1;

	GdiTransparentBlt(hDC,
		static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f + m_tOffset.fx * iDir),
		static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f + m_tOffset.fy),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	

}

void CMagicShield::Release()
{
}

void CMagicShield::DiscRender(HDC hDC, float fx, float fy)
{
	SIZE tWindowSize = { 300, 350 };

	RECT rc = {
		static_cast<LONG>(fx),
		static_cast<LONG>(fy),
		static_cast<LONG>(fx + tWindowSize.cx),
		static_cast<LONG>(fy + tWindowSize.cy) };
	if (fx + tWindowSize.cx >= WINCX)
	{
		rc.left = WINCX - tWindowSize.cx;
		rc.right = WINCX;
	}

	if (fy + tWindowSize.cy >= WINCY)
	{
		rc.top = WINCY - tWindowSize.cy;
		rc.bottom = WINCY;
	}

	HDC hMemDC = FIND_BITMAP_DC("ItemDiscWind");
	SIZE tSize = GET_BITMAP_SIZE("ItemDiscWind");

	BOOL bRender = GdiTransparentBlt(hDC,
		rc.left,
		rc.top,
		tWindowSize.cx,
		tWindowSize.cy,
		hMemDC,
		0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	GET_SINGLE(CFontManager)->Render(hDC, (rc.left + rc.right) / 2, rc.top + 10.f, FT_NORMAL, 3.f, m_strName, FPX_CENTER);

	hMemDC = FIND_BITMAP_DC("IconWhite");
	tSize = GET_BITMAP_SIZE("IconWhite");

	bRender = GdiTransparentBlt(hDC, rc.left + 25, rc.top + 70,
		static_cast<int>(tSize.cx * 4.f),
		static_cast<int>(tSize.cy * 4.f),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	IconRender(hDC, rc.left + 25 + tSize.cx * 4.f * 0.5f, rc.top + 70 + tSize.cy * 4.f * 0.5f);

	string strText = "Armor " + to_string((int)m_fArmor);
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 120.f, rc.top + 70.f, FT_NORMAL, 2.f, strText);

	//strText = "Attacks per second : " + to_string(m_iAttack);
	//GET_SINGLE(CFontManager)->Render(hDC, fx, fy - 150.f, FT_NORMAL, 1.f, strText);

	strText = "Rare";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 160.f, FT_NORMAL, 2.f, strText);

	strText = "Off Hand";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 200.f, FT_NORMAL, 2.f, strText);

	strText = "\"A magical shield that";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 240.f, FT_NORMAL, 2.f, strText);

	strText= "appears from a bracelet";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 280.f, FT_NORMAL, 2.f, strText);

	strText = "on command\"";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 320.f, FT_NORMAL, 2.f, strText);
}
