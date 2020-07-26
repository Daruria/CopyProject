#include "stdafx.h"
#include "Gun.h"
#include "Bullet.h"

CGun::CGun()
{
}


CGun::~CGun()
{
	Release();
}

bool CGun::Attack()
{
	//if (m_dwAttackCool + m_dwAttackCoolMax <= GetTickCount())
	{
		if (m_iRemainBullet <= 0)
		{
			if (!m_bReload)
			{
				m_bReload = true;
				m_dwReloadingTime = GetTickCount();
			}
		}
		else
			Shoot();
	}
	
	return false;
}

bool CGun::Attack(float fAttack)
{
	//if (m_dwAttackCool + m_dwAttackCoolMax <= GetTickCount())
	{
		if (m_iRemainBullet <= 0)
		{
			if (!m_bReload)
			{
				m_bReload = true;
				m_dwReloadingTime = GetTickCount();
			}
		}
		else
			Shoot();
	}

	return false;
}

void CGun::AddAttackEffect(float fx, float fy)
{
}

void CGun::Initialize()
{
	CWeapon::Initialize();
	m_iPrice = 200;

	INSERT_BITMAP("ReloadBase", L"Image/Weapon/Gun/Reload/ReloadBase.bmp");
	INSERT_BITMAP("ReloadBar", L"Image/Weapon/Gun/Reload/ReloadBar.bmp");

	m_bRenderBack = false;

	m_fAngle = 0.f;
	m_fRatio = 1.f;
	
	m_tInfo.icx = 44;
	m_tInfo.icy = 44;

	SIZE tSize = GET_BITMAP_SIZE("ReloadBase");
	m_tReloadInfo = {0.f, -65.f, tSize.cx, tSize.cy};
	m_dwReloadTime = 1000;
	m_iRemainBullet = 10;
	m_iMagazine = 10;

	m_eWeaponType = WT_RANGED;
	SetType(IT_WEAPON);

	m_dwAttackCoolMax = 200;

	m_tOffset = {50.f, 0.f, static_cast<int>(m_fRatio * m_tInfo.icx), static_cast<int>(m_fRatio * m_tInfo.icy)};

	//SetAttack(20);
	//SetName("Short Sword");
	//SetIconImage("ShortSwordIcon", L"Image/Weapon/BasicShortSwordIcon.bmp");
	//SetImage("ShortSword", L"Image/Weapon/BasicShortSword.bmp");
}

int CGun::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	float fdx = pt.x - tPos.fx;
	float fdy = pt.y - tPos.fy;
	float fDist = sqrtf(fdx * fdx + fdy * fdy);
	m_fAngle = fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);

	m_bDir = fdx >= 0;
	if (!m_bDir)
		m_fLeftAngle = m_fAngle + (float)PI;

	if (m_bReload)
	{
		if (m_dwReloadingTime + m_dwReloadTime <= GetTickCount())
		{
			m_bReload = false;
			m_iRemainBullet = m_iMagazine;

			GET_SINGLE(CEffectManager)->AddEffect("ReloadFX", L"Image/Weapon/Gun/Reload/ReloadFXSprite.bmp",
				m_tInfo.fx + m_tReloadInfo.fx, m_tInfo.fy + m_tReloadInfo.fy, 27, 9, 4, 4.f);
		}
		else
			Reload();
	}

	return 0;
}

void CGun::LateUpdate()
{
}

void CGun::Render(HDC hDC)
{
	HDC hMemDC = m_bDir ? FIND_ROTATED_DC(m_strKey, m_fAngle) : FIND_ROTATED_DC(m_strLeftKey, m_fLeftAngle);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		static_cast<int>(tPos.fx + m_tOffset.fx * cosf(m_fAngle) - m_tOffset.icx * 0.5f),
		static_cast<int>(tPos.fy + m_tOffset.fx * sinf(m_fAngle) - m_tOffset.icy * 0.5f),
		m_tOffset.icx,
		m_tOffset.icy,
		hMemDC,
		0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	if (m_bReload)
	{
		hMemDC = FIND_BITMAP_DC("ReloadBase");

		GdiTransparentBlt(hDC,
			static_cast<int>(tPos.fx + m_tReloadInfo.fx - m_tReloadInfo.icx * 2.f),
			static_cast<int>(tPos.fy + m_tReloadInfo.fy - m_tReloadInfo.icy * 2.f),
			static_cast<int>(m_tReloadInfo.icx * 4.f),
			static_cast<int>(m_tReloadInfo.icy * 4.f),
			hMemDC,
			0, 0, m_tReloadInfo.icx, m_tReloadInfo.icy, RGB(255, 0, 255));

		hMemDC = FIND_BITMAP_DC("ReloadBar");

		SIZE tSize = GET_BITMAP_SIZE("ReloadBar");

		GdiTransparentBlt(hDC,
			static_cast<int>(tPos.fx + m_tReloadInfo.fx - m_tReloadInfo.icx * 2.f + m_tReloadInfo.icx * 4.f * m_fReloadRatio),
			static_cast<int>(tPos.fy + m_tReloadInfo.fy - m_tReloadInfo.icy * 2.f - tSize.cy * 2.f),
			static_cast<int>(tSize.cx * 4.f),
			static_cast<int>(tSize.cy * 4.f),
			hMemDC,
			0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	}
}

void CGun::Release()
{
}

void CGun::DiscRender(HDC hDC, float fx, float fy)
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

	strText = "Main Handed";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 200.f, FT_NORMAL, 2.f, strText);

	strText = "\"Revoler\"";
	GET_SINGLE(CFontManager)->Render(hDC, rc.left + 20.f, rc.top + 240.f, FT_NORMAL, 2.f, strText);
}


void CGun::Shoot()
{
	CRangedWeapon::Shoot();
	
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GET_SINGLE(CEffectManager)->AddRotateEffect("BulletFire", L"Image/Weapon/Gun/Bullet/Fire/x4/BulletFX002", 
		m_tInfo.fx + (m_tOffset.fx + 44)* cosf(m_fAngle), 
		m_tInfo.fy + (m_tOffset.fx + 44)* sinf(m_fAngle), 60, 60, 6, m_fAngle, 1.f);

	CBullet* pBullet = new CBullet;
	pBullet->Initialize();
	pBullet->SetAngle(m_fAngle);
	pBullet->SetMaxFrame(1);
	pBullet->SetRatio(1.0f);
	pBullet->SetSpeed(10.f);
	pBullet->SetAttack(float(m_iAttack));
	pBullet->SetImage("GunBullet2", L"Image/Weapon/Gun/Bullet/Bullet02(x4).bmp");
	pBullet->SetBulletType(BT_GUN);
	pBullet->SetInfo(m_tInfo.fx + (m_tOffset.fx + 44) * cosf(m_fAngle), m_tInfo.fy + (m_tOffset.fx + 44) * sinf(m_fAngle), 36, 36);
	pBullet->SetStartPos();
	pBullet->SetRangeDist(600.f);
	GET_SINGLE(CObjectManager)->AddObj(OT_PLAYER_BULLET, pBullet);

	GET_SINGLE(CSoundMgr)->PlaySound(L"AKFire.wav",CSoundMgr::EFFECT);

}

void CGun::Shoot(float fAttack)
{
	CRangedWeapon::Shoot();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GET_SINGLE(CEffectManager)->AddRotateEffect("BulletFire", L"Image/Weapon/Gun/Bullet/Fire/x4/BulletFX002",
		m_tInfo.fx + (m_tOffset.fx + 44)* cosf(m_fAngle),
		m_tInfo.fy + (m_tOffset.fx + 44)* sinf(m_fAngle), 60, 60, 6, m_fAngle, 1.f);

	CBullet* pBullet = new CBullet;
	pBullet->Initialize();
	pBullet->SetAngle(m_fAngle);
	pBullet->SetMaxFrame(1);
	pBullet->SetRatio(1.0f);
	pBullet->SetSpeed(10.f);
	pBullet->SetAttack(fAttack);
	pBullet->SetImage("GunBullet2", L"Image/Weapon/Gun/Bullet/Bullet02(x4).bmp");
	pBullet->SetBulletType(BT_GUN);
	pBullet->SetInfo(m_tInfo.fx + (m_tOffset.fx + 44) * cosf(m_fAngle), m_tInfo.fy + (m_tOffset.fx + 44) * sinf(m_fAngle), 36, 36);
	pBullet->SetStartPos();
	pBullet->SetRangeDist(600.f);
	GET_SINGLE(CObjectManager)->AddObj(OT_PLAYER_BULLET, pBullet);

	GET_SINGLE(CSoundMgr)->PlaySound(L"AKFire.wav", CSoundMgr::EFFECT);
}
