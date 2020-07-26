#include "stdafx.h"
#include "TileManager.h"
#include "Tile.h"
#include "ScrollManager.h"

DEFINITION_SINGLE(CTileManager);

CTileManager::CTileManager()
{
}


CTileManager::~CTileManager()
{
	Release();
}

const int CTileManager::GetTileIndex(const INFO & tInfo)
{
	return GetTileIndex(static_cast<int>(tInfo.fx), static_cast<int>(tInfo.fy));
}

const int CTileManager::GetTileIndex(int iX, int iY)
{
	int iIndexX = iX / TILECX;
	int iIndexY = iY / TILECY;

	return iIndexY * TILEX + iIndexX;
}

void CTileManager::SaveData()
{
	HANDLE hFile = CreateFile(__T("Data/Tile.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Fail to Save", L"SaveData()", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	for (auto pTile : m_vecTile)
	{
		int iDrawID = pTile->GetDrawID();
		int iOptionID = pTile->GetOptionID();
		INFO tInfo = pTile->GetInfo();
		WriteFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOptionID, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Success to Save", L"SaveData()", MB_OK);
}

void CTileManager::LoadData()
{
	LoadData(L"Data/Tile.dat");
}

void CTileManager::LoadData(const wchar_t * pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Fail to Read File", L"LoadData()", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	INFO tInfo = {};
	int iDrawID = 0;
	int iOptionID = 0;

	if (!m_vecTile.empty())
	{
		for (size_t i = 0; i < m_vecTile.size(); ++i)
			SAFE_DELETE(m_vecTile[i]);
		m_vecTile.clear();
	}

	int iCount = 0;
	m_vecTile.assign(TILEX * TILEY, nullptr);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOptionID, sizeof(int), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CTile* pTile = new CTile;
		pTile->SetDrawID(iDrawID);
		pTile->SetOptionID(static_cast<TILE_OPTION_ID>(iOptionID));
		pTile->SetInfo(tInfo);

		m_vecTile[iCount] = pTile;
		++iCount;
	}

	CloseHandle(hFile);
}

void CTileManager::Initialize()
{
	INSERT_BITMAP("MapTile", L"Image/MapTile(64).bmp");
	INSERT_BITMAP("MapPickTile", L"Image/MapPickTile.bmp");

	m_vecTile.assign(TILEX * TILEY, nullptr);
	//m_vecTile = vector<CTile*>(TILEX * TILEY);
	
	for (size_t i = 0; i < TILEX * TILEY; ++i)
	{
		float fx = static_cast<float>((i % TILEX) * TILECX + TILECX / 2);
		float fy = static_cast<float>((i / TILEX) * TILECY + TILECY / 2);
		m_vecTile[i] = new CTile;
		m_vecTile[i]->Initialize();
		m_vecTile[i]->SetInfo(fx, fy, TILECX, TILECY);
	}
}

void CTileManager::Render(HDC hDC)
{
	int iRenderStartPosX = static_cast<int>(CScrollManager::GetScreenLeft());
	int iRenderStartPosY = static_cast<int>(CScrollManager::GetScreenTop());

	int iStartIndex = GetTileIndex(iRenderStartPosX, iRenderStartPosY);

	iStartIndex = iStartIndex < 0 ? 0 : iStartIndex;
	
	int iIndexX = iStartIndex % TILEX;
	int iIndexY = iStartIndex / TILEX;

	for (int y = iIndexY; (y <= WINCY / TILECY + iIndexY) && (y < TILEY); ++y)
	{
		for (int x = iIndexX; (x <= WINCX / TILECX + iIndexX) && x < TILEX; ++x)
		{
			int iIndex = y * TILEX + x;
			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

void CTileManager::RenderForEdit(HDC hDC)
{
	int iRenderStartPosX = static_cast<int>(CScrollManager::GetScreenLeft());
	int iRenderStartPosY = static_cast<int>(CScrollManager::GetScreenTop());

	int iStartIndex = GetTileIndex(iRenderStartPosX, iRenderStartPosY);

	iStartIndex = iStartIndex < 0 ? 0 : iStartIndex;

	int iIndexX = iStartIndex % TILEX;
	int iIndexY = iStartIndex / TILEX;

	for (int y = iIndexY; (y <= WINCY / TILECY + iIndexY) && (y < TILEY); ++y)
	{
		for (int x = iIndexX; (x <= WINCX / TILECX + iIndexX) && x < TILEX; ++x)
		{
			int iIndex = y * TILEX + x;
			m_vecTile[iIndex]->RenderForEdit(hDC);
		}
	}
}

void CTileManager::Release()
{
	for (auto pTile : m_vecTile)
	{
		SAFE_DELETE(pTile);
	}
}
