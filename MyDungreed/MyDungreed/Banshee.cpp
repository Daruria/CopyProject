#include "stdafx.h"
#include "Banshee.h"
#include "Player.h"
#include "Bullet.h"

CBanshee::CBanshee():
	m_bActionChange(true),
	m_dwActionTime(0),
	m_dwActionCool(0),
	m_bCanShoot(true)
{
}


CBanshee::~CBanshee()
{
	Release();
}

void CBanshee::Initialize()
{
	m_strName = "Banshee";

	INSERT_BITMAP("BansheeIdleSpriteR", L"Image/Monster/Banshee/Idle/BansheeIdleSpriteR.bmp");
	INSERT_BITMAP("BansheeIdleSpriteL", L"Image/Monster/Banshee/Idle/BansheeIdleSpriteL.bmp");

	INSERT_BITMAP("BansheeAttackSpriteR", L"Image/Monster/Banshee/Attack/BansheeAttackSpriteR.bmp");
	INSERT_BITMAP("BansheeAttackSpriteL", L"Image/Monster/Banshee/Attack/BansheeAttackSpriteL.bmp");

	m_tInfo.icx = 20;
	m_tInfo.icy = 22;

	CMonster::Initialize();

	m_dwActionCool = 3000;
	m_dwActionTime = GetTickCount();

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

	m_tHitBox = { 0.f, 0.f, int((m_tInfo.icx) * m_fRatio), int((m_tInfo.icy) * m_fRatio) };
	m_tAttackBox = {};

	m_bLife = true;
	m_tCharInfo.iHPMax = 100;
	m_tCharInfo.iHP = m_tCharInfo.iHPMax;
	m_tCharInfo.fAttack = 7.f;
	m_tCharInfo.fArmor = 3.f;
	m_tMoveDir = {};
}

int CBanshee::Update()
{
	CMonster::Update();
	AI();

	return 0;
}

void CBanshee::LateUpdate()
{
	CMonster::LateUpdate();
}

void CBanshee::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_tInfo.icx * m_iFrame, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	CMonster::Render(hDC);
}

void CBanshee::Release()
{
}

void CBanshee::Attack()
{
	m_dwMotion = BM_ATTACK;
	m_iFrame = 0;
	m_iMaxFrame = 6;
	GET_SINGLE(CSoundMgr)->PlaySound(L"high_pitch_scream_gverb.wav", CSoundMgr::EFFECT);
}

void CBanshee::Idle()
{
	m_dwMotion = BM_IDLE;
	m_iFrame = 0;
	m_iMaxFrame = 6;
}

void CBanshee::Move()
{
}

void CBanshee::Die()
{
}

void CBanshee::Hit(const float fAttack, const INFO & tAttackerInfo)
{
	CMonster::Hit(fAttack, tAttackerInfo);
}

void CBanshee::AddAttackEffect(float fx, float fy)
{
}

void CBanshee::Dying(float fx, float fy)
{
	CMonster::Dying(fx, fy);
	m_bLife = false;

	int iGolds = rand() % 5;
	for (int i = 0; i < iGolds; ++i)
		AddGold(10);
}

HDC CBanshee::MotionDC()
{
	switch (m_dwMotion)
	{
	case BM_IDLE:
		return m_bDir ? FIND_BITMAP_DC("BansheeIdleSpriteR") : FIND_BITMAP_DC("BansheeIdleSpriteL");
	case BM_ATTACK:
		return m_bDir ? FIND_BITMAP_DC("BansheeAttackSpriteR") : FIND_BITMAP_DC("BansheeAttackSpriteL");
	case BM_END:
		break;
	default:
		break;
	}

	return NULL;
}

void CBanshee::AI()
{
	DWORD dwCurTime = GetTickCount();
	if (m_dwActionTime + m_dwActionCool <= dwCurTime)
		m_bActionChange = true;

	if (m_bActionChange)
	{
		m_bActionChange = false;
		m_dwActionTime = dwCurTime;

		CObj* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
		
		float fdx = pPlayer->GetInfo().fx - m_tInfo.fx;
		float fdy = pPlayer->GetInfo().fy - m_tInfo.fy;

		float fDist = sqrtf(fdx * fdx + fdy * fdy);

		if (fDist <= 400.f)
		{
			m_bDir = fdx > 0;
			Attack();
		}
		else
		{
			float fDirX = float(rand() % 100 - 50);
			float fDirY = float(rand() % 100 - 50);
			CMath::NormalizeXY(fDirX, fDirY);
			m_tMoveDir = {fDirX, fDirY};
			Idle();
		}
	}

	switch (m_dwMotion)
	{
	case BM_IDLE:
		m_tInfo.fx += m_tMoveDir.fx * m_fSpeed;
		m_tInfo.fy += m_tMoveDir.fy * m_fSpeed;
		break;
	case BM_ATTACK:
		if (m_iFrame == 0)
		{
			if (!m_bCanShoot)
			{
				Idle();
				m_tMoveDir.fx = 0.f;
				m_tMoveDir.fy = 0.f;
				m_bCanShoot = true;
			}
		}
		else if (m_iFrame == 1)
		{
			if (m_bCanShoot)
			{
				m_bCanShoot = false;
				for (int i = 0; i < 12; ++i)
					Shoot(float(i * PI / 6));
			}
		}
		break;
	case BM_END:
		break;
	default:
		break;
	}


}

void CBanshee::Shoot(float fAngle)
{
	CBullet* pBullet = new CBullet;
	pBullet->Initialize();
	pBullet->SetAngle(fAngle);
	pBullet->SetRatio(4.0f);
	pBullet->SetSpeed(5.f);
	pBullet->SetAttack(4.f);
	pBullet->SetMaxFrame(4);
	pBullet->SetImage("Note", L"Image/Monster/Banshee/Bullet/BansheeBulletSprite.bmp");
	pBullet->SetBulletType(BT_NOTE);
	pBullet->SetName(m_strName);
	pBullet->SetRangeDist(800.f);
	pBullet->SetInfo(m_tInfo.fx + cosf(fAngle) * m_tInfo.icx, m_tInfo.fy + sinf(fAngle) * m_tInfo.icx, 13, 16);
	pBullet->SetStartPos();
	GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BULLET, pBullet);

}
