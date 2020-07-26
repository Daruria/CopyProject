#pragma once

class CFontNumber
{
public:
	CFontNumber();
	~CFontNumber();

public:
	static void Initialize();
	static void RenderNumber(HDC hDC, const string& strNumber, float fx, float fy, int iStyle = 0x11);
	static void Release();

private:
	static void RenderOneLetter(HDC hDC, char c, int ix, int iy, int iStyle);
};

