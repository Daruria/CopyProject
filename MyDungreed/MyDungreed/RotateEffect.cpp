#include "stdafx.h"
#include "RotateEffect.h"
#include "ScrollManager.h"

CRotateEffect::CRotateEffect()	:
	m_iFrame(0),
	m_iMaxFrame(0),
	m_dwFrameTime(0),
	m_fAngle(0.f),
	m_strKey(""),
	m_vecFrameKey()
{
}


CRotateEffect::~CRotateEffect()
{
	Release();
}

void CRotateEffect::Initialize()
{
	m_dwFrameTime = GetTickCount();
	m_iFrame = 0;
	m_bLife = true;
}

int CRotateEffect::Update()
{
	if (m_dwFrameTime + 100 < GetTickCount())
	{
		m_iFrame++;
		if (m_iFrame >= m_iMaxFrame)	m_bLife = false;
		m_dwFrameTime = GetTickCount();
	}
	return 0;
}

void CRotateEffect::LateUpdate()
{
}

void CRotateEffect::Render(HDC hDC)
{
	HDC hMemDC = FIND_ROTATED_DC(m_vecFrameKey[m_iFrame], m_fAngle);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	if (tPos.fx + m_tInfo.icx / 2 < 0 || tPos.fx - m_tInfo.icx / 2 > WINCX)
		return;
	if (tPos.fy + m_tInfo.icy / 2 < 0 || tPos.fy - m_tInfo.icy / 2 > WINCY)
		return;

	BOOL bRender;

	if (hMemDC)
	{
		bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio / 2), 
			static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio / 2),
			static_cast<int>(m_tInfo.icx * m_fRatio), 
			static_cast<int>(m_tInfo.icy * m_fRatio),
			hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
		int iA = 0;
	}
	else
		CAN_NOT_FOUND_BITMAP(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx / 2), 
			static_cast<int>(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);

//#ifdef _DEBUG
//	CMath::DrawBox(hDC, tPos.fx - m_tInfo.icx / 2, tPos.fy - m_tInfo.icy / 2, m_tInfo.icx, m_tInfo.icy);
//#endif // _DEBUG

}

void CRotateEffect::Release()
{
}
