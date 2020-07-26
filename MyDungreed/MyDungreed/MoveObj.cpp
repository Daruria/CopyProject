#include "stdafx.h"
#include "MoveObj.h"
#include "ScrollManager.h"

CMoveObj::CMoveObj() :
	m_bJump(false),
	m_bPhysics(false),
	m_bFalling(false),
	m_bPassible(false),
	m_fSpeed(0.f),
	m_fGravityTime(0.f),
	m_fJumpForce(0.f),
	
	m_dwMotion(0),
	m_fRatio(1.f),
	m_iFrame(0),
	m_iFrameDir(0),
	m_iMaxFrame(0),
	m_dwFrameTime(0),
	m_dwFrameMaxTime(0),
	
	m_bAttack(false),
	m_sAttackLife(0),
	m_sAttackBoxLifeTime(0),
	m_tPrevPos(POSITION{}),
	m_tHitBox(INFO{}),
	m_tAttackBox(INFO{}),
	m_strKey("")
{
}

CMoveObj::~CMoveObj()
{
	Release();
}

void CMoveObj::SetHitBox(const float fx, const float fy, const int icx, const int icy)
{
	m_tHitBox.fx = fx;
	m_tHitBox.fy = fy;
	m_tHitBox.icx = icx;
	m_tHitBox.icy = icy;
}

const INFO CMoveObj::GetHitBoxWorldInfo()
{
	return INFO{ m_tInfo.fx + m_tHitBox.fx, m_tInfo.fy + m_tHitBox.fy, m_tHitBox.icx, m_tHitBox.icy };
}

const INFO CMoveObj::GetHitBoxPrevIWorldInfo()
{
	return INFO{ m_tPrevPos.fx + m_tHitBox.fx, m_tPrevPos.fy + m_tHitBox.fy, m_tHitBox.icx, m_tHitBox.icy };
}

const int CMoveObj::GetPushDistTop()
{
	return static_cast<int>(m_tHitBox.icy / 2 - m_tHitBox.fy);
}

const int CMoveObj::GetPushDistBottom()
{
	return static_cast<int>(m_tHitBox.icy / 2 + m_tHitBox.fy);
}

const int CMoveObj::GetPushDistLeft()
{
	return static_cast<int>(m_tHitBox.icx / 2 - m_tHitBox.fx);
}

const int CMoveObj::GetPushDistRight()
{
	return static_cast<int>(m_tHitBox.icx / 2 + m_tHitBox.fx);
}

void CMoveObj::Jump()
{
	m_tInfo.fy -= m_fJumpForce;
}

void CMoveObj::Hit(const float fAttack, const INFO& tAttackerInfo)
{
	float fdx = m_tInfo.fx - tAttackerInfo.fx;
	float fdy = m_tInfo.fy - tAttackerInfo.fy;

	CMath::NormalizeXY(fdx, fdy);

	m_tInfo.fx += fdx * 10.f;
}

void CMoveObj::Gravity()
{
	float fPrevGravityTime = m_fGravityTime;
	m_fGravityTime += 0.1f;

	float fGravityForce = m_fSpeed + 1.f + (0.5f * GRAVITY *  m_fGravityTime);
	if (fGravityForce - (m_fJumpForce * m_bJump) > 50.f)
	{
		m_fGravityTime = fPrevGravityTime;
		fGravityForce = m_fSpeed + 1.f + (0.5f * GRAVITY *  m_fGravityTime);
	}

	m_tInfo.fy += fGravityForce;

	if (fGravityForce >= m_fJumpForce * m_bJump)
		m_bFalling = true;
}

void CMoveObj::Dying(float fx, float fy)
{
}

void CMoveObj::Initialize()
{
	m_dwFrameTime = GetTickCount();
}

int CMoveObj::Update()
{
	if (m_iFrame >= m_iMaxFrame) 
		m_iFrame = m_iMaxFrame - 1;

	int iPreFrame = m_iFrame;
	DWORD dwCurTime = GetTickCount();
	if (m_dwFrameTime + m_dwFrameMaxTime < dwCurTime)
	{
		int iFrame = m_iFrame + m_iFrameDir;
		m_iFrame = iFrame >= m_iMaxFrame ? 0 : iFrame < 0 ? m_iMaxFrame - 1 : iFrame;
		m_dwFrameTime = dwCurTime;
	}

	if (m_sAttackLife >= m_sAttackBoxLifeTime)
	{
		m_bAttack = false;
		m_sAttackLife = 0;
	}
	else if(iPreFrame != m_iFrame)
		m_sAttackLife++;
	
	return 0;
}

void CMoveObj::LateUpdate()
{
	SetPrevPos(m_tInfo);
}

void CMoveObj::Render(HDC hDC)
{
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	HDC hMemDC = FIND_BITMAP_DC(m_strKey);
	
	SIZE tSize = { static_cast<LONG>(m_tInfo.icx * m_fRatio), static_cast<LONG>(m_tInfo.icy * m_fRatio) };

	if (hMemDC)
		GdiTransparentBlt(hDC, 
			static_cast<int>(tPos.fx - tSize.cx / 2), 
			static_cast<int>(tPos.fy - tSize.cy / 2), tSize.cx, tSize.cy, hMemDC, m_iFrame * m_tInfo.icx, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, static_cast<int>(tPos.fx - m_tInfo.icx / 2), static_cast<int>(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);


}

void CMoveObj::Release()
{
}
