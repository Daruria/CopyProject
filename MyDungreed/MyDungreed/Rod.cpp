#include "stdafx.h"
#include "Rod.h"


CRod::CRod() :
	m_bIsTurning(false),
	m_fAngle(0.f),
	m_dwTurningTime(0)
{
}


CRod::~CRod()
{
}

bool CRod::Attack()
{
	DWORD dwCurTime = GetTickCount();

	//if (m_dwTurningTime + 500 <= dwCurTime)
	{
		m_bIsTurning = true;
		m_bAttack = true;
		m_dwTurningTime = dwCurTime;

		m_dwAttackCool = dwCurTime;

		m_tAttackBox.fx = m_tInfo.fx;
		m_tAttackBox.fy = m_tInfo.fy;
	}
	return false;
}

bool CRod::Attack(float fAttack)
{
	DWORD dwCurTime = GetTickCount();

	//if (m_dwTurningTime + 500 <= dwCurTime)
	{
		m_bIsTurning = true;
		m_bAttack = true;
		m_dwTurningTime = dwCurTime;

		m_dwAttackCool = dwCurTime;

		m_tAttackBox.fx = m_tInfo.fx;
		m_tAttackBox.fy = m_tInfo.fy;
	}
	return false;
}

void CRod::AddAttackEffect(float fx, float fy)
{
	float m_fSlashAngle = static_cast<float>((rand() % 360 - 180) * (180 / PI));
	GET_SINGLE(CEffectManager)->AddRotateEffect("Slash", L"Image/Effect/Slash/SlashFX", fx, fy, 168, 168, 4, m_fSlashAngle, 1.f);
}

void CRod::Initialize()
{
	CWeapon::Initialize();
	m_iPrice = 300;

	m_bRenderBack = false;

	m_fRatio = 4.f;
	m_fAngle = 0.f;
	m_dwTurningTime = 0;
	m_dwAttackCool = 0;
	m_dwAttackCoolMax = 500;

	m_bIsSkill		= false;	
	m_bCanUseSkill	= false;

	m_tAttackBox.icx = static_cast<int>(33 * m_fRatio);
	m_tAttackBox.icy = static_cast<int>(33 * m_fRatio);

	m_eWeaponType = WT_MELEE;
	SetType(IT_WEAPON);
	SetAttack(20);
	SetName("Quarter Staff");
	SetIconImage("QuarterStaffIcon", L"Image/Weapon/QuarterStaff/QuarterStaff.bmp");
	SetImage("QuarterStaff", L"Image/Weapon/QuarterStaff/QuarterStaff.bmp");
}

int CRod::Update()
{
	DWORD dwCurTime = GetTickCount();
	if (m_bIsTurning)
	{

		m_tAttackBox.fx = m_tInfo.fx;
		m_tAttackBox.fy = m_tInfo.fy;

		m_fAngle += float(PI * 2.f) * float(dwCurTime - m_dwPrevTime) / 250;

		if (m_fAngle >= float(2.f * PI))
			m_fAngle = 0.f;

		if (m_dwAttackCool + 200 <= dwCurTime)
		{
			m_bAttack = true;
			m_dwAttackCool = dwCurTime;
		}

		if (m_fAngle <= float(PI / 180)&& m_dwTurningTime + 500 <= dwCurTime)
		{
			m_bAttack = false;
			m_bIsTurning = false;
			m_fAngle = 0.f;
		}

		m_dwPrevTime = dwCurTime;
	}

	return 0;
}

void CRod::LateUpdate()
{
}

void CRod::Render(HDC hDC)
{
	HDC hMemDC = FIND_ROTATED_DC("QuarterStaff", m_fAngle);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC,
		0,
		0,
		m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
		
	CWeapon::Render(hDC);
}

void CRod::Release()
{
}

void CRod::DiscRender(HDC hDC, float fx, float fy)
{
	SIZE tWindowSize = { 300, 300 };

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

	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 50.f, rc.top + 10.f, FT_NORMAL, 3.f, m_strName);

	hMemDC = FIND_BITMAP_DC("IconWhite");
	tSize = GET_BITMAP_SIZE("IconWhite");

	bRender = GdiTransparentBlt(hDC, rc.left + 25, rc.top + 70,
		static_cast<int>(tSize.cx * 4.f),
		static_cast<int>(tSize.cy * 4.f),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	IconRender(hDC,
		rc.left + 25 + tSize.cx * 4.f * 0.5f,
		rc.top + 70 + tSize.cy * 4.f * 0.5f);

	string strText = "Damage " + to_string(m_iAttack);
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 120.f, rc.top + 70.f, FT_NORMAL, 2.f, strText);

	//strText = "Attacks per second : " + to_string(m_iAttack);
	//GET_SINGLE(CFontManager)->Render(hDC, fx, fy - 150.f, FT_NORMAL, 1.f, strText);

	strText = "Common";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 160.f, FT_NORMAL, 2.f, strText);

	strText = "Two Handed";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 200.f, FT_NORMAL, 2.f, strText);

	strText = "\"The Quarterstaff used by martial arts masters.\"";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 240.f, FT_NORMAL, 1.f, strText);
}
