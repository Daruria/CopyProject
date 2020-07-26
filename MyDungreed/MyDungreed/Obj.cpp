#include "stdafx.h"
#include "Obj.h"


CObj::CObj():
	m_bLife(true),
	m_tInfo(INFO{})
{
}


CObj::~CObj()
{
}

void CObj::SetInfo(float fx, float fy, int icx, int icy)
{
	m_tInfo.fx = fx;
	m_tInfo.fy = fy;
	m_tInfo.icx = icx;
	m_tInfo.icy = icy;
}

void CObj::SetInfo(const POSITION & tPos)
{
	m_tInfo.fx = tPos.fx;
	m_tInfo.fy = tPos.fy;
}

void CObj::SetInfo(const INFO & tInfo)
{
	m_tInfo.fx = tInfo.fx;
	m_tInfo.fy = tInfo.fy;
	m_tInfo.icx = tInfo.icx;
	m_tInfo.icy = tInfo.icy;
}

void CObj::SetInfo(float fx, float fy)
{
	m_tInfo.fx = fx;
	m_tInfo.fy = fy;
}

void CObj::SetInfo(int icx, int icy)
{
	m_tInfo.icx = icx;
	m_tInfo.icy = icy;
}

void CObj::SetInfofx(float fx)
{
	m_tInfo.fx = fx;
}

void CObj::SetInfofy(float fy)
{
	m_tInfo.fy = fy;
}

void CObj::AddInfofx(float fdx)
{
	m_tInfo.fx += fdx;
}

void CObj::AddInfofy(float fdy)
{
	m_tInfo.fy += fdy;
}
