#include "stdafx.h"
#include "BossSword.h"
#include "Player.h"

CBossSword::CBossSword():
	m_fAngle(0.f),
	//m_fFloorY(0.f),
	//m_fFloorYDepth(0.f),
	m_tSwordBoundaryMin(POSITION{}),
	m_tSwordBoundaryMax(POSITION{}),
	m_ePrevState(BSS_END),
	m_iCurState(0),
	m_dwStateTime(0),
	m_dwStateMaxTime(0)
{
}


CBossSword::~CBossSword()
{
	Release();
}

void CBossSword::Initialize()
{
	m_strName = "Belial";

	INSERT_BITMAP("BossSwordRect", L"Image/Monster/BossSkell/Sword/BossSwordRect.bmp");
	INSERT_BITMAP("BossSwordFXRect", L"Image/Monster/BossSkell/Sword/BossSwordFXRect.bmp");

	for (int i = 0; i < 8; ++i)
	{
		string strKey = "BossSwordChargeRect" + to_string(i);
		wstring wstrPath = L"Image/Monster/BossSkell/Sword/BossSwordCharge/Rect/BossSwordChargeRect" + to_wstring(i) + L".bmp";
		INSERT_BITMAP(strKey, wstrPath.c_str());
		m_vecChargeKey.push_back(strKey);
	}

	m_fRatio = 4.f;
	m_tInfo.icx = 65;
	m_tInfo.icy = 65;
	m_tHitBox = {};
	m_tAttackBox = { 0.f, 0.f, m_tInfo.icx, m_tInfo.icy };
	m_fSpeed = 50.f;

	m_fAngle = 0.f;

	m_dwFrameTime = GetTickCount();
	m_dwFrameMaxTime = 50;
	m_iFrameDir = 1;

	m_iCurState = 0;
	m_bLife = true;
	m_bPassible = true;

	m_bAttack = true;
	m_tCharInfo.fAttack = 13.f;
	m_sAttackLife = 0;
	m_sAttackBoxLifeTime = 7;

	GET_SINGLE(CEffectManager)->AddEffect("IcicleDestroyFX", L"Image/Monster/BossSkell/Sword/SwordDisappear/IcicleDestroyFX.bmp",
		m_tInfo.fx, m_tInfo.fy, 35, 48, 3, 4.f);

	m_tSwordBoundaryMin = {300.f, 390.f};
	m_tSwordBoundaryMax = {1510.f,1280.f};

	GET_SINGLE(CSoundMgr)->PlaySound(L"MonsterDie.wav", CSoundMgr::EFFECT);
}

int CBossSword::Update()
{
	ChangeState();

	DWORD dwCurTime = GetTickCount();
	if (m_iCurState != BSS_SHOOT && m_dwStateTime + m_dwStateMaxTime < dwCurTime)
	{
		++m_iCurState;
		if (m_iCurState == BSS_SHOOT)
		{
			m_bAttack = true;
			m_tCharInfo.fAttack = 13.f;
			m_sAttackLife = 0;
			m_sAttackBoxLifeTime = 7;

			int iCX = 260;
			int iCY = 84;

			m_tAttackBox.icx = (int)fabs(260 * cosf(m_fAngle)) + (int)fabs(iCY * sinf(m_fAngle));
			m_tAttackBox.icy = (int)fabs(260 * sinf(m_fAngle)) + (int)fabs(iCY * cosf(m_fAngle));
		}
		if (m_iCurState >= BSS_END)
		{
			m_bLife = false;
			GET_SINGLE(CEffectManager)->AddRotateEffect("IcicleDestroyFX", L"Image/Monster/BossSkell/Sword/SwordDisappear/IcicleDestroyFX",
				m_tInfo.fx, m_tInfo.fy, 48, 48, 3, m_fAngle, 4.f);

			GET_SINGLE(CSoundMgr)->PlaySound(L"MonsterDie.wav", CSoundMgr::EFFECT);
		}
	}
	
	CPlayer* pPlayer = GET_SINGLE(CObjectManager)->GetPlayer();
	if (!pPlayer)
		return 0;

	switch (m_iCurState)
	{
	case BSS_CHARGE:
	{
		float fdx = pPlayer->GetInfo().fx - m_tInfo.fx;
		float fdy = pPlayer->GetInfo().fy - m_tInfo.fy;

		float fDist = sqrtf(fdx * fdx + fdy * fdy);

		m_fAngle = fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);
	}
		break;
	case BSS_SHOOT:
		m_tAttackBox.fx = m_tInfo.fx;
		m_tAttackBox.fy = m_tInfo.fy;

		if (m_tInfo.fx <= m_tSwordBoundaryMin.fx)
		{
			m_iCurState = (int)BSS_STOP;
			GET_SINGLE(CEffectManager)->AddEffect("BossSwordHitFXL", L"Image/Monster/BossSkell/Sword/BossSwordHitFX/BossSwordHitFXL.bmp",
				256.f + 44.f, m_tInfo.fy, 22, 51, 5, 4.f);
			GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Player.wav", CSoundMgr::EFFECT);
		}
		else if (m_tInfo.fy <= m_tSwordBoundaryMin.fy)
		{
			m_iCurState = (int)BSS_STOP;
			GET_SINGLE(CEffectManager)->AddEffect("BossSwordHitFXU", L"Image/Monster/BossSkell/Sword/BossSwordHitFX/BossSwordHitFXU.bmp",
				m_tInfo.fx, 384.f + 44.f, 51, 22, 5, 4.f);
			GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Player.wav", CSoundMgr::EFFECT);
		}
		else if (m_tInfo.fx >= m_tSwordBoundaryMax.fx)
		{
			m_iCurState = (int)BSS_STOP;
			GET_SINGLE(CEffectManager)->AddEffect("BossSwordHitFXR", L"Image/Monster/BossSkell/Sword/BossSwordHitFX/BossSwordHitFXR.bmp",
				1536.f - 44.f, m_tInfo.fy, 22, 51, 5, 4.f);
			GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Player.wav", CSoundMgr::EFFECT);
		}
		else if (m_tInfo.fy >= m_tSwordBoundaryMax.fy)
		{
			m_iCurState = (int)BSS_STOP;
			GET_SINGLE(CEffectManager)->AddEffect("BossSwordHitFX", L"Image/Monster/BossSkell/Sword/BossSwordHitFX/BossSwordHitFX.bmp",
				m_tInfo.fx, 1344.f - 44.f, 51, 22, 5, 4.f);
			GET_SINGLE(CSoundMgr)->PlaySound(L"Hit_Player.wav", CSoundMgr::EFFECT);
		}
		else
		{
			m_tInfo.fx += m_fSpeed * cosf(m_fAngle);
			m_tInfo.fy += m_fSpeed * sinf(m_fAngle);
		}

		break;
	case BSS_STOP:
		
		break;
	case BSS_END:
		break;
	default:
		break;
	}

	CMonster::Update();

	return 0;
}

void CBossSword::LateUpdate()
{
	CMonster::LateUpdate();
}

void CBossSword::Render(HDC hDC)
{
	HDC hMemDC = StateDC();

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);


	int	iRenderStartX = int(m_tInfo.fx - m_tInfo.icx * m_fRatio * 0.5f);
	int iRenderStartY = int(m_tInfo.fy - m_tInfo.icy * m_fRatio * 0.5f);
	
	int	iRenderEndX = int(iRenderStartX + m_tInfo.icx * m_fRatio);
	int iRenderEndY = int(iRenderStartY + m_tInfo.icy * m_fRatio);

	int iLeftDefference = 0;
	int iRightDefference = 0;
	int iUpDefference = 0;
	int iDownDefference = 0;

	if ((float)iRenderStartX <= 256.f)
	{
		iRenderStartX = 256;
		iLeftDefference = 256 - int(m_tInfo.fx - m_tInfo.icy * m_fRatio  * 0.5f);
	}

	if ((float)iRenderStartY <= 384.f)
	{
		iRenderStartY = 384;
		iUpDefference = 384 - int(m_tInfo.fy - m_tInfo.icy * m_fRatio  * 0.5f);
	}

	if ((float)iRenderEndX >= 1536.f)
		iRightDefference = iRenderEndX - 1536;
	

	if ((float)iRenderEndY >= 1344.f)
		iDownDefference = iRenderEndY - 1344;
	

	int iSrcStartX = int(iLeftDefference * 0.25f);
	int iSrcStartY = int(iUpDefference * 0.25f);

	BOOL bRender = GdiTransparentBlt(hDC,
		int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f + iLeftDefference),
		int(tPos.fy - m_tInfo.icy * m_fRatio  * 0.5f + iUpDefference),
		int(m_tInfo.icx * m_fRatio - iRightDefference - iLeftDefference),
		int(m_tInfo.icy * m_fRatio - iDownDefference - iUpDefference),
		hMemDC,
		iSrcStartX,
		iSrcStartY,
		m_tInfo.icx - (iRightDefference / 4) -(iSrcStartX),
		m_tInfo.icy - (iDownDefference / 4) - (iSrcStartY),
		RGB(255, 0, 255));

	if (!bRender)
		int iA = 0;

	if (m_iCurState == BSS_CHARGE)
		ChargeRender(hDC);

#ifdef _DEBUG
	if (m_bAttack)
	{
		tPos = CScrollManager::WorldToScreen(m_tAttackBox);
		CMath::DrawBox(hDC,
			int(tPos.fx - m_tAttackBox.icx / 2),
			int(tPos.fy - m_tAttackBox.icy / 2), m_tAttackBox.icx, m_tAttackBox.icy);
	}
	int iA = 0;
#endif // _DEBUG

}

void CBossSword::Release()
{
}

void CBossSword::ChargeRender(HDC hDC)
{
	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	//for (size_t i = 0; i < m_vecChargeKey.size(); ++i)
	{
		HDC hMemDC = FIND_ROTATED_DC(m_vecChargeKey[m_iFrame], m_fAngle);
		GdiTransparentBlt(hDC,
			int(tPos.fx - m_tInfo.icx * m_fRatio * 0.5f),
			int(tPos.fy - m_tInfo.icy * m_fRatio * 0.5f),
			int(m_tInfo.icx * m_fRatio),
			int(m_tInfo.icy * m_fRatio),
			hMemDC,
			0,
			0,
			m_tInfo.icx,
			m_tInfo.icy,
			RGB(255, 0, 255));
	}
}

void CBossSword::ChangeState()
{
	if (m_ePrevState == m_iCurState)
		return;

	m_ePrevState = (BOSS_SWORD_STATE)m_iCurState;

	switch (m_iCurState)
	{
	case BSS_CHARGE:
		m_iFrame = 0;
		m_iMaxFrame = 8;
		m_dwStateTime = GetTickCount();
		m_dwStateMaxTime = 2000;
		break;
	case BSS_SHOOT:
		m_tInfo.icx = 70;
		m_tInfo.icy = 70;
		break;
	case BSS_STOP:
		m_tInfo.icx = 65;
		m_tInfo.icy = 65;
		m_dwStateTime = GetTickCount();
		m_dwStateMaxTime = 2000;
	//	GET_SINGLE(CEffectManager)->AddEffect();
		break;
	case BSS_END:
		break;
	default:
		break;
	}
}

HDC CBossSword::StateDC()
{
	switch (m_iCurState)
	{
	case BSS_CHARGE:
		return FIND_ROTATED_DC("BossSwordRect", m_fAngle);
		//return FIND_BITMAP_DC("BossSwordRect");
	case BSS_SHOOT:
		return FIND_ROTATED_DC("BossSwordFXRect", m_fAngle);
	case BSS_STOP:
		return FIND_ROTATED_DC("BossSwordRect", m_fAngle);
		//return FIND_BITMAP_DC("BossSwordRect");
	case BSS_END:
		break;
	default:
		break;
	}
	return 0;
}
