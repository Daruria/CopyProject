#include "stdafx.h"
#include "BossSkelHand.h"
#include "Laser.h"

CBossSkelHand::CBossSkelHand():
	m_bChangePattern(true),
	m_bMoveForShoot(false),
	m_bCanShoot(true),
	m_bLaserFinish(true),
	m_iMoveCurTime(0),
	m_iMoveTotalTime(0),
	m_fMovePosY(0.f),
	m_fMoveSpeed(0.f),
	m_eHandType(HRL_END)
{
}


CBossSkelHand::~CBossSkelHand()
{
	Release();
}

void CBossSkelHand::Initialize()
{
	INSERT_BITMAP("BossSkellRightHandIdle", L"Image/Monster/BossSkell/RightHand/Idle/SkellBossRightHandIdle.bmp");
	INSERT_BITMAP("BossSkellLeftHandIdle", L"Image/Monster/BossSkell/LeftHand/Idle/SkellBossLeftHandIdle.bmp");

	INSERT_BITMAP("BossSkellRightHandAttack", L"Image/Monster/BossSkell/RightHand/Attack/SkellBossRightHandAttack.bmp");
	INSERT_BITMAP("BossSkellLeftHandAttack", L"Image/Monster/BossSkell/LeftHand/Attack/SkellBossLeftHandAttack.bmp");

	m_fRatio = 4.f;
	m_tInfo.icx = 57;
	m_tInfo.icy = 69;
	m_fMovePosY = 0.f;
	m_fMoveSpeed = 0.f;
	m_iMoveTotalTime = 20;
	m_iMoveCurTime = 0;
	m_iFrameDir = 1;
	m_dwFrameTime = GetTickCount();
	m_dwFrameMaxTime = 50;
	Idle();
	m_bLife = true;
}

int CBossSkelHand::Update()
{
	if (m_bLife)
		CMonster::Update();

	if (m_dwMotion == SBHM_ATTACK)
	{
		if (m_iFrame == 10 && m_bCanShoot)
		{
			m_bCanShoot = false;
			LaserShot();
		}
		if (m_iFrame >= m_iMaxFrame - 1)
		{
			m_bCanShoot = true;
			m_bLaserFinish = true;
			Idle();
		}
	}

	if (m_bMoveForShoot)
	{
		m_tInfo.fy += m_fMoveSpeed;
		m_iMoveCurTime++;
		if (m_iMoveCurTime >= m_iMoveTotalTime)
		{
			m_bMoveForShoot = false;
			m_iMoveCurTime = 0;
			Attack();
		}
	}

	return 0;
}

void CBossSkelHand::LateUpdate()
{
}

void CBossSkelHand::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	BOOL bRender = GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx,
		0,
		m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));
	
	int iA = 0;
}

void CBossSkelHand::Release()
{
}

void CBossSkelHand::Attack()
{
	m_dwMotion = SBHM_ATTACK;
	m_tInfo.icx = 70;
	m_tInfo.icy = 80;
	m_iFrame = 0;
	m_iMaxFrame = 18;
}

void CBossSkelHand::Idle()
{
	m_dwMotion = SBHM_IDLE;
	m_tInfo.icx = 57;
	m_tInfo.icy = 69;
	m_iFrame = 0;
	m_iMaxFrame = 10;
}

void CBossSkelHand::MoveForShoot(float fy)
{
	m_fMovePosY = fy;
	m_fMoveSpeed = (fy - m_tInfo.fy) / m_iMoveTotalTime;
	m_bMoveForShoot = true;
}

HDC CBossSkelHand::MotionDC()
{
	switch (m_eHandType)
	{
	case HRL_RIGHT:
		switch (m_dwMotion)
		{
		case SBHM_IDLE:
			return FIND_BITMAP_DC("BossSkellRightHandIdle");
		case SBHM_ATTACK:
		{
			SIZE tSize = GET_BITMAP_SIZE("BossSkellRightHandAttack");
			int iA = 0;
		}
			return FIND_BITMAP_DC("BossSkellRightHandAttack");
		}
	case HRL_LEFT:
		switch (m_dwMotion)
		{
		case SBHM_IDLE:
			return FIND_BITMAP_DC("BossSkellLeftHandIdle");
		case SBHM_ATTACK:
		{
			SIZE tSize = GET_BITMAP_SIZE("BossSkellLeftHandAttack");
			int iA = 0;
		}
			return FIND_BITMAP_DC("BossSkellLeftHandAttack");
		}
	}

	return 0;
}

void CBossSkelHand::LaserShot()
{
	
	CLaser* pLaser = new CLaser;
	pLaser->Initialize();
	switch (m_eHandType)
	{
	case HRL_RIGHT:
		pLaser->SetInfo(m_tInfo.fx - 30, m_tInfo.fy + 20);
		pLaser->SetRightHand(m_tInfo.fx);
		break;
	case HRL_LEFT:
		pLaser->SetInfo(m_tInfo.fx + 30, m_tInfo.fy + 20);
		pLaser->SetLeftHand(m_tInfo.fx);
		break;
	case HRL_END:
		break;
	default:
		break;
	}
	GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BOSS, pLaser);
}
