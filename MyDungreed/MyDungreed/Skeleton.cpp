#include "stdafx.h"
#include "Skeleton.h"
#include "Player.h"
#include "Bullet.h"

CSkeleton::CSkeleton():
	m_bCanAttack(true),
	//m_fRatio(0.f),
	m_fAngle(0.f),
	m_eSkelType(ST_SHORT),
	m_tWeaponOffSet(INFO{})
{
}


CSkeleton::~CSkeleton()
{
	Release();
}

void CSkeleton::Initialize()
{
	m_strName = "Skeleton";

	INSERT_BITMAP("SkelIdleR", L"Image/Monster/Skel/Idle/SkelIdleR.bmp");
	INSERT_BITMAP("SkelIdleL", L"Image/Monster/Skel/Idle/SkelIdleL.bmp");

	INSERT_BITMAP("SkelWalkSpriteR", L"Image/Monster/Skel/Walk/SkelWalkSpriteR.bmp");
	INSERT_BITMAP("SkelWalkSpriteL", L"Image/Monster/Skel/Walk/SkelWalkSpriteL.bmp");

	INSERT_BITMAP("SkelHand", L"Image/Monster/Skel/Hand/SkelHand.bmp");

	CMonster::Initialize();

	Idle();
	m_iFrame = 0;
	m_iFrameDir = 1;
	m_dwFrameMaxTime = 100;

	m_bFalling = false;
	m_bJump = false;
	m_bPassible = false;
	m_bPhysics = true;
	m_fSpeed = 3.f;
	m_fJumpForce = 15.f;

	m_bCanAttack = true;
	m_bAttack = false;
	m_fRatio = 4.f;
	m_fAngle = 0.f;
	m_tInfo.icx = 32;
	m_tInfo.icy = 32;
	m_tHitBox = { 0, 8 * m_fRatio, int(9 * m_fRatio), int(16 * m_fRatio) };

	m_bLife = true;
	m_tCharInfo.iHPMax = 50;
	m_tCharInfo.iHP = m_tCharInfo.iHPMax;
	m_tCharInfo.fAttack = 7.f;
	m_tCharInfo.fArmor = 3.f;
}

int CSkeleton::Update()
{

	AI();

	CMonster::Update();

	if (m_bPhysics) Gravity();


	return 0;
}

void CSkeleton::LateUpdate()
{
	CMonster::LateUpdate();
}

void CSkeleton::Render(HDC hDC)
{
	HDC hMemDC = MotionDC();
	
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	bool bIsAttackMotion = (m_dwMotion == SM_ATTACK);

	GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx * !bIsAttackMotion, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	hMemDC = WeaponDC();

	int iDir = m_bDir ? 1 : -1;
	if (m_eSkelType == ST_BOW)
	{
		iDir = 1;
		bIsAttackMotion = false;
	}
	float fWeaponX = tPos.fx - m_tWeaponOffSet.icx * m_fRatio * 0.5f + iDir * m_tWeaponOffSet.fx;
	float fWeaponY = tPos.fy - m_tWeaponOffSet.icy * m_fRatio * 0.5f + m_tWeaponOffSet.fy;

	if (m_eSkelType == ST_BOW)
	{
		fWeaponX += cosf(m_fAngle) * 25.f;
		fWeaponY += sinf(m_fAngle) * 25.f;
	}

	BOOL bRender = GdiTransparentBlt(hDC,
		int(fWeaponX),
		int(fWeaponY), 
		int(m_tWeaponOffSet.icx * m_fRatio), 
		int(m_tWeaponOffSet.icy * m_fRatio),
		hMemDC, 
		m_iFrame * m_tWeaponOffSet.icx * bIsAttackMotion, 
		0,
		m_tWeaponOffSet.icx, 
		m_tWeaponOffSet.icy, RGB(255, 0, 255));

	CMonster::Render(hDC);

#ifdef _DEBUG
	tPos = CScrollManager::WorldToScreen(m_tInfo);
	CMath::DrawBox(hDC, static_cast<int>(tPos.fx + m_tHitBox.fx - m_tHitBox.icx / 2),
		static_cast<int>(tPos.fy + m_tHitBox.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);
	if (m_bAttack)
	{
		tPos = CScrollManager::WorldToScreen(m_tAttackBox);
		CMath::DrawBox(hDC, static_cast<int>(tPos.fx - m_tAttackBox.icx / 2),
			static_cast<int>(tPos.fy - m_tAttackBox.icy / 2), m_tAttackBox.icx, m_tAttackBox.icy);
	}
#endif // DEBUG
}

void CSkeleton::Release()
{
}

void CSkeleton::Attack()
{
	if (m_dwMotion != SM_ATTACK)
		m_iFrame = 0;

	m_dwMotion = SM_ATTACK;

	switch (m_eSkelType)
	{
	case ST_SHORT:
		m_iMaxFrame = 12;
		m_sAttackBoxLifeTime = 12;
		m_tAttackBox.icx = static_cast<int>(32 * m_fRatio);
		m_tAttackBox.icy = static_cast<int>(32 * m_fRatio);
		break;
	case ST_GREAT:
		m_iMaxFrame = 15;
		m_sAttackBoxLifeTime = 15;
		m_tAttackBox.icx = static_cast<int>(51 * m_fRatio);
		m_tAttackBox.icy = static_cast<int>(49 * m_fRatio);
		break;
	case ST_BOW:
		m_iMaxFrame = 6;
		m_sAttackBoxLifeTime = 6;
		break;
	case ST_END:
		break;
	default:
		break;
	}
	m_dwAttackCool = GetTickCount();
	int iDir = m_bDir ? 1 : -1;
	m_tAttackBox.fx = m_tInfo.fx + m_tWeaponOffSet.fx * iDir;
	m_tAttackBox.fy = m_tInfo.fy + m_tWeaponOffSet.fy;
}

void CSkeleton::Idle()
{
	m_dwMotion = SM_IDLE;
	m_iFrame = 0;
	m_iMaxFrame = 1;
}

void CSkeleton::Move()
{
	m_dwMotion = SM_WALK;
	//m_iFrame = 0;
	m_iMaxFrame = 6;
}

void CSkeleton::Die()
{
	
}

void CSkeleton::Hit(const float fAttack, const INFO & tAttackerInfo)
{
	CMonster::Hit(fAttack, tAttackerInfo);
}

void CSkeleton::AddAttackEffect(float fx, float fy)
{
	float m_fSlashAngle = static_cast<float>((rand() % 360 - 180) * (180 / PI));
	GET_SINGLE(CEffectManager)->AddRotateEffect("Slash", L"Image/Effect/Slash/SlashFX", fx, fy, 168, 168, 4, m_fSlashAngle, 1.f);
}

void CSkeleton::Dying(float fx, float fy)
{
	CMonster::Dying(fx, fy);

	float fDirX = m_tInfo.fx - fx;
	float fDirY = m_tInfo.fy - fy + 10.f;

	float fDist = sqrtf(fDirX * fDirX + fDirY * fDirY);
	fDirX /= fDist;
	fDirY /= fDist;

	m_bLife = false;
	m_dwMotion = SM_DIE;
	m_iFrame = 0;
	m_iMaxFrame = 1;

	AddRotateParticle("SkelParticleHead", L"Image/Monster/Skel/Particle/SkelParticleHead.bmp", fDirX, fDirY);

	int iGolds = rand() % 5;
	for (int i = 0; i < iGolds; ++i)
		AddGold(10);

	m_dwAttackCool = GetTickCount();
}

HDC CSkeleton::MotionDC()
{
	switch (m_dwMotion)
	{
	case SM_IDLE:
		return m_bDir ? FIND_BITMAP_DC("SkelIdleR") : FIND_BITMAP_DC("SkelIdleL");
	case SM_WALK:
		return m_bDir ? FIND_BITMAP_DC("SkelWalkSpriteR") : FIND_BITMAP_DC("SkelWalkSpriteL");
	case SM_ATTACK:
		return m_bDir ? FIND_BITMAP_DC("SkelIdleR") : FIND_BITMAP_DC("SkelIdleL");
		break;
	case SM_END:
		return 0;
	default:
		return 0;
	}
	return 0;
}

HDC CSkeleton::WeaponDC()
{
	switch (m_eSkelType)
	{
	case ST_SHORT:
		return m_bDir ? FIND_BITMAP_DC("RustyShortSwordSpriteR") : FIND_BITMAP_DC("RustyShortSwordSpriteL");
	case ST_GREAT:
		return m_bDir ? FIND_BITMAP_DC("RustyGreatSwordSpriteR") : FIND_BITMAP_DC("RustyGreatSwordSpriteL");
	case ST_BOW:
		if (m_iFrame >= 6)
			return FIND_ROTATED_DC(m_vecFrameKey[5], m_fAngle);
		
		return FIND_ROTATED_DC(m_vecFrameKey[m_iFrame], m_fAngle);
	case ST_END:
		break;
	default:
		break;
	}

	return 0;
}

void CSkeleton::AI()
{
	if (m_iFrame == m_iMaxFrame - 1)
	{
		CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();

		INFO tInfo = pPlayer->GetInfo();

		float fdx = tInfo.fx - m_tInfo.fx;
		float fdy = tInfo.fy - m_tInfo.fy;

		float fDist = sqrtf(fdx * fdx + fdy * fdy);

		m_bDir = fdx > 0;
		switch (m_eSkelType)
		{
		case ST_SHORT:
			if (100 < fDist && fDist < 400.f)
				Move();
			else if (fDist <= 100 && (m_dwAttackCool + 2000 <= GetTickCount()))
				Attack();
			else
				Idle();
			break;
		case ST_GREAT:
			if (100.f < fDist && fDist < 300.f)
				Move();
			else if (fDist <= 100.f && (m_dwAttackCool + 2000 <= GetTickCount()))
				Attack();
			else
				Idle();
			break;
		case ST_BOW:
			m_fAngle = fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);
			if (fDist <= 500 && (m_dwAttackCool + 2000 <= GetTickCount()))
				Attack();
			else
				Idle();
			break;
		case ST_END:
			break;
		default:
			break;
		}
	}

	if (m_dwMotion == SM_ATTACK)
	{
		switch (m_eSkelType)
		{
		case ST_SHORT:
			if (m_iFrame == 7 && m_bCanAttack)
			{
				m_bAttack = true;
				m_bCanAttack = false;
			}

			if (m_iFrame == 8)
				m_bCanAttack = true;
			break;
		case ST_GREAT:
			if (m_iFrame == 6 && m_bCanAttack)
			{
				m_bAttack = true;
				m_bCanAttack = false;
			}

			if (m_iFrame == 7)
				m_bCanAttack = true;
			break;
		case ST_BOW:
			if (m_iFrame == 4 && m_bCanAttack)
			{
				m_bAttack = true;
				m_bCanAttack = false;
				CBullet* pBullet = new CBullet;
				pBullet->Initialize();
				pBullet->SetAngle(m_fAngle);
				pBullet->SetMaxFrame(1);
				pBullet->SetRatio(4.0f);
				pBullet->SetSpeed(10.f);
				pBullet->SetAttack(2.f);
				pBullet->SetImage("Arrow", L"Image/Monster/Skel/Arrow/Arrow.bmp");
				pBullet->SetName(m_strName);
				pBullet->SetBulletType(BT_ARROW);
				pBullet->SetRangeDist(500.f);
				pBullet->SetInfo(m_tInfo.fx + m_tWeaponOffSet.fx, m_tInfo.fy + m_tWeaponOffSet.fy, 13, 13);
				pBullet->SetStartPos();
				GET_SINGLE(CObjectManager)->AddObj(OT_MONSTER_BULLET, pBullet);
			}

			else if (m_iFrame == 0)
				m_bCanAttack = true;
			break;
		case ST_END:
			break;
		default:
			break;
		}
	}

	if (m_dwMotion == SM_WALK)
		m_tInfo.fx += m_bDir ? m_fSpeed : -m_fSpeed;


}

void CSkeleton::SetSkelType(SKEL_TYPE eSkelType)
{
	m_eSkelType = eSkelType;
	switch (m_eSkelType)
	{
	case ST_SHORT:
		INSERT_BITMAP("RustyShortSwordSpriteR", L"Image/Monster/Skel/RustyShortSword/RustyShortSwordSpriteR.bmp");
		INSERT_BITMAP("RustyShortSwordSpriteL", L"Image/Monster/Skel/RustyShortSword/RustyShortSwordSpriteL.bmp");
		m_tAttackBox = { 40.f, 30.f, 32, 32 };
		m_tWeaponOffSet = { 40.f, 30.f, 32, 32};
		break;
	case ST_GREAT:
		INSERT_BITMAP("RustyGreatSwordSpriteR", L"Image/Monster/Skel/RustyGreatSword/RustyGreatSwordSpriteR.bmp");
		INSERT_BITMAP("RustyGreatSwordSpriteL", L"Image/Monster/Skel/RustyGreatSword/RustyGreatSwordSpriteL.bmp");
		m_tAttackBox = { 40.f, 30.f, 51, 49 };
		m_tWeaponOffSet = { 40.f, 30.f, 51, 49};
		break;
	case ST_BOW:
		m_tAttackBox = {};
		m_tWeaponOffSet = { 0.f, 30.f, 17 , 17 };
		for (int i = 0; i < 6; ++i)
		{
			string FrameKey = "OakBow" + to_string(i);
			wstring wstrPath = L"Image/Monster/Skel/OakBow/OakBow" + to_wstring(i) + L".bmp";
			INSERT_BITMAP(FrameKey, wstrPath.c_str());
			m_vecFrameKey.push_back(FrameKey);
		}
		break;
	case ST_END:
		break;
	default:
		break;
	}
}
