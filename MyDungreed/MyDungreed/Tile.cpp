#include "stdafx.h"
#include "Tile.h"
#include "BitmapManager.h"
#include "ScrollManager.h"

CTile::CTile():m_iDrawID(96), m_iOptionID(0)
{
}


CTile::~CTile()
{
	Release();
}

void CTile::RenderForEdit(HDC hDC)
{
	Render(hDC);

	HDC hMemDC = GET_SINGLE(CBitmapManager)->FindBitmapDC("OptionMapTile");

	int iDrawPosX = (m_iOptionID % TILEX) * TILECX;
	int iDrawPosY = (m_iOptionID / TILEY) * TILECY;

	POSITION tScreePos = CScrollManager::WorldToScreen(m_tInfo);
	if (hMemDC)
		GdiTransparentBlt(hDC,
			static_cast<int>(tScreePos.fx - m_tInfo.icx / 2), 
			static_cast<int>(tScreePos.fy - m_tInfo.icy / 2),
			m_tInfo.icx, m_tInfo.icy, hMemDC,
			iDrawPosX, iDrawPosY, TILECX, TILECY, RGB(255, 0, 255));
}

void CTile::Initialize()
{
	m_tInfo.icx = TILECX;
	m_tInfo.icy = TILECY;
}

int CTile::Update()
{
	return 0;
}

void CTile::LateUpdate()
{
}

void CTile::Release()
{
}

void CTile::Render(HDC hDC)
{
	HDC hMemDC = GET_SINGLE(CBitmapManager)->FindBitmapDC("MapTile");
	
	int iDrawPosX = (m_iDrawID % TILEMAPX) * TILECX;
	int iDrawPosY = (m_iDrawID / TILEMAPY) * TILECY;

	POSITION tScreePos = CScrollManager::WorldToScreen(m_tInfo);
	if (hMemDC)
		GdiTransparentBlt(hDC,
			static_cast<int>(tScreePos.fx - m_tInfo.icx / 2), 
			static_cast<int>(tScreePos.fy - m_tInfo.icy / 2),
			m_tInfo.icx, m_tInfo.icy, hMemDC,
			iDrawPosX, iDrawPosY, TILECX, TILECY, RGB(255, 0, 255));
}
