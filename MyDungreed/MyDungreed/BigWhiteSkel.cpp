#include "stdafx.h"
#include "BigWhiteSkel.h"
#include "Player.h"

CBigWhiteSkel::CBigWhiteSkel() :
	m_dwActionTime(0),
	m_dwActionChangeTime(0),
	m_vecMotionInfo()
{
}


CBigWhiteSkel::~CBigWhiteSkel()
{
	Release();
}

void CBigWhiteSkel::Initialize()
{
	m_strName = "Big White Skel";

	INSERT_BITMAP("BigWhiteSkelIdleR", L"Image/Monster/BigWhiteSkel/BigWhiteSkelIdleR.bmp");
	INSERT_BITMAP("BigWhiteSkelAttackR", L"Image/Monster/BigWhiteSkel/BigWhiteSkelAttackR.bmp");
	INSERT_BITMAP("BigWhiteSkelMoveR", L"Image/Monster/BigWhiteSkel/BigWhiteSkelMoveR.bmp");

	INSERT_BITMAP("BigWhiteSkelDie", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticle.bmp");

	INSERT_BITMAP("BigWhiteSkelIdleL", L"Image/Monster/BigWhiteSkel/BigWhiteSkelIdleL.bmp");
	INSERT_BITMAP("BigWhiteSkelAttackL", L"Image/Monster/BigWhiteSkel/BigWhiteSkelAttackL.bmp");
	INSERT_BITMAP("BigWhiteSkelMoveL", L"Image/Monster/BigWhiteSkel/BigWhiteSkelMoveL.bmp");

	m_vecMotionInfo.assign(BWS_END, INFO{});
	m_vecMotionInfo[BWS_IDLE] = { -12, 1, 132, 120 };
	m_vecMotionInfo[BWS_ATTACK] = { 60, -36, 284, 192 };
	m_vecMotionInfo[BWS_MOVE] = { -12, 1, 132, 120 };
	m_vecMotionInfo[BWS_DIE] = { 0, 0, 92, 80 };

	m_tInfo.icx = 132;
	m_tInfo.icy = 120;

	CMonster::Initialize();

	m_dwActionChangeTime = 1200;
	m_dwActionTime = GetTickCount();

	Idle();
	m_iFrame = 0;
	m_iFrameDir = 1;
	m_dwFrameMaxTime = 100;

	m_bFalling = false;
	m_bJump = false;
	m_bPassible = false;
	m_bPhysics = true;
	m_fSpeed = 5.f;
	m_fJumpForce = 15.f;

	m_tHitBox = { 0, 1, 66, 118 };
	m_tAttackBox = { m_tInfo.fx, m_tInfo.fy, 284, 192 };

	m_bLife = true;
	m_tCharInfo.iHPMax = 100;
	m_tCharInfo.iHP = m_tCharInfo.iHPMax;
	m_tCharInfo.fAttack = 7.f;
	m_tCharInfo.fArmor = 3.f;
}

int CBigWhiteSkel::Update()
{
	if (/*m_dwActionTime + m_dwActionChangeTime < GetTickCount()*/
		m_iFrame == m_iMaxFrame - 1)
	{
		CObj* pTarget = GET_SINGLE(CObjectManager)->GetPlayer();

		float fdx = pTarget->GetInfo().fx - m_tInfo.fx;
		float fdy = pTarget->GetInfo().fy - m_tInfo.fy;

		float fDist = sqrtf(fdx * fdx + fdy * fdy);

		m_bDir = fdx > 0;

		if (fDist >= 100.f && fDist <= 400.f)
			Move();
		else if (fDist < 100.f)
		{
			Attack();
			//m_dwActionTime = GetTickCount();
		}
		else
			Idle();
	}

	if (m_tCharInfo.iHP <= 0)
	{
		return 0;
	}

	if (m_bPhysics) Gravity();

	if (m_dwMotion == BWS_MOVE)
		m_bDir ? MoveRight() : MoveLeft();

	CMoveObj::Update();

	return 0;
}

void CBigWhiteSkel::LateUpdate()
{
	CMoveObj::LateUpdate();
}

void CBigWhiteSkel::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();

	int iDir = m_bDir ? 1 : -1;
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);
	tPos.fx += iDir * m_vecMotionInfo[m_dwMotion].fx;
	tPos.fy += m_vecMotionInfo[m_dwMotion].fy;
	BOOL bRender;
	if (hMemDC)
		bRender = GdiTransparentBlt(hDC,
			int(tPos.fx - m_vecMotionInfo[m_dwMotion].icx / 2),
			int(tPos.fy - m_vecMotionInfo[m_dwMotion].icy / 2),
			m_vecMotionInfo[m_dwMotion].icx, m_vecMotionInfo[m_dwMotion].icy,
			hMemDC,
			m_vecMotionInfo[m_dwMotion].icx * m_iFrame, 0,
			m_vecMotionInfo[m_dwMotion].icx, m_vecMotionInfo[m_dwMotion].icy, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, int(tPos.fx - m_tInfo.icx / 2), int(tPos.fy - m_tInfo.icy / 2), m_tInfo.icx, m_tInfo.icy);

	CMonster::Render(hDC);

#ifdef _DEBUG
	tPos = CScrollManager::WorldToScreen(m_tInfo);
	CMath::DrawBox(hDC, 
		int(tPos.fx + m_tHitBox.fx - m_tHitBox.icx / 2),
		int(tPos.fy + m_tHitBox.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);
	if (m_bAttack)
	{
		tPos = CScrollManager::WorldToScreen(m_tAttackBox);
		CMath::DrawBox(hDC, 
			int(tPos.fx - m_tAttackBox.icx / 2),
			int(tPos.fy - m_tAttackBox.icy / 2), m_tAttackBox.icx, m_tAttackBox.icy);
	}
#endif // DEBUG
}

void CBigWhiteSkel::Release()
{
}

void CBigWhiteSkel::Attack()
{
	m_dwMotion = BWS_ATTACK;
	m_iFrame = 0;
	m_iMaxFrame = 12;

	m_bAttack = true;
	m_sAttackBoxLifeTime = m_iMaxFrame;

	int iDir = m_bDir ? 1 : -1;
	m_tAttackBox.fx = m_tInfo.fx + 60 * iDir;
	m_tAttackBox.fy = m_tInfo.fy - 36;
}

void CBigWhiteSkel::Idle()
{
	m_dwMotion = BWS_IDLE;
	m_iMaxFrame = 6;
}

void CBigWhiteSkel::Move()
{
	m_dwMotion = BWS_MOVE;
	m_iMaxFrame = 6;
}

void CBigWhiteSkel::Die()
{
	m_dwMotion = BWS_DIE;
	m_iMaxFrame = 1;
}

void CBigWhiteSkel::Hit(const float fAttack, const INFO& tAttackerInfo)
{
	CMonster::Hit(fAttack, tAttackerInfo);
}

void CBigWhiteSkel::AddAttackEffect(float fx, float fy)
{
	float m_fSlashAngle = float((rand() % 360 - 180) * (180 / PI));
	GET_SINGLE(CEffectManager)->AddRotateEffect("Slash", L"Image/Effect/Slash/SlashFX", fx, fy, 168, 168, 4, m_fSlashAngle, 1.f);
}

void CBigWhiteSkel::MoveLeft()
{
	m_tInfo.fx -= m_fSpeed;
}

void CBigWhiteSkel::MoveRight()
{
	m_tInfo.fx += m_fSpeed;
}

HDC CBigWhiteSkel::MotionDC()
{
	if (m_bDir)
		switch (m_dwMotion)
		{
		case BWS_IDLE:
			return FIND_BITMAP_DC("BigWhiteSkelIdleR");
		case BWS_ATTACK:
			return FIND_BITMAP_DC("BigWhiteSkelAttackR");
		case BWS_MOVE:
			return FIND_BITMAP_DC("BigWhiteSkelMoveR");
		case BWS_DIE:
			return FIND_BITMAP_DC("BigWhiteSkelDie");
		}
	else
		switch (m_dwMotion)
		{
		case BWS_IDLE:
			return FIND_BITMAP_DC("BigWhiteSkelIdleL");
		case BWS_ATTACK:
			return FIND_BITMAP_DC("BigWhiteSkelAttackL");
		case BWS_MOVE:
			return FIND_BITMAP_DC("BigWhiteSkelMoveL");
		case BWS_DIE:
			return FIND_BITMAP_DC("BigWhiteSkelDie");
		}
	return 0;
}

void CBigWhiteSkel::Dying(float fx, float fy)
{
	CMonster::Dying(fx, fy);

	float fDirX = m_tInfo.fx - fx;
	float fDirY = m_tInfo.fy - fy + 10.f;

	float fDist = sqrtf(fDirX * fDirX + fDirY * fDirY);
	fDirX /= fDist;
	fDirY /= fDist;

	m_bLife = false;
	m_dwMotion = BWS_DIE;
	m_iFrame = 0;
	m_iMaxFrame = 1;

	AddRotateParticle("BigWhiteSkelParticleHead", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleHead.bmp", fDirX, fDirY);
	AddRotateParticle("BigWhiteSkelParticleBody", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBody.bmp", fDirX, fDirY);

	AddParticle("BigWhiteSkelParticleBone1", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone1.bmp", fDirX, fDirY);
	AddParticle("BigWhiteSkelParticleBone2", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone2.bmp", fDirX, fDirY);
	AddParticle("BigWhiteSkelParticleBone3", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone3.bmp", fDirX, fDirY);
	AddParticle("BigWhiteSkelParticleBone4", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone4.bmp", fDirX, fDirY);
	AddParticle("BigWhiteSkelParticleBone5", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone5.bmp", fDirX, fDirY);
	AddParticle("BigWhiteSkelParticleBone6", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone6.bmp", fDirX, fDirY);
	AddParticle("BigWhiteSkelParticleBone7", L"Image/Monster/BigWhiteSkel/BigWhiteSkelParticleBone7.bmp", fDirX, fDirY);

	int iGolds = rand() % 5;
	for (int i = 0; i < iGolds; ++i)
		AddGold(10);
}
