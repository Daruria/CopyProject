#include "stdafx.h"
#include "ScrollManager.h"

float CScrollManager::m_fScrollX = 0.f;
float CScrollManager::m_fScrollY = 0.f;
int	CScrollManager::m_iScrollSizeX = 0;
int	CScrollManager::m_iScrollSizeY = 0;
int	CScrollManager::m_iScrollStartX = 0;
int	CScrollManager::m_iScrollStartY = 0;

CScrollManager::CScrollManager()
{
}


CScrollManager::~CScrollManager()
{
}

const POSITION CScrollManager::WorldToScreen(const INFO & tInfo)
{
	return POSITION{ tInfo.fx - m_fScrollX + WINCX / 2, tInfo.fy - m_fScrollY + WINCY / 2 };
}

const POSITION CScrollManager::ScreenToWorld(const INFO & tInfo)
{
	return ScreenToWorld(tInfo.fx, tInfo.fy);
}

const POSITION CScrollManager::ScreenToWorld(const POSITION & tPos)
{
	return ScreenToWorld(tPos.fx, tPos.fy);
}

const POSITION CScrollManager::ScreenToWorld(const POINT & tPoint)
{
	return ScreenToWorld(tPoint.x, tPoint.y);
}

const POSITION CScrollManager::ScreenToWorld(const float fx, const float fy)
{
	return POSITION{ m_fScrollX - WINCX / 2 + fx, m_fScrollY - WINCY / 2 + fy};
}

const POSITION CScrollManager::ScreenToWorld(const int ix, const int iy)
{
	return POSITION{ m_fScrollX - WINCX / 2 + ix, m_fScrollY - WINCY / 2 + iy };
}

void CScrollManager::ScrollLock()
{
	if (GetScreenLeft() < m_iScrollStartX)	
		m_fScrollX = static_cast<float>(WINCX / 2 + m_iScrollStartX);
	if (GetScreenTop() < m_iScrollStartY)	
		m_fScrollY = static_cast<float>(WINCY / 2 + m_iScrollStartY);

	if (m_fScrollX + WINCX / 2 > m_iScrollSizeX) m_fScrollX = static_cast<float>(m_iScrollSizeX - WINCX / 2);
	if (m_fScrollY + WINCY / 2 > m_iScrollSizeY) m_fScrollY = static_cast<float>(m_iScrollSizeY - WINCY / 2);
}
