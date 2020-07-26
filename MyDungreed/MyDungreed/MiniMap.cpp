#include "stdafx.h"
#include "MiniMap.h"
#include "Tile.h"


CMiniMap::CMiniMap()
{
}


CMiniMap::~CMiniMap()
{
	Release();
}

void CMiniMap::Initialize()
{	
	INSERT_BITMAP("MiniMapPlayer",	L"Image/MiniMap/MiniMapPlayer.bmp");
	INSERT_BITMAP("MiniMapNPC",		L"Image/MiniMap/MiniMapNPC.bmp");
	INSERT_BITMAP("MiniMapEnemy",	L"Image/MiniMap/MiniMapEnemy.bmp");
	INSERT_BITMAP("MiniMapTresure", L"Image/MiniMap/MiniMapTresure.bmp");
	INSERT_BITMAP("MiniMapTile",	L"Image/MiniMap/MiniMapTile.bmp");
	INSERT_BITMAP("MiniMapClear",	L"Image/MiniMap/MiniMap.bmp");
	INSERT_BITMAP("MiniMap",		L"Image/MiniMap/MiniMap.bmp");

	int iMaxX = CScrollManager::GetScrollSizeX() / TILECX;
	int iMaxY = CScrollManager::GetScrollSizeY() / TILECY;

	m_fRatio = 3.f;
	m_tInfo = 
	{
		WINCX - iMaxX * m_fRatio * 0.5f - 20, 
		iMaxY * m_fRatio * 0.5f + 20, 
		static_cast<int>(iMaxX), 
		static_cast<int>(iMaxY)
	};
	
	CreateMiniMap();
}

int CMiniMap::Update()
{
	return 0;
}

void CMiniMap::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("MiniMap");

	GdiTransparentBlt(hDC,
		static_cast<int>(m_tInfo.fx - m_tInfo.icx * 0.5f * m_fRatio),
		static_cast<int>(m_tInfo.fy - m_tInfo.icy * 0.5f * m_fRatio),
		static_cast<int>(m_tInfo.icx * m_fRatio),
		static_cast<int>(m_tInfo.icy * m_fRatio),
		hMemDC,
		0, 0, m_tInfo.icx, m_tInfo.icy, RGB(255, 0, 255));

	RenderObject(hDC, "MiniMapPlayer",	OT_PLAYER);
	RenderObject(hDC, "MiniMapEnemy",	OT_MONSTER);
	RenderObject(hDC, "MiniMapEnemy",	OT_MONSTER_BOSS);
	RenderObject(hDC, "MiniMapNPC",		OT_NPC);
	RenderObject(hDC, "MiniMapTresure", OT_TRESURE_BOX);
}

void CMiniMap::Release()
{
}

void CMiniMap::CreateMiniMap()
{
	HDC hMiniMap = FIND_BITMAP_DC("MiniMap");
	HDC hMemDC = FIND_BITMAP_DC("MiniMapClear");
	
	SIZE tSize = GET_BITMAP_SIZE("MiniMap");

	BitBlt(hMiniMap, 0, 0, tSize.cx, tSize.cy, hMemDC, 0, 0, SRCCOPY);

	hMemDC = FIND_BITMAP_DC("MiniMapTile");

	vector<CTile*>* pvecTile = GET_SINGLE(CTileManager)->GetpVecTile();

	int iScrollSizeX = CScrollManager::GetScrollSizeX() / TILECX;
	int iScrollSizeY = CScrollManager::GetScrollSizeY() / TILECY;

	for (int i = 0; i < iScrollSizeY; ++i)		//	y
	{
		for (int j = 0; j < iScrollSizeX; ++j)	//	x
		{
			int iIndex = i * TILEX + j;

			if (TO_NONE < (*pvecTile)[iIndex]->GetOptionID() &&
				(*pvecTile)[iIndex]->GetOptionID() < TO_NOUP_RECT)
				GdiTransparentBlt(hMiniMap, j, i, 1, 1, hMemDC, 0, 0, 2, 2, RGB(255, 0, 255));
		}
	}
	
}

void CMiniMap::RenderObject(HDC hDC, const string & strKey, OBJECT_TYPE eType)
{
	HDC hMemDC = FIND_BITMAP_DC(strKey);

	for (auto pObj : *(GET_SINGLE(CObjectManager)->GetObjList(eType)))
	{

		INFO tInfo = pObj->GetInfo();
		POSITION tPos = { tInfo.fx * m_fRatio / (TILECX), tInfo.fy * m_fRatio / (TILECY) };
		SIZE tSize = GET_BITMAP_SIZE(strKey);
		GdiTransparentBlt(hDC,
			static_cast<int>(m_tInfo.fx + tPos.fx - m_tInfo.icx * 0.5f * m_fRatio - tSize.cx * 0.5f),
			static_cast<int>(m_tInfo.fy + tPos.fy - m_tInfo.icy * 0.5f * m_fRatio - tSize.cy * 0.5f),
			tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
	}
}
