#pragma once
class CScrollManager
{
public:
	CScrollManager();
	~CScrollManager();

private:
	static float	m_fScrollX;
	static float	m_fScrollY;
	static int		m_iScrollSizeX;
	static int		m_iScrollSizeY;
	static int		m_iScrollStartX;
	static int		m_iScrollStartY;

public:
	static void SetScrollX(float fx) { m_fScrollX = fx; }
	static void SetScrollY(float fy) { m_fScrollY = fy; }
	static void AddScrollX(float fx) { m_fScrollX += fx; }
	static void AddScrollY(float fy) { m_fScrollY += fy; }
	static void SetScrollSizeX(int iScrollSizeX) { m_iScrollSizeX = iScrollSizeX; }
	static void SetScrollSizeY(int iScrollSizeY) { m_iScrollSizeY = iScrollSizeY; }
	static void SetScrollStartX(int iScrollStartX) { m_iScrollStartX = iScrollStartX; }
	static void SetScrollStartY(int iScrollStartY) { m_iScrollStartY = iScrollStartY; }

public:
	static const float GetScrollX()		{ return m_fScrollX; }
	static const float GetScrollY()		{ return m_fScrollY; }
	static const int GetScrollSizeX()	{ return m_iScrollSizeX; }
	static const int GetScrollSizeY()	{ return m_iScrollSizeY; }
	static const int GetScrollStartX()	{ return m_iScrollStartX; }
	static const int GetScrollStartY()	{ return m_iScrollStartY; }
	static const float GetScreenLeft()	{ return m_fScrollX - WINCX / 2; }
	static const float GetScreenTop()	{ return m_fScrollY - WINCY / 2; }
	static const float GetScreenRight() { return m_fScrollX + WINCX / 2; }
	static const float GetScreenBottom(){ return m_fScrollY + WINCY / 2; }

public:
	static const POSITION WorldToScreen(const INFO& tInfo);
	static const POSITION ScreenToWorld(const INFO& tInfo);
	static const POSITION ScreenToWorld(const POSITION& tPos);
	static const POSITION ScreenToWorld(const POINT& tPoint);
	static const POSITION ScreenToWorld(const float fx, const float fy);
	static const POSITION ScreenToWorld(const int ix, const int iy);


public:
	static void ScrollLock();

};

