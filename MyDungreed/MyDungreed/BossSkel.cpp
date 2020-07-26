#include "stdafx.h"
#include "BossSkel.h"
#include "BossSkelHand.h"
#include "Bullet.h"
#include "Player.h"
#include "BGImage.h"
#include "BossSword.h"
#include "TresureBox.h"

CBossSkel::CBossSkel() :
	m_bStartPattern(false),
	m_bLaser(false),
	m_bSword(false),
	m_bShootHand(false),
	m_fAngle(0.f),
	m_dwPatternTime(0),
	m_dwPatternMaxTime(0),
	m_dwBallTime(0),
	m_dwSwordTime(0),
	m_dwDieTime(0),
	m_iLaserCount(0),
	m_iSwordCount(0),
	m_iDieEffectDist(0),
	m_pRightHand(nullptr),
	m_pLeftHand(nullptr),
	m_pShootingHand(nullptr),
	m_pBackImage(nullptr)
{
}


CBossSkel::~CBossSkel()
{
	Release();
}

void CBossSkel::Initialize()
{
	m_strName = "Belial";

	INSERT_BITMAP("SkellBossIdle", L"Image/Monster/BossSkell/Idle/SkellBossIdle.bmp");

	INSERT_BITMAP("SkellBossAttack", L"Image/Monster/BossSkell/Attack/SkellBossAttack.bmp");

	m_pRightHand = new CBossSkelHand;
	m_pRightHand->Initialize();
	m_pRightHand->SetHandType(HRL_RIGHT);

	m_pLeftHand = new CBossSkelHand;
	m_pLeftHand->Initialize();
	m_pLeftHand->SetHandType(HRL_LEFT);

	m_pShootingHand = m_bShootHand ? m_pRightHand : m_pLeftHand;

	m_pBackImage = new CBGImage;
	m_pBackImage->Initialize();
	m_pBackImage->SetRatio(4.f);
	m_pBackImage->SetMaxFrame(10);
	m_pBackImage->SetImage("BossBack", L"Image/Monster/BossSkell/Back/SkellBossBack.bmp");
	m_pBackImage->SetInfo(50, 50);

	m_fRatio = 4.f;
	m_tInfo.icx = 70;
	m_tInfo.icy = 99;
	m_tHitBox = { 0.f, 0.f, int(m_tInfo.icx * m_fRatio), int(m_tInfo.icy * m_fRatio) };
	m_tAttackBox = {};

	m_iPrevPattern = 0;
	Idle();
	m_dwFrameTime = GetTickCount();
	m_dwFrameMaxTime = 50;
	m_iFrameDir = 1;

	m_tCharInfo.fArmor = 10.f;
	m_tCharInfo.iHPMax = 500;
	m_tCharInfo.iHP = m_tCharInfo.iHPMax;

	m_bStartPattern = false;
}

int CBossSkel::Update()
{
	m_pRightHand->Update();
	m_pLeftHand->Update();
	m_pBackImage->Update();

	if ((m_dwMotion != SBM_DIE )&& (m_dwMotion != SBM_DIE2))
		AddParticle();
	
	CMonster::Update();

	if (!m_bStartPattern)
		return 0;

	DWORD dwCurTime = GetTickCount();

	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

	float fPlayerY = CScrollManager::GetScrollSizeY() * 0.5f;
	if (pPlayer)
		fPlayerY = pPlayer->GetInfo().fy;

	switch (m_dwMotion)
	{
	case SBM_IDLE:
		if (m_dwPatternTime + m_dwPatternMaxTime <= dwCurTime)
		{
			++m_iPrevPattern;
			if (m_iPrevPattern > SBM_SWORD)
				m_iPrevPattern = int(SBM_ATTACK);
			switch (m_iPrevPattern)
			{
			case SBM_ATTACK:
				Attack();
				break;
			case SBM_LASER:
				ShootLaser();
				break;
			case SBM_SWORD:
				SwordAttack();
				break;
			default:
				break;
			}
		}
		break;

	case SBM_LASER:	//	Laser 패턴에 실행 될 코드.
	{
		if (m_iLaserCount < 3)
		{
			if (m_pShootingHand->AttackFinish())
			{
				m_pShootingHand = m_bShootHand ? m_pRightHand : m_pLeftHand;
				m_pShootingHand->SetLaserFinish(false);
				m_pShootingHand->MoveForShoot(fPlayerY);
				m_iLaserCount++;
				m_bShootHand = !m_bShootHand;
			}
		}
		else
		{
			m_iLaserCount = 0;
			Idle();
		}
	}
	break;
	case SBM_SWORD:	//	Sword 패턴에 실행 될 코드.
		if (m_dwSwordTime + 400 < dwCurTime)
		{
			if (m_iSwordCount < 6)
			{
				m_iSwordCount++;
				CBossSword* pSword = new CBossSword;
				pSword->SetInfo(m_tInfo.fx - 400.f + m_iSwordCount * 150.f, m_tInfo.fy - 250.f);
				pSword->Initialize();
				GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BOSS, pSword);
				m_dwSwordTime = dwCurTime;
			}
		}
		if (m_dwPatternTime + m_dwPatternMaxTime <= dwCurTime)
		{
			m_iSwordCount = 0;
			Idle();
		}
		break;
	case SBM_ATTACK:
		if (m_iFrame >= m_iMaxFrame - 1)
			m_iFrame = 3;
		
		if (m_iFrame >= 3)
		{
			if (m_dwBallTime + 100 < dwCurTime)
			{
				ShootBall(m_fAngle);
				GET_SINGLE(CSoundMgr)->PlaySound(L"AKFire.wav", CSoundMgr::EFFECT);
				ShootBall(float(m_fAngle + PI / 2));
				ShootBall(float(m_fAngle + PI));
				ShootBall(float(m_fAngle + PI * 3 / 2));

				m_fAngle += float(PI / 24);
				if (m_fAngle >= float(2 * PI))
					m_fAngle -= float(2 * PI);
				m_dwBallTime = dwCurTime;
			}

		}
		if (m_dwPatternTime + m_dwPatternMaxTime <= dwCurTime)
			Idle();

		break;

	case SBM_DIE:
		if (rand() % 2)
		{
			float fx = (rand() % m_tInfo.icx * m_fRatio * 2.f - m_tInfo.icx * m_fRatio);
			float fy = (rand() % m_tInfo.icy * m_fRatio * 2.f - m_tInfo.icy * m_fRatio);

			GET_SINGLE(CEffectManager)->AddEffect("DieFX", L"Image/Effect/Die/DieFX.bmp", m_tInfo.fx - fx, m_tInfo.fy - fy, 256, 256, 11);
		}

		if (m_dwDieTime + 200 < dwCurTime)
		{
			GET_SINGLE(CSoundMgr)->PlaySound(L"MonsterDie.wav", CSoundMgr::EFFECT);
			m_dwDieTime = dwCurTime;
		}

		if (m_dwPatternTime + m_dwPatternMaxTime <= dwCurTime)
		{
			m_dwMotion = SBM_DIE2;
			m_dwPatternMaxTime = 150;
			m_dwPatternTime = dwCurTime;

			m_iDieEffectDist = 10;
		}
		break;
	case SBM_DIE2:

		CScrollManager::SetScrollX(m_tInfo.fx + 28);
		CScrollManager::SetScrollY(m_tInfo.fy);

		if (m_dwPatternTime + m_dwPatternMaxTime <= dwCurTime)
		{
			if (m_iDieEffectDist > 500)
			{
				m_bLife = false;

				CMonster::AddParticle("SkellBossDeadChin",		L"Image/Monster/BossSkell/Dead/SkellBossDeadChin.bmp",		0.f, 0.f, 4.f, 0.f, 0.5f, 0.f, false);
				CMonster::AddParticle("SkellBossDeadHead",		L"Image/Monster/BossSkell/Dead/SkellBossDeadHead.bmp",		0.f, 0.f, 4.f, 0.f, 0.5f, 0.f, false);
				CMonster::AddParticle("SkellBossDeadParticle0", L"Image/Monster/BossSkell/Dead/SkellBossDeadParticle0.bmp", 1.f, 1.f, 4.f);
				CMonster::AddParticle("SkellBossDeadParticle1", L"Image/Monster/BossSkell/Dead/SkellBossDeadParticle1.bmp", 2.f, 1.f, 4.f);
				CMonster::AddParticle("SkellBossDeadParticle2", L"Image/Monster/BossSkell/Dead/SkellBossDeadParticle2.bmp", -1.f, 1.f, 4.f);
				CMonster::AddParticle("SkellBossDeadParticle3", L"Image/Monster/BossSkell/Dead/SkellBossDeadParticle3.bmp", -2.f, 1.f, 4.f);

				GET_SINGLE(CResult)->AddActivities("Belial Killed");

				for (int i = 0; i < 15; ++i)
					AddGold(100);

				
			}
			else
			{
				//if (m_dwDieTime + 100 < dwCurTime)
				{
					GET_SINGLE(CSoundMgr)->PlaySound(L"MonsterDie.wav", CSoundMgr::EFFECT);
					m_dwDieTime = dwCurTime;
				}

				m_dwPatternTime = dwCurTime;
				m_iDieEffectDist += 75;
				float fAngle = 0.f;
				for (int i = 0; i < 10; ++i)
				{
					fAngle = float(i * PI / 5);
					GET_SINGLE(CEffectManager)->AddEffect("DieFX", L"Image/Effect/Die/DieFX.bmp",
						m_tInfo.fx + 28 + cosf(fAngle) * m_iDieEffectDist,
						m_tInfo.fy + sinf(fAngle) * m_iDieEffectDist, 256, 256, 11);
				}
			}
		}
		break;
	default:
		break;
	}


	return 0;
}

void CBossSkel::LateUpdate()
{
}

void CBossSkel::Render(HDC hDC)
{
	m_pBackImage->Render(hDC);


	HDC hMemDC = MotionDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_tInfo.icx * m_iFrame,
		0,
		m_tInfo.icx,
		m_tInfo.icy,
		RGB(255, 0, 255));

	m_pLeftHand->Render(hDC);
	m_pRightHand->Render(hDC);

#ifdef _DEBUG
	tPos = CScrollManager::WorldToScreen(GetHitBoxWorldInfo());
	CMath::DrawBox(hDC, int(tPos.fx - m_tHitBox.icx / 2), int(tPos.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);
#endif // _DEBUG

}

void CBossSkel::Release()
{
	SAFE_DELETE(m_pRightHand);
	SAFE_DELETE(m_pLeftHand);
	SAFE_DELETE(m_pBackImage);

}

void CBossSkel::Attack()
{
	m_tInfo.icx = 70;
	m_tInfo.icy = 128;
	m_dwMotion = SBM_ATTACK;
	m_iFrame = 0;
	m_iMaxFrame = 10;
	m_dwPatternTime = GetTickCount();
	m_dwPatternMaxTime = 4000;
}

void CBossSkel::Idle()
{
	m_tInfo.icx = 70;
	m_tInfo.icy = 99;
	m_dwMotion = SBM_IDLE;
	m_iFrame = 0;
	m_iMaxFrame = 10;
	m_dwPatternTime = GetTickCount();
	m_dwPatternMaxTime = 2000;
}

void CBossSkel::Hit(const float fAttack, const INFO & tAttackerInfo)
{
	int iDamage = int(fAttack - m_tCharInfo.fArmor);
	m_tCharInfo.iHP -= iDamage;
	POSITION tDamageFontPos = { m_tInfo.fx + rand() % 100 - 25, m_tInfo.fy - rand() % 50 - 25 };

	GET_SINGLE(CEffectManager)->AddFontEffect(tDamageFontPos.fx, tDamageFontPos.fy, tDamageFontPos.fx + 10, tDamageFontPos.fy - 20, 0.5f, 3.f, to_string(iDamage), FT_EDGE);

	GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Monster.wav", CSoundMgr::EFFECT);
}

void CBossSkel::AddAttackEffect(float fx, float fy)
{
}

void CBossSkel::Dying(float fx, float fy)
{
	m_dwMotion = SBM_DIE;
	m_iFrame = 0;
	m_iMaxFrame = 1;
	m_tInfo.icx = 70;
	m_tInfo.icy = 99;
	m_dwPatternTime = GetTickCount();
	m_dwDieTime - GetTickCount();
	m_dwPatternMaxTime = 4000;

	m_pRightHand->Die();
	m_pLeftHand->Die();

	GET_SINGLE(CResult)->MonsterCount();
	GET_SINGLE(CSoundMgr)->StopSound(CSoundMgr::BGM);
}

void CBossSkel::ShootLaser()
{
	m_dwMotion = SBM_LASER;
	m_tInfo.icx = 70;
	m_tInfo.icy = 99;
	m_iFrame = 0;
	m_iMaxFrame = 10;
}

void CBossSkel::SwordAttack()
{
	m_dwMotion = SBM_SWORD;
	m_tInfo.icx = 70;
	m_tInfo.icy = 99;
	m_iFrame = 0;
	m_iMaxFrame = 10;

	m_dwPatternTime = GetTickCount();
	m_dwPatternMaxTime = 6000;

}

void CBossSkel::HandPosition()
{
	m_pLeftHand->SetInfo(m_tInfo.fx - 400.f, m_tInfo.fy);
	m_pRightHand->SetInfo(m_tInfo.fx + 450.f, m_tInfo.fy);
}

void CBossSkel::BackImageSetting()
{
	m_pBackImage->SetInfo(m_tInfo.fx + 30.f, m_tInfo.fy + 100.f);
}

HDC CBossSkel::MotionDC()
{
	switch (m_dwMotion)
	{
	case SBM_IDLE:
	case SBM_LASER:
	case SBM_SWORD:
		return FIND_BITMAP_DC("SkellBossIdle");
	case SBM_ATTACK:
		return FIND_BITMAP_DC("SkellBossAttack");
	case SBM_DIE:
		return FIND_BITMAP_DC("SkellBossIdle");
	case SBM_DIE2:
		return FIND_BITMAP_DC("SkellBossIdle");
	case SBM_END:
		break;
	default:
		break;
	}

	return 0;
}

void CBossSkel::ShootBall(float fAngle)
{
	CBullet* pBullet = new CBullet;
	pBullet->Initialize();
	pBullet->SetAngle(fAngle);
	pBullet->SetRatio(4.0f);
	pBullet->SetSpeed(10.f);
	pBullet->SetAttack(10.f);
	pBullet->SetMaxFrame(2);
	pBullet->SetName(m_strName);
	pBullet->SetImage("BossBall", L"Image/Monster/BossSkell/Bullet/BossBullet.bmp");
	pBullet->SetBulletType(BT_BOSSBALL);
	pBullet->SetRangeDist(1500.f);
	pBullet->SetInfo(m_tInfo.fx + 30.f, m_tInfo.fy + 100.f, 13, 13);
	pBullet->SetStartPos();
	GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BULLET, pBullet);
}

void CBossSkel::AddParticle()
{
	if (rand() % 100 < 20)
	{
		float fAngle = float((rand() % 100) * PI / 50);
		float fSpeed = float((rand() % 100) / 20);

		GET_SINGLE(CEffectManager)->AddEffectToPret("BossParticle", L"Image/Monster/BossSkell/Particle/SkellBossParticle.bmp",
			m_tInfo.fx + 30.f, m_tInfo.fy + 100.f, 30, 30, 8, 100, 4.f, fAngle, fSpeed);
	}
}