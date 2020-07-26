#include "stdafx.h"
#include "TresureBox.h"
#include "FieldItem.h"
#include "Item.h"

CTresureBox::CTresureBox() :
	m_bCanOpen(false),
	m_bOpenClose(false),
	m_bTakeOutItem(false),
	m_pItem(nullptr)
{
}


CTresureBox::~CTresureBox()
{
	Release();
}

void CTresureBox::Initialize()
{
	m_fRatio = 4.f;
	m_bTakeOutItem = false;
	m_bOpenClose = false;
	m_bTakeOutItem = false;
}

int CTresureBox::Update()
{
	if (m_bCanOpen && !m_bOpenClose && KEYDOWN('F'))
		m_bOpenClose = true;

	if (m_bOpenClose)
	{
		if (!m_bTakeOutItem)
		{
			CFieldItem* pFieldItem = new CFieldItem;
			pFieldItem->Initialize();

			pFieldItem->SetInfo(m_tInfo.fx, m_tInfo.fy, m_pItem->GetInfo().icx, m_pItem->GetInfo().icy);
			pFieldItem->SetKey(m_pItem->GetIconKey());
			pFieldItem->SetHitBox(0.f, 0.f, m_tInfo.icx, m_tInfo.icy);
			pFieldItem->SetMaxFrame(1);
			pFieldItem->SetItem(m_pItem);
			pFieldItem->SetFrame(0);
			pFieldItem->SetRatio(m_pItem->GetRatio());
			GET_SINGLE(CObjectManager)->AddObj(OT_FIELD_ITEM, pFieldItem);
			m_bTakeOutItem = true;
		}
	}

	return 0;
}

void CTresureBox::LateUpdate()
{
}

void CTresureBox::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_bOpenClose ? m_strOpenKey : m_strKey);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		0,
		0,
		m_tInfo.icx,
		m_tInfo.icy,
		RGB(255, 0, 255));
}

void CTresureBox::Release()
{
	if (!m_bOpenClose)
		SAFE_DELETE(m_pItem);
}

void CTresureBox::SetCloseImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(strKey, pPath);
}

void CTresureBox::SetOpenImage(const string & strKey, const wchar_t * pPath)
{
	m_strOpenKey = strKey;
	INSERT_BITMAP(strKey, pPath);
}

void CTresureBox::SetBox(int icx, int icy, float fRatio)
{
	m_tInfo.icx = icx;
	m_tInfo.icy = icy;

	m_fRatio = fRatio;

	m_tHitBox = { 0.f, 0.f, int(icx * fRatio), int(icy * fRatio) };
}

