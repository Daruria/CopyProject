#pragma once
class CMath
{
public:
	CMath();
	~CMath();

public:
	static const RECT InfoToRect(const INFO& tInfo);
	static const RECT InfoToRect(float fx, float fy, int icx, int icy);
	static const RECT InfoToRect(int ix, int iy, int icx, int icy);

	static const SPHERE InfoToSphere(const INFO& tInfo);


public:
	static int	LDTriangleTop(float fx, const RECT& LDTriangle);
	static int	RDTriangleTop(float fx, const RECT& RDTriangle);

public:
	static void NormalizeXY(float& fx, float& fy);

	static const float Distance(float fx1, float fy1, float fx2, float fy2);
	static const float Distance(const POSITION& p1, const POSITION& p2);
	static const float Distance(const POINT& p1, const POINT& p2);

	static float Angle(float fx1, float fy1, float fx2, float fy2, float fx0 = 0.f, float fy0 = 0.f);
	static float Angle(const POSITION& p1, const POSITION& p2, const POSITION& p0 = POSITION{0.f, 0.f});
	static float RadiansToDegrees(float fRadians);
	static float DegreesToRadians(float fDegrees);

public:
	static const DWORD Flagging(DWORD dwFlags, DWORD dwFlag);
	static const DWORD Deflagging(DWORD dwFlags, DWORD dwFlag);
	static const bool GetFlag(DWORD dwFlags, DWORD dwFlag);

public:
	static void NotFoundImage(HDC hDC, int iStartX, int iStartY, int iCX, int iCY);
	static void DrawBox(HDC hDC, int iStartX, int iStartY, int iCX, int iCY);
	static void DrawLine(HDC hDC, int iStartX, int iStartY, int iEndX, int iEndY, COLORREF LineColor);
};

