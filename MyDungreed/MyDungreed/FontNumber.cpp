#include "stdafx.h"
#include "FontNumber.h"


CFontNumber::CFontNumber()
{
}


CFontNumber::~CFontNumber()
{
}

void CFontNumber::Initialize()
{
	INSERT_BITMAP("FontNumber", L"Image/Font/FontNumberSprite.bmp");
	INSERT_BITMAP("FontNumber(x2)", L"Image/Font/FontNumberSprite(x2).bmp");
	INSERT_BITMAP("FontNumber(x3)", L"Image/Font/FontNumberSprite(x3).bmp");
	INSERT_BITMAP("FontNumber(x4)", L"Image/Font/FontNumberSprite(x4).bmp");
	INSERT_BITMAP("FontEdgeNumber", L"Image/Font/FontEdgeNumberSprite.bmp");
	INSERT_BITMAP("FontEdgeNumber(x2)", L"Image/Font/FontEdgeNumberSprite(x2).bmp");
	INSERT_BITMAP("FontEdgeNumber(x3)", L"Image/Font/FontEdgeNumberSprite(x3).bmp");
	INSERT_BITMAP("FontEdgeNumber(x4)", L"Image/Font/FontEdgeNumberSprite(x4).bmp");

	INSERT_BITMAP("FontSlash", L"Image/Font/FontSlash.bmp");
	INSERT_BITMAP("FontSlash(x2)", L"Image/Font/FontSlash(x2).bmp");
	INSERT_BITMAP("FontSlash(x3)", L"Image/Font/FontSlash(x3).bmp");
	INSERT_BITMAP("FontSlash(x4)", L"Image/Font/FontSlash(x4).bmp");

	INSERT_BITMAP("FontEdgeSlash", L"Image/Font/FontEdgeSlash.bmp");
	INSERT_BITMAP("FontEdgeSlash(x2)", L"Image/Font/FontEdgeSlash(x2).bmp");
	INSERT_BITMAP("FontEdgeSlash(x3)", L"Image/Font/FontEdgeSlash(x3).bmp");
	INSERT_BITMAP("FontEdgeSlash(x4)", L"Image/Font/FontEdgeSlash(x4).bmp");
}

void CFontNumber::RenderNumber(HDC hDC, const string & strNumber, float fx, float fy, int iStyle)
{
	int iLength = strNumber.size();
	
	int iOneLetterSizeX = 7 * (iStyle & FONT_STYLE_SIZE);
	int iOneLetterSizeY = 11 * (iStyle & FONT_STYLE_SIZE);
	int iStartX = fx - (iLength * iOneLetterSizeX / 2);

	for (size_t i = 0; i < iLength; ++i)
	{
		RenderOneLetter(hDC, strNumber[i], iStartX + iOneLetterSizeX * i, fy, iStyle);
	}
}

void CFontNumber::Release()
{
}

void CFontNumber::RenderOneLetter(HDC hDC, char c, int ix, int iy, int iStyle)
{
	HDC hMemDC = FIND_BITMAP_DC("FontNumber");
	int iSizeX = 7 * (iStyle & FONT_STYLE_SIZE);
	int iSizeY = 11 * (iStyle & FONT_STYLE_SIZE);

	if (c == ' ')
		return;
	
	if (c == '/')
	{
		switch (iStyle)
		{
		case FONT_STYLE_X1 | FONT_STYLE_NORMAL:
			hMemDC = FIND_BITMAP_DC("FontSlash");
			break;
		case FONT_STYLE_X2 | FONT_STYLE_NORMAL:
			hMemDC = FIND_BITMAP_DC("FontSlash(x2)");
			break;
		case FONT_STYLE_X3 | FONT_STYLE_NORMAL:
			hMemDC = FIND_BITMAP_DC("FontSlash(x3)");
			break;
		case FONT_STYLE_X4 | FONT_STYLE_NORMAL:
			hMemDC = FIND_BITMAP_DC("FontSlash(x4)");
			break;
		case FONT_STYLE_X1 | FONT_STYLE_EDGE:
			hMemDC = FIND_BITMAP_DC("FontEdgeSlash");
			break;
		case FONT_STYLE_X2 | FONT_STYLE_EDGE:
			hMemDC = FIND_BITMAP_DC("FontEdgeSlash(x2)");
			break;
		case FONT_STYLE_X3 | FONT_STYLE_EDGE:
			hMemDC = FIND_BITMAP_DC("FontEdgeSlash(x3)");
			break;
		case FONT_STYLE_X4 | FONT_STYLE_EDGE:
			hMemDC = FIND_BITMAP_DC("FontEdgeSlash(x4)");
			break;
		}

		GdiTransparentBlt(hDC, ix, iy, iSizeX, iSizeY, hMemDC, 0, 0, iSizeX, iSizeY, RGB(255, 0, 255));
		return;
	}

	switch (iStyle)
	{
	case FONT_STYLE_X1 | FONT_STYLE_NORMAL:
		hMemDC = FIND_BITMAP_DC("FontNumber");
		break;
	case FONT_STYLE_X2 | FONT_STYLE_NORMAL:
		hMemDC = FIND_BITMAP_DC("FontNumber(x2)");
		break;
	case FONT_STYLE_X3 | FONT_STYLE_NORMAL:
		hMemDC = FIND_BITMAP_DC("FontNumber(x3)");
		break;
	case FONT_STYLE_X4 | FONT_STYLE_NORMAL:
		hMemDC = FIND_BITMAP_DC("FontNumber(x4)");
		break;
	case FONT_STYLE_X1 | FONT_STYLE_EDGE:
		hMemDC = FIND_BITMAP_DC("FontEdgeNumber");
		break;
	case FONT_STYLE_X2 | FONT_STYLE_EDGE:
		hMemDC = FIND_BITMAP_DC("FontEdgeNumber(x2)");
		break;
	case FONT_STYLE_X3 | FONT_STYLE_EDGE:
		hMemDC = FIND_BITMAP_DC("FontEdgeNumber(x3)");
		break;
	case FONT_STYLE_X4 | FONT_STYLE_EDGE:
		hMemDC = FIND_BITMAP_DC("FontEdgeNumber(x4)");
		break;
	}

	c -= '0';
	//SIZE tSize = GET_BITMAP_SIZE("FontNumber(x2)");
	BOOL bRender = GdiTransparentBlt(hDC, ix, iy, iSizeX, iSizeY, hMemDC, iSizeX * c, 0, iSizeX, iSizeY, RGB(255, 0, 255));
	int A = 0;
}
