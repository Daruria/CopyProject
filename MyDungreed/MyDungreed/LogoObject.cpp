#include "stdafx.h"
#include "LogoObject.h"


CLogoObject::CLogoObject():
	m_fDirAngle(0.f)
{
}


CLogoObject::~CLogoObject()
{
	Release();
}

void CLogoObject::Initialize()
{
	CMoveObj::Initialize();
	m_dwFrameMaxTime = 100;
	m_iFrame = 0;
	m_fDirAngle = 0.f;
	m_iFrameDir = 1;
}

int CLogoObject::Update()
{
	CMoveObj::Update();

	float fdx = m_fSpeed * cosf(m_fDirAngle);
	float fdy = m_fSpeed * sinf(m_fDirAngle);

	m_tInfo.fx += fdx;
	m_tInfo.fy += fdy;

	return 0;
}

void CLogoObject::LateUpdate()
{
}

void CLogoObject::Render(HDC hDC)
{
	CMoveObj::Render(hDC);
}

void CLogoObject::Release()
{
}
