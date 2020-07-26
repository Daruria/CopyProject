#include "stdafx.h"
#include "MyBitmap.h"


CMyBitmap::CMyBitmap()
{
}

CMyBitmap::~CMyBitmap()
{
	Release();
}

void CMyBitmap::Initialize()
{
	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	m_hRotatedDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);
}

void CMyBitmap::Release()
{
	DeleteObject(SelectObject(m_hMemDC, m_hOldBitmap));
	DeleteDC(m_hMemDC);

	DeleteObject(SelectObject(m_hRotatedDC, m_hOldRotatedBitmap));
	DeleteDC(m_hRotatedDC);
}

HBITMAP CMyBitmap::GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, int dest_width, int dest_height, float angle)
{
	HDC sourceDC = CreateCompatibleDC(hdc); 
	HDC destDC = CreateCompatibleDC(hdc);
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, dest_width, dest_height); 

	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult); 

	HBRUSH hbrBack = CreateSolidBrush(0xff00ff);
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);
	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY); 
	DeleteObject(SelectObject(destDC, hbrOld)); 

	float cosine = (float)cos(angle);
	float sine = (float)sin(angle); 

	SetGraphicsMode(destDC, GM_ADVANCED); 

	XFORM xform; 
	xform.eM11 = cosine; 
	xform.eM12 = sine; 
	xform.eM21 = -sine; 
	xform.eM22 = cosine; 
	xform.eDx = (FLOAT)dest_width / 2.0f; 
	xform.eDy = (FLOAT)dest_height / 2.0f; 

	SetWorldTransform(destDC, &xform);

	BitBlt(destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height, sourceDC, 0, 0, SRCCOPY);

	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);
	DeleteObject(sourceDC);
	DeleteObject(destDC);

	return hbmResult;
}

void CMyBitmap::SetImage(const wchar_t * pPath)
{
	HDC hDC = GetDC(g_hWnd);
	m_hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), pPath, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	if (!m_hTempBitmap)
	{
		m_hTempBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), pPath, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		BITMAP bm;
		GetObject(m_hTempBitmap, sizeof(bm), &bm);
		m_hRotatedBitmap = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
		m_hOldRotatedBitmap = (HBITMAP)SelectObject(m_hRotatedDC, m_hRotatedBitmap);
	}
	ReleaseDC(g_hWnd, hDC);
}

void CMyBitmap::RotateDC(const float fRadians)
{
	HDC hdc = GetDC(g_hWnd);

	BITMAP bm;
	GetObject(m_hTempBitmap, sizeof(bm), &bm);

	int iNewWidth = max(bm.bmWidth, bm.bmHeight);
	int iNewHeight = max(bm.bmWidth, bm.bmHeight);

	static HDC hTempMemDC; 
	HBITMAP hTempBitmap, hOldBitmap; 

	if (!hTempMemDC) hTempMemDC = CreateCompatibleDC(hdc);

	hTempBitmap = GetRotatedBitmap(hdc, m_hTempBitmap, iNewWidth, iNewHeight, fRadians);

	hOldBitmap = (HBITMAP)SelectObject(hTempMemDC, hTempBitmap);

	BitBlt(m_hRotatedDC, 0, 0, iNewWidth, iNewHeight, hTempMemDC, 0, 0, SRCCOPY);

	SelectObject(hTempMemDC, hOldBitmap);
	DeleteObject(hTempBitmap);
	ReleaseDC(g_hWnd, hdc);
}
