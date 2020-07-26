#include "stdafx.h"
#include "Particle.h"
#include "ScrollManager.h"

CParticle::CParticle():
	m_bBounce(true),
	m_fMagnitude(0.f),
	m_fDirX(0.f),
	m_fDirY(0.f),
	m_fFriction(0.f),
	m_fFallingSpeed(0.f),
	m_tDrawStartPos(POSITION{})
{
}


CParticle::~CParticle()
{
	Release();
}

void CParticle::Gravity()
{
	float fPrevGravityTime = m_fGravityTime;
	m_fGravityTime += 0.1f;

	float fGravityForce = m_fSpeed + 1.f + (0.5f * m_fFallingSpeed *  m_fGravityTime);
	if (fGravityForce - (m_fJumpForce * m_bJump) > 50.f)
	{
		m_fGravityTime = fPrevGravityTime;
		fGravityForce = m_fSpeed + 1.f + (0.5f * m_fFallingSpeed *  m_fGravityTime);
	}

	m_tInfo.fy += fGravityForce;

	if (fGravityForce >= m_fJumpForce * m_bJump)
		m_bFalling = true;
}

void CParticle::Stop()
{
	m_fGravityTime = 0.f;
	m_fJumpForce *= m_fFriction;
	m_fMagnitude *= m_fFriction;
}

void CParticle::Initialize()
{
	m_bJump = true;
	m_bPhysics = true;
	m_bFalling = false;;
	m_bPassible = false;
	m_fSpeed = 5.f;
	m_fGravityTime = 0.f;
	m_fJumpForce = rand() % 5 + 10.f;
	m_fMagnitude = (rand() % 10) / 2.f;
	m_fFriction = 0.f;
	m_fFallingSpeed = 0.f;
}

int CParticle::Update()
{
	if (m_bBounce && m_fJumpForce <= 0.1f)
		m_bLife = false;

	m_tInfo.fx += m_fDirX * m_fMagnitude;
	m_tInfo.fy -= m_fDirY * m_fMagnitude;

	if (m_bJump)	Jump();
	if (m_bPhysics)	Gravity();
	return 0;
}

void CParticle::LateUpdate()
{
	CMoveObj::LateUpdate();
}

void CParticle::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	BOOL bRender = TRUE;

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	
	if (hMemDC)
		bRender = GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - m_tInfo.icx * m_fRatio / 2), 
			static_cast<int>(tPos.fy - m_tInfo.icy * m_fRatio / 2), 
			static_cast<int>(m_tInfo.icx * m_fRatio), 
			static_cast<int>(m_tInfo.icy * m_fRatio),
			hMemDC, static_cast<int>(m_tDrawStartPos.fx), static_cast<int>(m_tDrawStartPos.fy), m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, static_cast<int>(tPos.fx - m_tInfo.icx / 2), static_cast<int>(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);
}

void CParticle::Release()
{
}
