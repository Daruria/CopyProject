#include "stdafx.h"
#include "NormalEffect.h"
#include "ScrollManager.h"


CNormalEffect::CNormalEffect():
	m_fAngle(0.f),
	m_fSpeed(0.f),
	m_iFrame(0),
	m_iMaxFrame(0),
	m_dwFrameTime(0),
	m_dwFrameMaxTime(0),
	m_strKey("")
{
}


CNormalEffect::~CNormalEffect()
{
	Release();
}

void CNormalEffect::Initialize()
{
	m_dwFrameTime = GetTickCount();
	m_dwFrameMaxTime = 100;
	m_iFrame = 0;
	m_bLife = true;
}

int CNormalEffect::Update()
{
	if (m_iMaxFrame == 1)
	{
		if (m_dwFrameTime + m_dwFrameMaxTime < GetTickCount())
			m_bLife = false;
		return 0;
	}

	if (m_dwFrameTime + m_dwFrameMaxTime < GetTickCount())
	{
		m_iFrame++;
		if (m_iFrame >= m_iMaxFrame)	m_bLife = false;
		m_dwFrameTime = GetTickCount();
	}

	m_tInfo.fx += cosf(m_fAngle) * m_fSpeed;
	m_tInfo.fy += sinf(m_fAngle) * m_fSpeed;

	return 0;
}

void CNormalEffect::LateUpdate()
{
}

void CNormalEffect::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	if (tPos.fx + m_tInfo.icx / 2 < 0 || tPos.fx - m_tInfo.icx / 2 > WINCX)
		return;
	if (tPos.fy + m_tInfo.icy / 2 < 0 || tPos.fy - m_tInfo.icy / 2 > WINCY)
		return;

	SIZE tSize = GET_BITMAP_SIZE(m_strKey);

	BOOL bRender = TRUE;

	if (hMemDC)
		bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio / 2), 
			static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio / 2),
			static_cast<int>(m_tInfo.icx * m_fRatio),
			static_cast<int>(m_tInfo.icy * m_fRatio),
			hMemDC, 
			m_tInfo.icx * m_iFrame, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, static_cast<int>(tPos.fx - m_tInfo.icx / 2), static_cast<int>(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);
	int iA = 0;
}

void CNormalEffect::Release()
{
}
