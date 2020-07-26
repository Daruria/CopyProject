#include "stdafx.h"
#include "Math.h"


CMath::CMath()
{
}


CMath::~CMath()
{
}

const RECT CMath::InfoToRect(const INFO & tInfo)
{
	return InfoToRect(tInfo.fx, tInfo.fy, tInfo.icx, tInfo.icy);
}

const RECT CMath::InfoToRect(float fx, float fy, int icx, int icy)
{
	return InfoToRect(static_cast<int>(fx), static_cast<int>(fy), icx, icy);
}

const RECT CMath::InfoToRect(int ix, int iy, int icx, int icy)
{
	return RECT{ix - icx / 2, iy - icy / 2, ix + icx / 2, iy + icy / 2};
}

const SPHERE CMath::InfoToSphere(const INFO & tInfo)
{
	SPHERE sp;
	sp.fCenterX = tInfo.fx;
	sp.fCenterY = tInfo.fy;
	sp.fRadius = static_cast<float>(tInfo.icx / 2);
	return sp;
}


int CMath::LDTriangleTop(float fx, const RECT & LDTriangle)
{
	int iHeight = LDTriangle.bottom - LDTriangle.top;
	float fRatio = (LDTriangle.right - fx) / (LDTriangle.right - LDTriangle.left);

	if (fRatio >= 1.f)
		fRatio = 1.f;
	/*else if (fRatio <= 0.f)
	fRatio = 0.f;*/

	return static_cast<int>(LDTriangle.bottom - fRatio * iHeight);
}

int CMath::RDTriangleTop(float fx, const RECT & RDTriangle)
{
	int iHeight = RDTriangle.bottom - RDTriangle.top;
	float fRatio = (fx - RDTriangle.left) / (RDTriangle.right - RDTriangle.left);

	if (fRatio >= 1.f)
		fRatio = 1.f;
	/*else if (fRatio <= 0.f)
	fRatio = 0.f;*/

	return static_cast<int>(RDTriangle.bottom - fRatio * iHeight);
}


void CMath::NormalizeXY(float & fx, float & fy)
{
	if (fx == 0.f && fy == 0.f)
		return;
	float fMagnitude = sqrtf(fx * fx + fy * fy);
	fx /= fMagnitude;
	fy /= fMagnitude;
}

const float CMath::Distance(float fx1, float fy1, float fx2, float fy2)
{
	float fdx = fx1 - fx2;
	float fdy = fy1 - fy2;

	return sqrtf(fdx * fdx + fdy * fdy);
}

const float CMath::Distance(const POSITION & p1, const POSITION & p2)
{
	return Distance(p1.fx, p1.fy, p2.fx, p2.fy);
}

const float CMath::Distance(const POINT & p1, const POINT & p2)
{
	return Distance(static_cast<float>(p1.x), static_cast<float>(p1.y),
		static_cast<float>(p2.x), static_cast<float>(p2.y));
}

float CMath::Angle(float fx1, float fy1, float fx2, float fy2, float fx0, float fy0)
{
	float fdx = (fx1 - fx0) - (fx2 - fx0);
	float fdy = (fy1 - fy0) - (fy2 - fy0);

	float fDist = sqrtf((fdx * fdx + fdy * fdy));

	return fdx >= 0 ? atanf(fdy / fdx) : fdy >= 0 ? acosf(fdx / fDist) : -acosf(fdx / fDist);
}

float CMath::Angle(const POSITION & p1, const POSITION & p2, const POSITION & p0)
{
	return Angle(p1.fx, p1.fy, p2.fx, p2.fy, p0.fx, p0.fy);
}

float CMath::RadiansToDegrees(float fRadians)
{
	return static_cast<float>(fRadians * 180 / PI);
}

float CMath::DegreesToRadians(float fDegrees)
{
	return static_cast<float>(fDegrees * PI / 180);
}

const DWORD CMath::Flagging(DWORD dwFlags, DWORD dwFlag)
{
	return dwFlags | dwFlag;
}

const DWORD CMath::Deflagging(DWORD dwFlags, DWORD dwFlag)
{
	return dwFlags & !dwFlag;
}

const bool CMath::GetFlag(DWORD dwFlags, DWORD dwFlag)
{
	return bool(dwFlags & dwFlag);
}

void CMath::NotFoundImage(HDC hDC, int iStartX, int iStartY, int iCX, int iCY)
{
	int iEndX = iStartX + iCX;
	int iEndY = iStartY + iCY;
	Rectangle(hDC, iStartX, iStartY, iEndX, iEndY);
	string strText = "Not Found Image";
	TextOutA(hDC, iStartX + iCX / 2 - 20, iStartY, strText.c_str(), strText.length());
	MoveToEx(hDC, iStartX, iStartY, nullptr);
	LineTo(hDC, iEndX, iEndY);
	MoveToEx(hDC, iEndX, iStartY, nullptr);
	LineTo(hDC, iStartX, iEndY);
}

void CMath::DrawBox(HDC hDC, int iStartX, int iStartY, int iCX, int iCY)
{
	HPEN h_red_pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	// DC에 생성한 Pen 객체를 연결하고 기존에 사용하던 Pen 객체는 h_old_pen에 저장한다. 
	HGDIOBJ h_old_pen = SelectObject(hDC, h_red_pen);

	int iEndX = iStartX + iCX;
	int iEndY = iStartY + iCY;
	MoveToEx(hDC, iStartX, iStartY, nullptr);
	LineTo(hDC, iEndX, iStartY);
	LineTo(hDC, iEndX, iEndY);
	LineTo(hDC, iStartX, iEndY);
	LineTo(hDC, iStartX, iStartY);

	// 기존에 사용하던 Pen 객체로 복구한다. 
	SelectObject(hDC, h_old_pen);
	// 자신이 만든 Pen 객체를 제거한다. 
	DeleteObject(h_red_pen);
}

void CMath::DrawLine(HDC hDC, int iStartX, int iStartY, int iEndX, int iEndY, COLORREF LineColor)
{
	HPEN h_red_pen = CreatePen(PS_SOLID, 3, LineColor);
	// DC에 생성한 Pen 객체를 연결하고 기존에 사용하던 Pen 객체는 h_old_pen에 저장한다. 
	HGDIOBJ h_old_pen = SelectObject(hDC, h_red_pen);

	MoveToEx(hDC, iStartX, iStartY, nullptr);
	LineTo(hDC, iEndX, iEndY);

	// 기존에 사용하던 Pen 객체로 복구한다. 
	SelectObject(hDC, h_old_pen);
	// 자신이 만든 Pen 객체를 제거한다. 
	DeleteObject(h_red_pen);
}
