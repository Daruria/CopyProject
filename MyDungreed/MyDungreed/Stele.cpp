#include "stdafx.h"
#include "Stele.h"


CStele::CStele():
	m_eSteleType(STELE_END),
	m_ePrevState(STELE_STATE_END),
	m_eState(STELE_STATE_END)
{
}


CStele::~CStele()
{
	Release();
}

void CStele::Initialize()
{
	INSERT_BITMAP("SteleH", L"Image/Monster/Stele/Horizontal/SteleSprite.bmp");
	INSERT_BITMAP("SteleVL", L"Image/Monster/Stele/Vertical/SteleVSpriteL.bmp");
	INSERT_BITMAP("SteleVR", L"Image/Monster/Stele/Vertical/SteleVSpriteR.bmp");

	m_tInfo.icx = 66;
	m_tInfo.icy = 20;
	m_bLife = true;
	m_fRatio = 4.f;
	m_dwFrameMaxTime = 100;
	m_dwFrameTime = GetTickCount();
}

int CStele::Update()
{
	if (m_dwFrameTime + m_dwFrameMaxTime < GetTickCount())
	{
		m_iFrame++;
		
		if (m_iFrame >= m_iMaxFrame)
		{
			switch (m_eState)
			{
			case STELE_CLOSE:
				Idle();
				break;
			case STELE_IDLE:
				m_iFrame = m_iStartFrame;
				break;
			case STELE_OPEN:
				m_bLife = false;
				break;
			case STELE_STATE_END:
				break;
			default:
				break;
			}
		}
		
		m_dwFrameTime = GetTickCount();
	}

	return 0;
}

void CStele::LateUpdate()
{
}

void CStele::Render(HDC hDC)
{
	HDC hMemDC = 0;

	switch (m_eSteleType)
	{
	case STELE_H:
		hMemDC = FIND_BITMAP_DC("SteleH");
		break;
	case STELE_VL:
		hMemDC = FIND_BITMAP_DC("SteleVL");
		break;
	case STELE_VR:
		hMemDC = FIND_BITMAP_DC("SteleVR");
		break;
	case STELE_END:
		break;
	default:
		break;
	}

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

}

void CStele::Release()
{
}

void CStele::StateChange(STELE_STATE eCurType)
{
	if (m_ePrevState == eCurType)
		return;

	m_ePrevState = eCurType;

	switch (eCurType)
	{
	case STELE_CLOSE:
		Close();
		break;
	case STELE_IDLE:
		Idle();
		break;
	case STELE_OPEN:
		Open();
		break;
	case STELE_STATE_END:
		break;
	default:
		break;
	}
}

void CStele::Idle()
{
	m_eState = STELE_IDLE;
	m_iStartFrame = 8;
	m_iFrame = m_iStartFrame;
	m_iMaxFrame = 16;
}

void CStele::Close()
{
	m_eState = STELE_CLOSE;
	m_iStartFrame = 0;
	m_iFrame = m_iStartFrame;
	m_iMaxFrame = 8;
}

void CStele::Open()
{
	m_eState = STELE_OPEN;
	m_iStartFrame = 16;
	m_iFrame = m_iStartFrame;
	m_iMaxFrame = 23;
}

void CStele::SetSteleType(STELE_TYPE eSteleType)
{
	m_eSteleType = eSteleType;

	switch (m_eSteleType)
	{
	case STELE_H:
		m_tInfo.icx = 66;
		m_tInfo.icy = 20;
		break;
	case STELE_VL:
	case STELE_VR:
		m_tInfo.icx = 20;
		m_tInfo.icy = 66;
		break;
	case STELE_END:
		break;
	default:
		break;
	}

	m_tHitBox.icx = static_cast<int>(m_tInfo.icx * m_fRatio);
	m_tHitBox.icy = static_cast<int>(m_tInfo.icy * m_fRatio);
}