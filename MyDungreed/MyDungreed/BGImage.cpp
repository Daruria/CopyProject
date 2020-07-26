#include "stdafx.h"
#include "BGImage.h"
#include "ScrollManager.h"

CBGImage::CBGImage()
	:m_fRatio(1.f),
	m_iFrame(0),
	m_iMaxFrame(1),
	m_dwFrameTime(0),
	m_dwFrameMaxTime(100),
	m_strKey("")
{
}


CBGImage::~CBGImage()
{
	Release();
}

void CBGImage::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(m_strKey, pPath);
}

void CBGImage::Initialize()
{
}

int CBGImage::Update()
{
	if (m_dwFrameTime + m_dwFrameMaxTime <= GetTickCount())
	{
		m_iFrame++;
		if (m_iFrame >= m_iMaxFrame)
			m_iFrame = 0;
		m_dwFrameTime = GetTickCount();
	}
	return 0;
}

void CBGImage::LateUpdate()
{
}

void CBGImage::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	SIZE tSize = GET_BITMAP_SIZE(m_strKey);

	if (hMemDC)
	{
		BOOL b = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio / 2),
			static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio / 2),
			static_cast<int>(m_tInfo.icx * m_fRatio),
			static_cast<int>(m_tInfo.icy * m_fRatio),
			hMemDC, 
			m_iFrame * m_tInfo.icx, 
			0, 
			m_tInfo.icx, 
			m_tInfo.icy, RGB(255, 0, 255));
		int a = 0;
	}
	else
		CAN_NOT_FOUND_BITMAP(hDC, static_cast<int>(tPos.fx - m_tInfo.icx / 2), 
			static_cast<int>(tPos.fy - m_tInfo.icy / 2),
			m_tInfo.icx, m_tInfo.icy);
}

void CBGImage::Release()
{
}

void CBGImage::RenderWithoutScroll(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	GdiTransparentBlt(hDC,
		static_cast<int>(m_tInfo.fx - m_tInfo.icx * m_fRatio * 0.5f),
		static_cast<int>(m_tInfo.fy - m_tInfo.icy * m_fRatio * 0.5f),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
}
