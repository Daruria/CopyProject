#include "stdafx.h"
#include "SelectTile.h"


CSelectTile::CSelectTile()
{
}


CSelectTile::~CSelectTile()
{
	Release();
}

const int CSelectTile::SelectID()
{
	POINT pt = ScreenToIndex();
	int iTileX = (m_iTileTotalCX / m_iTileCX);
	int iIndex = pt.x + (pt.y * iTileX);
	if (iIndex < 0) iIndex = 0;
	return iIndex;
}

const POINT CSelectTile::ScreenToIndex()
{
	int iPosX = m_tMousePos.x - (WINCX - m_iTileTotalCX);
	int iPosY = m_tMousePos.y - (WINCY - m_iTileTotalCY);

	iPosX /= m_iTileCX;
	iPosY /= m_iTileCY;

	if (iPosX < 0) iPosX = 0;
	if (iPosY < 0) iPosY = 0;

	if (iPosX >= m_iTileTotalCX / m_iTileCX) iPosX = m_iTileTotalCX / m_iTileCX - 1;
	if (iPosY >= m_iTileTotalCY / m_iTileCY) iPosY = m_iTileTotalCY / m_iTileCY - 1;

	return POINT{static_cast<LONG>(iPosX), static_cast<LONG>(iPosY)};
}

void CSelectTile::Initialize(const string& strKey, const wchar_t* pPath, int iTileCX, int iTileCY, int iTileTotalCX, int iTileTotalCY)
{
	m_strSelectBoxKey = "SelectBox(" + to_string(iTileCX) + ")";
	string strPath = "Image/" + m_strSelectBoxKey + ".bmp";
	wstring wstrPath;
	wstrPath.assign(strPath.begin(), strPath.end());

	INSERT_BITMAP(m_strSelectBoxKey, wstrPath.c_str());
	m_strKey = strKey;
	INSERT_BITMAP(m_strKey, pPath);
	m_iTileCX = iTileCX;
	m_iTileCY = iTileCY;
	m_iTileTotalCX = iTileTotalCX;
	m_iTileTotalCY = iTileTotalCY;
}

int CSelectTile::Update()
{
	GetCursorPos(&m_tMousePos);
	ScreenToClient(g_hWnd, &m_tMousePos);
	
	if (KEYDOWN(VK_LBUTTON))	return SelectID();
	

	return -1;
}

void CSelectTile::LateUpdate()
{
}

void CSelectTile::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_strKey);

	int iStartX = WINCX - m_iTileTotalCX;
	int iStartY = WINCY - m_iTileTotalCY;

	if (hMemDC)
		GdiTransparentBlt(hDC, iStartX, iStartY, m_iTileTotalCX, m_iTileTotalCY, hMemDC, 0, 0, m_iTileTotalCX, m_iTileTotalCY, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, iStartX, iStartY, m_iTileTotalCX, m_iTileTotalCY);
	
	hMemDC = FIND_BITMAP_DC(m_strSelectBoxKey);

	POINT tBoxPoint = ScreenToIndex();
	if (hMemDC)
		GdiTransparentBlt(hDC, tBoxPoint.x * m_iTileCX + iStartX, tBoxPoint.y * m_iTileCY + iStartY, m_iTileCX, m_iTileCY, 
			hMemDC, 0, 0, m_iTileCX, m_iTileCY, RGB(255, 0, 255));
	else
		CAN_NOT_FOUND_BITMAP(hDC, tBoxPoint.x, tBoxPoint.y, m_iTileCX, m_iTileCY);
}

void CSelectTile::Release()
{
}
