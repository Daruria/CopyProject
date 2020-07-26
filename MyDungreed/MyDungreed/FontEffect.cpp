#include "stdafx.h"
#include "FontEffect.h"


CFontEffect::CFontEffect() :
	m_fRatio(0.f),
	m_fDist(0.f),
	m_fSpeed(0.f),
	m_strKey(""),
	m_strSentence(""),
	m_tStartPos(POSITION{}),
	m_tEndPos(POSITION{}),
	m_tDir(POSITION{})
{
}


CFontEffect::~CFontEffect()
{
	Release();
}

void CFontEffect::Initialize()
{
	m_tInfo.icx = 9;
	m_tInfo.icy = 13;
}

int CFontEffect::Update()
{
	m_tInfo.fx += m_fSpeed * m_tDir.fx;
	m_tInfo.fy += m_fSpeed * m_tDir.fy;
	if (CMath::Distance(m_tInfo.fx, m_tInfo.fy, m_tStartPos.fx, m_tStartPos.fy) > m_fDist)
		m_bLife = false;
	return 0;
}

void CFontEffect::LateUpdate()
{
}

void CFontEffect::Render(HDC hDC)
{
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);


	GET_SINGLE(CFontManager)->Render(hDC, tPos.fx, tPos.fy, m_eType, m_fRatio, m_strSentence);

	//SIZE tSize = { m_tInfo.icx * m_fRatio, m_tInfo.icy * m_fRatio };

	//int iLength = m_strNumber.size();
	//int iSize = iLength * tSize.cx;

	//for (int i = 0; i < m_strNumber.size(); ++i)
	//{
	//	int iNumber = static_cast<int>(m_strNumber[i] - '0');
	//	NumberRender(hDC, POSITION{ tPos.fx - iSize * 0.5f + tSize.cx * i, tPos.fy }, tSize, iNumber);
	//}
}

void CFontEffect::Release()
{
}

void CFontEffect::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(strKey, pPath);
}

void CFontEffect::NumberRender(HDC hDC, const POSITION & tPos, const SIZE & tSize, int iNumber)
{
	//HDC hMemDC = FIND_BITMAP_DC(m_strKey);
	//
	//BOOL bRender = TRUE;
	//bRender = GdiTransparentBlt(hDC, tPos.fx - tSize.cx / 2, tPos.fy - tSize.cy/2, tSize.cx, tSize.cy, hMemDC, 
	//	m_tInfo.icx * iNumber, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	//int iA = 0;

	GET_SINGLE(CFontManager)->Render(hDC, tPos.fx, tPos.fy, m_eType, 3.f, to_string(iNumber));
}

void CFontEffect::MoveDirSetting(const POSITION & tStartPos, const POSITION & tEndPos)
{
	SetStartPos(tStartPos);
	SetEndPos(tEndPos);
	m_tDir.fx = tEndPos.fx - tStartPos.fx;
	m_tDir.fy = tEndPos.fy - tStartPos.fy;
	m_fDist = sqrtf(m_tDir.fx * m_tDir.fx + m_tDir.fy * m_tDir.fy);

	CMath::NormalizeXY(m_tDir.fx, m_tDir.fy);
}