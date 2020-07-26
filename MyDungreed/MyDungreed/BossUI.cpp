#include "stdafx.h"
#include "BossUI.h"
#include "Monster.h"

CBossUI::CBossUI():
	m_bLife(true),
	m_iHp(0),
	m_iMaxHp(0),
	m_fRatio(0.f)
{
}


CBossUI::~CBossUI()
{
	Release();
}

void CBossUI::Initialize(const string & strKey, const wchar_t * pPath)
{

}

int CBossUI::Update()
{
	CMonster* pMonster = dynamic_cast<CMonster*>((*GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER_BOSS)).front());
	if (!pMonster || m_iHp <= 0)
	{
		m_bLife = false;
		return 1;
	}

	m_iHp = pMonster->GetCharInfo().iHP;
	m_iMaxHp = pMonster->GetCharInfo().iHPMax;

	return 0;
}

void CBossUI::LateUpdate()
{
}

void CBossUI::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("BossLifeBack");

	SIZE tSize = GET_BITMAP_SIZE("BossLifeBack");

	int iLifeBarOffset = 50;

	GdiTransparentBlt(hDC, 
		int(WINCX / 2 - tSize.cx * m_fRatio / 2), 
		int(WINCY - iLifeBarOffset - tSize.cy * m_fRatio / 2),
		int(tSize.cx * m_fRatio), 
		int(tSize.cy * m_fRatio), 
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	hMemDC = FIND_BITMAP_DC("BossLifeBar");
	tSize = GET_BITMAP_SIZE("BossLifeBar");
	float fHPRatio = float(m_iHp) / float(m_iMaxHp);
	int iTotalLifeBarLength = 400;
	GdiTransparentBlt(hDC,
		WINCX / 2 - 162,
		int(WINCY - iLifeBarOffset - tSize.cy * m_fRatio / 2),
		int(iTotalLifeBarLength * fHPRatio),
		int(tSize.cy * m_fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	hMemDC = FIND_BITMAP_DC("BossSkellPortrait");
	tSize = GET_BITMAP_SIZE("BossSkellPortrait");
	GdiTransparentBlt(hDC,
		int(WINCX / 2 - tSize.cx * m_fRatio / 2) - 204,
		int(WINCY - iLifeBarOffset - tSize.cy * m_fRatio / 2),
		int(tSize.cx * m_fRatio),
		int(tSize.cy * m_fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	hMemDC = FIND_BITMAP_DC("BossLifeBase");
	tSize = GET_BITMAP_SIZE("BossLifeBase");
	GdiTransparentBlt(hDC, 
		int(WINCX / 2 - tSize.cx * m_fRatio / 2), 
		int(WINCY - iLifeBarOffset - tSize.cy * m_fRatio / 2),
		int(tSize.cx * m_fRatio), 
		int(tSize.cy * m_fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));


}

void CBossUI::Release()
{

}

void CBossUI::Initialize()
{
	INSERT_BITMAP("BossLifeBack", L"Image/Monster/LifeBar/BossLifeBack.bmp");
	INSERT_BITMAP("BossLifeBase", L"Image/Monster/LifeBar/BossLifeBase.bmp");
	INSERT_BITMAP("BossSkellPortrait", L"Image/Monster/LifeBar/BossSkellPortrait.bmp");
	INSERT_BITMAP("BossLifeBar", L"Image/Monster/LifeBar/LifeBar.bmp");


	m_bLife = true;

	m_iHp = 100;
	m_iMaxHp = 100;
	m_fRatio = 4.f;
}

