#include "stdafx.h"
#include "AnimatedBGObject.h"

CAnimatedBGObject::CAnimatedBGObject():
	m_fRatio(1.f),
	m_iFrame(0),
	m_iStartFrame(0),
	m_iMaxFrame(0),
	m_iTotalFrame(0),
	m_dwFrameTime(0),
	m_dwTimePerFrame(0),
	m_strKey(""),
	m_eAnimationType(AT_END),
	m_eRenderType(RT_WORLD)
{
}


CAnimatedBGObject::~CAnimatedBGObject()
{
	Release();
}

void CAnimatedBGObject::Initialize()
{
	m_dwTimePerFrame = 100;
	m_fRatio = 1.f;
	m_iFrame = 0;
	m_iMaxFrame = 1;
	m_dwFrameTime = GetTickCount();
	m_eAnimationType = AT_REPEAT;
}

int CAnimatedBGObject::Update()
{

	if (m_eAnimationType == AT_ONCE)
	{
		if (m_iFrame < m_iMaxFrame - 1)
		{
			DWORD dwCurTime = GetTickCount();
			if (m_dwFrameTime + m_dwTimePerFrame < dwCurTime)
			{
				++m_iFrame;
				m_dwFrameTime = dwCurTime;
			}
		}
		else
			return 1;
	}
	else if (m_eAnimationType == AT_REPEAT)
	{
		DWORD dwCurTime = GetTickCount();
		if (m_dwFrameTime + m_dwTimePerFrame < dwCurTime)
		{
			++m_iFrame;
			if (m_iFrame >= m_iMaxFrame) m_iFrame = m_iStartFrame;
			m_dwFrameTime = dwCurTime;
		}
	}
	return 0;
}

void CAnimatedBGObject::LateUpdate()
{
}

void CAnimatedBGObject::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	SIZE tSize = GET_BITMAP_SIZE(m_strKey);
	tSize.cx = tSize.cx / m_iTotalFrame;

	
	POSITION tPos;
	
	switch (m_eRenderType)
	{
	case RT_WORLD:
		tPos = CScrollManager::WorldToScreen(m_tInfo);
		break;
	case RT_SCREEN:
		tPos.fx = m_tInfo.fx;
		tPos.fy = m_tInfo.fy;
		break;
	case RT_END:
		break;
	default:
		break;
	}
	
	SIZE RenderSize = {LONG(tSize.cx * m_fRatio), LONG(tSize.cy * m_fRatio)};
	GdiTransparentBlt(hDC, int(tPos.fx - RenderSize.cx / 2), int(tPos.fy - RenderSize.cy / 2),
		RenderSize.cx, RenderSize.cy, hMemDC, tSize.cx * m_iFrame, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

}

void CAnimatedBGObject::Release()
{
}

void CAnimatedBGObject::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(m_strKey, pPath);
}
