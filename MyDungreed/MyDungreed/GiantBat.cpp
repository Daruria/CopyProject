#include "stdafx.h"
#include "GiantBat.h"
#include "Player.h"
#include "Bullet.h"

CGiantBat::CGiantBat() :
	m_bCanShoot(true),
	m_bMotionChange(true),
	m_fAngle(0.f),
	m_iShootCount(0),
	m_dwMotionTime(0),
	m_dwChangeTime(0)
{
}


CGiantBat::~CGiantBat()
{
	Release();
}

void CGiantBat::Initialize()
{
	m_strName = "Giant Bat";
	INSERT_BITMAP("GiantBatIdleR", L"Image/Monster/GiantBat/Idle/GiantBatSpriteR.bmp");
	INSERT_BITMAP("GiantBatIdleL", L"Image/Monster/GiantBat/Idle/GiantBatSpriteL.bmp");

	INSERT_BITMAP("GiantBatAttackR", L"Image/Monster/GiantBat/Attack/GiantBatAttackSpriteR.bmp");
	INSERT_BITMAP("GiantBatAttackL", L"Image/Monster/GiantBat/Attack/GiantBatAttackSpriteL.bmp");

	m_tInfo.icx = 64;
	m_tInfo.icy = 64;

	CMonster::Initialize();

	m_dwMotionTime = GetTickCount();
	m_dwChangeTime = 3000;

	Idle();
	m_iFrame = 0;
	m_iFrameDir = 1;
	m_dwFrameMaxTime = 100;
	m_fRatio = 4.f;

	m_bFalling = false;
	m_bJump = false;
	m_bPassible = true;
	m_bPhysics = true;
	m_fSpeed = 1.f;
	m_fJumpForce = 15.f;

	m_tHitBox = { 0.f, 0.f, int((m_tInfo.icx - 32) * m_fRatio), int((m_tInfo.icy - 32) * m_fRatio) };
	m_tAttackBox = {};

	m_bLife = true;
	m_tCharInfo.iHPMax = 100;
	m_tCharInfo.iHP = m_tCharInfo.iHPMax;
	m_tCharInfo.fAttack = 7.f;
	m_tCharInfo.fArmor = 3.f;
	m_tMoveDir = {};
}

int CGiantBat::Update()
{
	CMonster::Update();
	AI();
	if (m_tInfo.fx < 10.f)
		m_tInfo.fx = 10.f;
	if (m_tInfo.fy < 10.f)
		m_tInfo.fy = 10.f;
	return 0;
}

void CGiantBat::LateUpdate()
{
	CMonster::LateUpdate();
}

void CGiantBat::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	BOOL bRender = GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));


#ifdef _DEBUG
	tPos = CScrollManager::WorldToScreen(m_tInfo);
	CMath::DrawBox(hDC, int(tPos.fx + m_tHitBox.fx - m_tHitBox.icx / 2),
		int(tPos.fy + m_tHitBox.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);

#endif // DEBUG

	CMonster::Render(hDC);
}

void CGiantBat::Release()
{
}

void CGiantBat::Attack()
{
	m_dwMotion = GBM_ATTACK;
	m_iMaxFrame = 10;
}

void CGiantBat::Idle()
{
	m_dwMotion = GBM_IDLE;
	m_iMaxFrame = 7;
	if (m_iFrame >= 7)
		m_iFrame = 0;
}

void CGiantBat::Move()
{
}

void CGiantBat::Die()
{
}

void CGiantBat::Hit(const float fAttack, const INFO & tAttackerInfo)
{
	CMonster::Hit(fAttack, tAttackerInfo);
}

void CGiantBat::AddAttackEffect(float fx, float fy)
{
}

void CGiantBat::Dying(float fx, float fy)
{
	CMonster::Dying(fx, fy);

	m_bLife = false;

	int iGolds = rand() % 5;
	for (int i = 0; i < iGolds; ++i)
		AddGold(10);
}

void CGiantBat::AI()
{
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	if (!pPlayer)
		return;

	float fdx = pPlayer->GetInfo().fx - m_tInfo.fx;
	float fdy = pPlayer->GetInfo().fy - m_tInfo.fy;

	float fDist = sqrtf(fdx * fdx + fdy * fdy);

	m_bDir = fdx > 0;
	if (m_bMotionChange)
	{
		m_bMotionChange = false;
		m_dwMotionTime = GetTickCount();
		if (fDist <= 400)
		{
			Attack();
			m_iShootCount = 0;
			m_fAngle = fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);
		}
		else
		{
			Idle();
			m_tMoveDir = { float(rand() % 200 - 100), float(rand() % 200 - 100) };
			CMath::NormalizeXY(m_tMoveDir.fx, m_tMoveDir.fy);
		}
	}

	if (m_dwChangeTime + m_dwMotionTime <= GetTickCount())
		m_bMotionChange = true;


	switch (m_dwMotion)
	{
	case GBM_ATTACK:
		if (m_iFrame % 3 == 1 && m_iShootCount < 3)
		{
			if (m_bCanShoot)
			{
				Shoot(m_fAngle);
				Shoot(float(m_fAngle + (PI / 6)));
				Shoot(float(m_fAngle - (PI / 6)));
				m_iShootCount++;
				m_bCanShoot = false;
			}
		}
		else
			m_bCanShoot = true;
		break;
	case GBM_IDLE:
	{
		m_tInfo.fx += m_tMoveDir.fx * m_fSpeed;
		m_tInfo.fy += m_tMoveDir.fy * m_fSpeed;
	}
	break;
	default:
		break;
	}

}

void CGiantBat::Shoot(float fAngle)
{
	CBullet* pBullet = new CBullet;
	pBullet->Initialize();
	pBullet->SetAngle(fAngle);
	pBullet->SetRatio(4.0f);
	pBullet->SetSpeed(5.f);
	pBullet->SetAttack(2.f);
	pBullet->SetMaxFrame(5);
	pBullet->SetImage("Ball", L"Image/Monster/GiantBat/RangeBallBullet/Bullet/RangeBallBullet.bmp");
	pBullet->SetBulletType(BT_BALL);
	pBullet->SetName(m_strName);
	pBullet->SetRangeDist(600.f);
	pBullet->SetInfo(m_tInfo.fx + cosf(fAngle) * m_tInfo.icx * 0.5f, m_tInfo.fy + sinf(fAngle) * m_tInfo.icx * 0.5f, 14, 14);
	pBullet->SetStartPos();
	GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BULLET, pBullet);

}

HDC CGiantBat::MotionDC()
{
	switch (m_dwMotion)
	{
	case GBM_IDLE:
		return m_bDir ? FIND_BITMAP_DC("GiantBatIdleR") : FIND_BITMAP_DC("GiantBatIdleL");
	case GBM_ATTACK:
	{
		SIZE tSize = GET_BITMAP_SIZE("GiantBatAttackR");
		int iA = 0;
	}
	return m_bDir ? FIND_BITMAP_DC("GiantBatAttackR") : FIND_BITMAP_DC("GiantBatAttackL");
	case GBM_DIE:
		break;
	case GBM_END:
		break;
	default:
		break;
	}

	return NULL;
}
