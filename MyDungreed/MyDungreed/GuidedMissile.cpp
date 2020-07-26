#include "stdafx.h"
#include "GuidedMissile.h"
#include "Monster.h"

CGuidedMissile::CGuidedMissile():
	m_iTailFrame(0)
{
}


CGuidedMissile::~CGuidedMissile()
{
	Release();
}

void CGuidedMissile::Initialize()
{
	m_bLife = true;
	m_iFrameDir = 1;
	m_dwFrameMaxTime = 40;
	m_fRangeDist = 2000.f;
	m_iTailFrame = 1;
}

int CGuidedMissile::Update()
{
	ChangeAngle();

	CBullet::Update();
	
	if (m_iTailFrame == m_iFrame)
		CreateTail();

	return 0;
}

void CGuidedMissile::LateUpdate()
{
}

void CGuidedMissile::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

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

void CGuidedMissile::Release()
{
}

void CGuidedMissile::Dying(float fx, float fy)
{
	GET_SINGLE(CEffectManager)->AddEffect("StarBulletFX", L"Image/Weapon/Lala'sMagicWand/StarBullet/StarBulletFX.bmp", m_tInfo.fx, m_tInfo.fy, 36, 35, 7, 4.f);

}

void CGuidedMissile::CreateTail()
{
	switch (m_iFrame)
	{
	case 0:
		GET_SINGLE(CEffectManager)->AddEffectToPret("StarBulletFadeout0", L"Image/Weapon/Lala'sMagicWand/StarBullet/StarBulletFadeout0.bmp",
			m_tInfo.fx, m_tInfo.fy, m_tInfo.icx, m_tInfo.icy, 19, 15, 4.f);
			break;
	case 1:
		GET_SINGLE(CEffectManager)->AddEffectToPret("StarBulletFadeout1", L"Image/Weapon/Lala'sMagicWand/StarBullet/StarBulletFadeout1.bmp",
			m_tInfo.fx, m_tInfo.fy, m_tInfo.icx, m_tInfo.icy, 19, 15, 4.f);
		break;
	case 2:
		GET_SINGLE(CEffectManager)->AddEffectToPret("StarBulletFadeout2", L"Image/Weapon/Lala'sMagicWand/StarBullet/StarBulletFadeout2.bmp",
			m_tInfo.fx, m_tInfo.fy, m_tInfo.icx, m_tInfo.icy, 19, 15, 4.f);
		break;
	case 3:
		GET_SINGLE(CEffectManager)->AddEffectToPret("StarBulletFadeout3", L"Image/Weapon/Lala'sMagicWand/StarBullet/StarBulletFadeout3.bmp",
			m_tInfo.fx, m_tInfo.fy, m_tInfo.icx, m_tInfo.icy, 19, 15, 4.f);
		break;
	default:
		break;
	}
	m_iTailFrame = m_iFrame + 1;
	if (m_iTailFrame >= m_iMaxFrame)
		m_iTailFrame = 0;
}

void CGuidedMissile::ChangeAngle()
{
	CObj* pTarget = nullptr;

	list<CObj*>* pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER_BOSS);

	if (!pObjList->empty())
		pTarget = pObjList->front();

	if (!pTarget)
	{
		pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER);

		float fMinimumDist = 10000.f;

		for (auto pObj : *pObjList)
		{
			float fdx = pObj->GetInfo().fx - m_tInfo.fx;
			float fdy = pObj->GetInfo().fy - m_tInfo.fy;

			float fDist = sqrtf(fdx * fdx + fdy * fdy);

			if (fMinimumDist > fDist)
			{
				pTarget = pObj;
				fMinimumDist = fDist;
			}
		}
	}

	if (!pTarget)
		return;

	float fdx = pTarget->GetInfo().fx - m_tInfo.fx;
	float fdy = pTarget->GetInfo().fy - m_tInfo.fy;

	float fDist = sqrtf(fdx * fdx + fdy * fdy);

	float fAngle = fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);

	float fdAngle = fAngle - m_fAngle;
	
	fdAngle = fdAngle < -PI ? fdAngle + 2 * PI : fdAngle > PI ? fdAngle - 2 * PI : fdAngle;

	float fMaxAngle = 0.08f;
	fdAngle = fdAngle < -fMaxAngle ? -fMaxAngle : fdAngle > fMaxAngle ? fMaxAngle : fdAngle;

	m_fAngle += fdAngle;
}
