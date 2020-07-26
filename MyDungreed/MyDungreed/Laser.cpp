#include "stdafx.h"
#include "Laser.h"


CLaser::CLaser():
	m_iLaserLength(0)
{
}


CLaser::~CLaser()
{
	Release();
}

void CLaser::Initialize()
{
	m_strName = "Belial";

	INSERT_BITMAP("LaserHeadLeftHand", L"Image/Monster/BossSkell/LaserHead/SkellBossLaserHeadLeftHand.bmp");
	INSERT_BITMAP("LaserHeadRightHand", L"Image/Monster/BossSkell/LaserHead/SkellBossLaserHeadRightHand.bmp");

	INSERT_BITMAP("LaserBody", L"Image/Monster/BossSkell/LaserBody/SkellBossLaserBody.bmp");;
	
	m_fRatio = 4.f;
	m_tInfo.icx = 32;
	m_tInfo.icy = 55;

	m_tCharInfo.fAttack = 15.f;
	m_tHitBox = {};
	m_iLaserLength = 10;
	m_bAttack = true;

	m_tAttackBox = { 0.f, 0.f, int(m_tInfo.icx * m_fRatio * 10), int((m_tInfo.icy - 15) * m_fRatio) };
	m_sAttackLife = 0;
	m_sAttackBoxLifeTime = 7;


	m_iFrame = 0;
	m_iFrameDir = 1;
	m_iMaxFrame = 7;
	m_dwFrameMaxTime = 50;
	m_dwFrameTime = GetTickCount();

	GET_SINGLE(CSoundMgr)->PlaySound(L"iceball.wav", CSoundMgr::EFFECT);
}

int CLaser::Update()
{
	CMonster::Update();
	//m_bAttack = true;
	if (m_iFrame >= m_iMaxFrame - 1)
		m_bLife = false;

	m_tAttackBox.fy = m_tInfo.fy;
	return 0;
}

void CLaser::LateUpdate()
{
}

void CLaser::Render(HDC hDC)
{
	HDC hMemDC = m_bDir ? FIND_BITMAP_DC("LaserHeadRightHand") : FIND_BITMAP_DC("LaserHeadLeftHand");

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	BOOL bRender = GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
		int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
		int(m_tInfo.icx * m_fRatio),
		int(m_tInfo.icy * m_fRatio),
		hMemDC,
		m_iFrame * m_tInfo.icx,
		0,
		m_tInfo.icx,
		m_tInfo.icy,
		RGB(255, 0, 255));
	int iA = 0;

	hMemDC = FIND_BITMAP_DC("LaserBody");

	int iDir = m_bDir ? -1 : 1;	//	레이저 진행 방향. 오른손이면 왼쪽으로
	for (int i = 1; i < m_iLaserLength; ++i)
	{
		GdiTransparentBlt(hDC,
			int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f + iDir * (m_tInfo.icx * m_fRatio) * i),
			int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
			int(m_tInfo.icx * m_fRatio),
			int(m_tInfo.icy * m_fRatio),
			hMemDC,
			m_iFrame * m_tInfo.icx,
			0,
			m_tInfo.icx,
			m_tInfo.icy,
			RGB(255, 0, 255));
	}

#ifdef _DEBUG
	if (m_bAttack)
	{
		tPos = CScrollManager::WorldToScreen(m_tAttackBox);
		CMath::DrawBox(hDC,
			int(tPos.fx - m_tAttackBox.icx / 2),
			int(tPos.fy - m_tAttackBox.icy / 2), m_tAttackBox.icx, m_tAttackBox.icy);
	}
	iA = 0;
#endif // _DEBUG

}

void CLaser::Release()
{
}

void CLaser::AddAttackEffect(float fx, float fy)
{

}

void CLaser::SetRightHand(float fx)
{
	m_bDir = true;
	m_tAttackBox.fx = fx - m_tAttackBox.icx * 0.5f;

}

void CLaser::SetLeftHand(float fx)
{
	m_bDir = false;
	m_tAttackBox.fx = fx + m_tAttackBox.icx * 0.5f;
}
