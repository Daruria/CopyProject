#include "stdafx.h"
#include "Thorn.h"


CThorn::CThorn() :
	m_fAttack(0.f)
{
}


CThorn::~CThorn()
{
	Release();
}

void CThorn::Initialize()
{
	m_tInfo.icx = 64;
	m_tInfo.icy = 64;
	m_fRatio = 1.f;
	INSERT_BITMAP("Thorn", L"Image/Dungeon/Trap.bmp");
}

int CThorn::Update()
{
	return 0;
}

void CThorn::LateUpdate()
{
}

void CThorn::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("Thorn");

	POSITION tPos = CScrollManager::WorldToScreen(m_tInfo);

	int iRenderStartX = static_cast<int>(tPos.fx - m_iThornCount * m_tInfo.icx * 0.5f);
	
	for (int i = 0; i < m_iThornCount; ++i)
	{
		GdiTransparentBlt(hDC,
			iRenderStartX + m_tInfo.icx * i,
			static_cast<int>(tPos.fy - m_tInfo.icy * 0.5f),
			m_tInfo.icx,
			m_tInfo.icy,
			hMemDC,
			0,
			0,
			m_tInfo.icx,
			m_tInfo.icy,
			RGB(255, 0, 255));
	}

#ifdef _DEBUG
	CMath::DrawBox(hDC, 
		static_cast<int>(tPos.fx + m_tHitBox.fx - m_tHitBox.icx * 0.5f),
		static_cast<int>(tPos.fy + m_tHitBox.fy - m_tHitBox.icy * 0.5f),
		m_tHitBox.icx, 
		m_tHitBox.icy);
#endif // _DEBUG

}

void CThorn::Release()
{
}

void CThorn::SetThornCount(int iThornCount)
{
	m_iThornCount = iThornCount;

	m_tHitBox = { 0.f, m_tInfo.icx / 4.f, m_tInfo.icx * iThornCount, m_tInfo.icy / 2 };
}
