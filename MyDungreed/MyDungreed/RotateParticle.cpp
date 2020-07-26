#include "stdafx.h"
#include "RotateParticle.h"
#include "ScrollManager.h"

CRotateParticle::CRotateParticle()
{
}


CRotateParticle::~CRotateParticle()
{
	Release();
}

void CRotateParticle::Initialize()
{
	m_bJump = true;
	m_bPhysics = true;
	m_bFalling = false;;
	m_bPassible = false;
	m_fSpeed = 5.f;
	m_fGravityTime = 0.f;
	m_fJumpForce = rand() % 5 + 10.f;
	m_fForce = (rand() % 10) / 2.f;
	m_fTorque = (rand() % 10) / 10.f;
	m_fAngle = 0.f;
}

int CRotateParticle::Update()
{
	if (m_fJumpForce <= 0.1f)
		m_bLife = false;

	m_tInfo.fx += m_tDir.fx * m_fForce;
	m_tInfo.fy -= m_tDir.fy * m_fForce;
	m_fAngle += m_fTorque * m_iDir;

	if (m_bJump)	Jump();
	if (m_bPhysics)	Gravity();
	return 0;
}

void CRotateParticle::LateUpdate()
{
	CMoveObj::LateUpdate();
}

void CRotateParticle::Render(HDC hDC)
{
	HDC hMemDC = FIND_ROTATED_DC(m_strKey, m_fAngle);

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	BOOL bRender = GdiTransparentBlt(hDC, 
		static_cast<int>(tPos.fx - m_tInfo.icx / 2), 
		static_cast<int>(tPos.fy - m_tInfo.icy / 2),
		m_tInfo.icx, m_tInfo.icy, hMemDC, 0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	int iA = 0;
}

void CRotateParticle::Release()
{
}

void CRotateParticle::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(strKey, pPath);
}

void CRotateParticle::Stop()
{
	m_fGravityTime = 0.f;
	m_fJumpForce *= 0.9f;
	m_fForce *= 0.9f;
	m_fTorque *= 0.5f;
}
