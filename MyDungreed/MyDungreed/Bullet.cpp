#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet() :
	m_fAngle(0.f),
	m_fAttack(0.f),
	m_fRangeDist(0.f),
	m_tStartPos(POSITION{}),
	m_eType(BT_ARROW),
	m_strName("")
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_bLife = true;
	m_iFrameDir = 1;
	m_dwFrameMaxTime = 100;
	m_fRangeDist = 2000.f;
}

int CBullet::Update()
{
	m_tInfo.fx += cosf(m_fAngle) * m_fSpeed;
	m_tInfo.fy += sinf(m_fAngle) * m_fSpeed;

	float fdx = m_tInfo.fx - m_tStartPos.fx;
	float fdy = m_tInfo.fy - m_tStartPos.fy;

	float fDist = sqrtf(fdx * fdx + fdy * fdy);

	if (fDist >= m_fRangeDist)
	{
		m_bLife = false;
		Dying(m_tStartPos.fx, m_tStartPos.fy);
	}

	CMoveObj::Update();

	return 0;
}

void CBullet::LateUpdate()
{
}

void CBullet::Render(HDC hDC)
{
	HDC hMemDC = 0;
	
	switch (m_eType)
	{
	case BT_ARROW:
	case BT_GUN:
		hMemDC = FIND_ROTATED_DC(m_strKey, m_fAngle);
		break;
	case BT_BALL:
	case BT_NOTE:
	case BT_BOSSBALL:
		hMemDC = FIND_BITMAP_DC(m_strKey);
		break;
	case BT_END:
		break;
	default:
		break;
	}

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	BOOL bRender = GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	int iA = 0;
}

void CBullet::Release()
{
}

void CBullet::Dying(float fx, float fy)	//	fx, fy : ºÎµúÈù ¿ÀºêÁ§Æ® À§Ä¡
{
	switch (m_eType)
	{
	case BT_ARROW:
	{
		float fdx = fx - m_tInfo.fx;
		float fdy = fy - m_tInfo.fy;

		CMath::NormalizeXY(fdx, fdy);

		GET_SINGLE(CEffectManager)->AddRotateEffect("ArrowFX", L"Image/Monster/Skel/Arrow/Effect/Rect/ArrowFX", 
			m_tInfo.fx + fdx * (m_tInfo.icx * 0.5f + 50), m_tInfo.fy + fdy * (m_tInfo.icy * 0.5f + 50), 75, 75, 6, m_fAngle, 3.f);
	}
		break;
	case BT_BALL:
		GET_SINGLE(CEffectManager)->AddEffect("RangeBallBulletHit", L"Image/Monster/GiantBat/RangeBallBullet/Hit/RangeBallBulletHit.bmp", m_tInfo.fx, m_tInfo.fy, 32, 32, 7, 4.f);
		break;
	case BT_NOTE:
		GET_SINGLE(CEffectManager)->AddEffect("BansheeBulletBoomSprite", L"Image/Monster/Banshee/Bullet/Boom/BansheeBulletBoomSprite.bmp", m_tInfo.fx, m_tInfo.fy, 20, 27, 6, 4.f);
		break;
	case BT_BOSSBALL:
		GET_SINGLE(CEffectManager)->AddEffect("BossBulletFX", L"Image/Monster/BossSkell/Bullet/FX/BossBulletFX.bmp", m_tInfo.fx, m_tInfo.fy, 32 ,32, 5, 4.f);
		break;
	case BT_GUN:
	{
		float fdx = fx - m_tInfo.fx;
		float fdy = fy - m_tInfo.fy;

		CMath::NormalizeXY(fdx, fdy);

		GET_SINGLE(CEffectManager)->AddRotateEffect("GunBulletHitFX", L"Image/Weapon/Gun/Bullet/Hit/x4/BulletFX001",
			m_tInfo.fx, m_tInfo.fy, 60, 60, 6, m_fAngle, 1.f);
	}
		break;
	case BT_END:
		break;
	default:
		break;
	}
	
}

void CBullet::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(strKey, pPath);
}
