#include "stdafx.h"
#include "FontManager.h"

DEFINITION_SINGLE(CFontManager);

CFontManager::CFontManager()
{
}


CFontManager::~CFontManager()
{
	Release();
}

void CFontManager::Initialize()
{
	INSERT_BITMAP("FieldFontSpriteMerge", L"Image/Font/FieldFont/FieldFontSpriteMerge.bmp");
	INSERT_BITMAP("FieldGoldFontSpriteMerge", L"Image/Font/FieldFont/FieldGoldFontSpriteMerge.bmp");
	INSERT_BITMAP("NormalFontSpriteMerge", L"Image/Font/NormalFont/NormalFontSpriteMerge.bmp");
	INSERT_BITMAP("EdgeFontSpriteMerge", L"Image/Font/EdgeFont/EdgeFontSpriteMerge.bmp");
	
	FontInfoInit();
}

void CFontManager::Render(HDC hDC, float fx, float fy, FONT_TYPE eFontType, float fRatio, const string & strSentence, FONT_POS_X eFontPosX, FONT_POS_Y eFontPosY)
{
	HDC hMemDC = 0;
	vector<INFO> vecInfo;
	
	switch (eFontType)
	{
	case FT_NORMAL:
		hMemDC = FIND_BITMAP_DC("NormalFontSpriteMerge");
		vecInfo = m_vecNormalFontInfo;
		break;
	case FT_EDGE:
		hMemDC = FIND_BITMAP_DC("EdgeFontSpriteMerge");
		vecInfo = m_vecEdgeFontInfo;
		break;
	case FT_FIELD:
		hMemDC = FIND_BITMAP_DC("FieldFontSpriteMerge");
		vecInfo = m_vecFieldFontInfo;
		break;
	case FT_FIELD_GOLD:
		hMemDC = FIND_BITMAP_DC("FieldGoldFontSpriteMerge");
		vecInfo = m_vecFieldFontInfo;
		break;
	case FT_END:
		break;
	default:
		break;
	}

	int iSizeX = GetFontTotalXSize(strSentence, eFontType, fRatio);
	int iSizeY = GetFontTotalYSize(strSentence, eFontType, fRatio);

	fx -= static_cast<int>(eFontPosX) * iSizeX * 0.5f;
	fy -= static_cast<int>(eFontPosY) * iSizeY * 0.5f;

	//switch (eFontPosX)
	//{
	//case FPX_LEFT:
	//	break;
	//case FPX_CENTER:
	//	fx -= 0.5f * iSizeX;
	//	break;
	//case FPX_RIGHT:
	//	fx -= iSizeX;
	//	break;
	//case FPX_END:
	//	break;
	//default:
	//	break;
	//}

	for (size_t i = 0; i < strSentence.size(); ++i)
	{
		char c = strSentence[i];

		int iFont = 0;

		if ('!' <= c && c <= '/')
			iFont = c - '!';
		else if ('0' <= c && c <= '9')
			iFont = c - '0' + 15;
		else if ('a' <= c && c <= 'z')
			iFont = c - 'a' + 25;
		else if ('A' <= c && c <= 'Z')
			iFont = c - 'A' + 51;
		else if ('?' == c)
		{
			iFont = 77;
		}
			else
		{
			fx += 3 * fRatio;
			continue;
		}

		GdiTransparentBlt(hDC, (int)fx, (int)fy, int(vecInfo[iFont].icx * fRatio), int(vecInfo[iFont].icy * fRatio), hMemDC,
			int(vecInfo[iFont].fx), int(vecInfo[iFont].fy), vecInfo[iFont].icx, vecInfo[iFont].icy, RGB(255, 0, 255));
		fx += (vecInfo[iFont].icx + 1)* fRatio;
	}
}

void CFontManager::Release()
{
}

const int CFontManager::GetFontTotalXSize(const string & strText, FONT_TYPE eFontType, float fRatio)
{
	vector<INFO> vecInfo;

	switch (eFontType)
	{
	case FT_NORMAL:			vecInfo = m_vecNormalFontInfo;
		break;
	case FT_EDGE:			vecInfo = m_vecEdgeFontInfo;
		break;
	case FT_FIELD:			vecInfo = m_vecFieldFontInfo;
		break;
	case FT_FIELD_GOLD:		vecInfo = m_vecFieldFontInfo;
		break;
	case FT_END:
		break;
	default:
		break;
	}

	int iSize = 0;

	for (size_t i = 0; i < strText.size(); ++i)
	{
		char c = strText[i];

		int iFont = 0;

		if ('!' <= c && c <= '/')
			iFont = c - '!';
		else if ('0' <= c && c <= '9')
			iFont = c - '0' + 15;
		else if ('a' <= c && c <= 'z')
			iFont = c - 'a' + 25;
		else if ('A' <= c && c <= 'Z')
			iFont = c - 'A' + 51;
		else
		{
			iSize += static_cast<int>(3 * fRatio);
			continue;
		}
		
		iSize += static_cast<int>((vecInfo[iFont].icx + 1)* fRatio);
	}

	return iSize;
}

const int CFontManager::GetFontTotalYSize(const string & strText, FONT_TYPE eFontType, float fRatio)
{
	vector<INFO> vecInfo;

	switch (eFontType)
	{
	case FT_NORMAL:			vecInfo = m_vecNormalFontInfo;	break;
	case FT_EDGE:			vecInfo = m_vecEdgeFontInfo;	break;
	case FT_FIELD:			vecInfo = m_vecFieldFontInfo;	break;
	case FT_FIELD_GOLD:		vecInfo = m_vecFieldFontInfo;	break;
	case FT_END:
		break;
	default:
		break;
	}

	return static_cast<int>(vecInfo[0].icy * fRatio);
}

void CFontManager::FontInfoInit()
{
	m_vecFieldFontInfo.assign(78, INFO{});
	// ! ~ /
	m_vecFieldFontInfo[0]  = INFO{ 4.f,	0.f, 4,	16 };
	m_vecFieldFontInfo[1]  = INFO{ 19.f, 0.f, 9, 16 };
	m_vecFieldFontInfo[2]  = INFO{ 32.f, 0.f, 10, 16 };
	m_vecFieldFontInfo[3]  = INFO{ 48.f, 0.f, 8, 16 };
	m_vecFieldFontInfo[4]  = INFO{ 62.f, 0.f, 12, 16 };
	m_vecFieldFontInfo[5]  = INFO{ 77.f, 0.f, 11, 16 };
	m_vecFieldFontInfo[6]  = INFO{ 95.f, 0.f, 3, 16 };
	m_vecFieldFontInfo[7]  = INFO{ 110.f, 0.f, 6, 16 };
	m_vecFieldFontInfo[8]  = INFO{ 124.f, 0.f, 6, 16 };
	m_vecFieldFontInfo[9]  = INFO{ 139.f, 0.f, 6, 16 };
	m_vecFieldFontInfo[10] = INFO{ 152.f, 0.f, 10, 16 };
	m_vecFieldFontInfo[11] = INFO{ 171.f, 0.f, 3, 16 };
	m_vecFieldFontInfo[12] = INFO{ 184.f, 0.f, 6, 16 };
	m_vecFieldFontInfo[13] = INFO{ 201.f, 0.f, 2, 16 };
	m_vecFieldFontInfo[14] = INFO{ 212.f, 0.f, 10, 16 };
	
	// 0 ~ 9
	m_vecFieldFontInfo[15] = INFO{ 1.f, 16.f, 7, 16 };
	m_vecFieldFontInfo[16] = INFO{ 12.f, 16.f, 4, 16 };
	m_vecFieldFontInfo[17] = INFO{ 20.f, 16.f, 6, 16 };
	m_vecFieldFontInfo[18] = INFO{ 28.f, 16.f, 7, 16 };
	m_vecFieldFontInfo[19] = INFO{ 36.f, 16.f, 9, 16 };
	m_vecFieldFontInfo[20] = INFO{ 46.f, 16.f, 7, 16 };
	m_vecFieldFontInfo[21] = INFO{ 55.f, 16.f, 8, 16 };
	m_vecFieldFontInfo[22] = INFO{ 64.f, 16.f, 8, 16 };
	m_vecFieldFontInfo[23] = INFO{ 73.f, 16.f, 8, 16 };
	m_vecFieldFontInfo[24] = INFO{ 82.f, 16.f, 8, 16 };


	// a ~ z
	m_vecFieldFontInfo[25] = INFO{ 5.f,  32.f, 7, 16 };
	m_vecFieldFontInfo[26] = INFO{ 16.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[27] = INFO{ 29.f, 32.f, 7, 16 };
	m_vecFieldFontInfo[28] = INFO{ 40.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[29] = INFO{ 53.f, 32.f, 7, 16 };
	m_vecFieldFontInfo[30] = INFO{ 65.f, 32.f, 7, 16 };
	m_vecFieldFontInfo[31] = INFO{ 76.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[32] = INFO{ 87.f, 32.f, 9, 16 };
	m_vecFieldFontInfo[33] = INFO{ 103.f, 32.f, 4, 16 };
	m_vecFieldFontInfo[34] = INFO{ 115.f, 32.f, 3, 16 };

	m_vecFieldFontInfo[35] = INFO{ 123.f, 32.f, 9, 16 };
	m_vecFieldFontInfo[36] = INFO{ 139.f, 32.f, 4, 16 };
	m_vecFieldFontInfo[37] = INFO{ 144.f, 32.f, 12, 16 };
	m_vecFieldFontInfo[38] = INFO{ 159.f, 32.f, 9, 16 };
	m_vecFieldFontInfo[39] = INFO{ 173.f, 32.f, 7, 16 };
	m_vecFieldFontInfo[40] = INFO{ 184.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[41] = INFO{ 196.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[42] = INFO{ 209.f, 32.f, 7, 16 };
	m_vecFieldFontInfo[43] = INFO{ 222.f, 32.f, 6, 16 };
	m_vecFieldFontInfo[44] = INFO{ 234.f, 32.f, 5, 16 };

	m_vecFieldFontInfo[45] = INFO{ 243.f, 32.f, 9, 16 };
	m_vecFieldFontInfo[46] = INFO{ 256.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[47] = INFO{ 265.f, 32.f, 11, 16 };
	m_vecFieldFontInfo[48] = INFO{ 280.f, 32.f, 8, 16 };
	m_vecFieldFontInfo[49] = INFO{ 293.f, 32.f, 7, 16 };
	m_vecFieldFontInfo[50] = INFO{ 305.f, 32.f, 7, 16 };

	// A ~ Z
	m_vecFieldFontInfo[51] = INFO{ 4.f,   49.f, 7, 16 };
	m_vecFieldFontInfo[52] = INFO{ 16.f,  49.f, 8, 16 };
	m_vecFieldFontInfo[53] = INFO{ 28.f,  49.f, 8, 16 };
	m_vecFieldFontInfo[54] = INFO{ 39.f,  49.f, 9, 16 };
	m_vecFieldFontInfo[55] = INFO{ 51.f,  49.f, 9, 16 };
	m_vecFieldFontInfo[56] = INFO{ 63.f,  49.f, 9, 16 };
	m_vecFieldFontInfo[57] = INFO{ 76.f,  49.f, 8, 16 };
	m_vecFieldFontInfo[58] = INFO{ 87.f,  49.f, 9, 16 };
	m_vecFieldFontInfo[59] = INFO{ 103.f, 49.f, 4, 16 };
	m_vecFieldFontInfo[60] = INFO{ 115.f, 49.f, 3, 16 };

	m_vecFieldFontInfo[61] = INFO{ 123.f, 49.f, 9, 16 };
	m_vecFieldFontInfo[62] = INFO{ 136.f, 49.f, 7, 16 };
	m_vecFieldFontInfo[63] = INFO{ 144.f, 49.f, 12, 16 };
	m_vecFieldFontInfo[64] = INFO{ 158.f, 49.f, 10, 16 };
	m_vecFieldFontInfo[65] = INFO{ 172.f, 49.f, 8, 16 };
	m_vecFieldFontInfo[66] = INFO{ 184.f, 49.f, 8, 16 };
	m_vecFieldFontInfo[67] = INFO{ 195.f, 49.f, 9, 16 };
	m_vecFieldFontInfo[68] = INFO{ 207.f, 49.f, 8, 16 };
	m_vecFieldFontInfo[69] = INFO{ 220.f, 49.f, 8, 16 };
	m_vecFieldFontInfo[70] = INFO{ 231.f, 49.f, 10, 16 };

	m_vecFieldFontInfo[71] = INFO{ 242.f, 49.f, 10, 16 };
	m_vecFieldFontInfo[72] = INFO{ 255.f, 49.f, 9, 16 };
	m_vecFieldFontInfo[73] = INFO{ 265.f, 49.f, 11, 16 };
	m_vecFieldFontInfo[74] = INFO{ 279.f, 49.f, 9, 16 };
	m_vecFieldFontInfo[75] = INFO{ 291.f, 49.f, 9, 16 };
	m_vecFieldFontInfo[76] = INFO{ 304.f, 49.f, 8, 16 };

	//	===========================================Normal Font Info Initialize===============================

	m_vecNormalFontInfo.assign(78, INFO{});

	m_vecNormalFontInfo[0]	= INFO{ 4.f,   0.f, 1,  14 };
	m_vecNormalFontInfo[1]	= INFO{ 13.f,  0.f, 3,  14 };
	m_vecNormalFontInfo[2]	= INFO{ 22.f,  0.f, 6, 14 };
	m_vecNormalFontInfo[3]	= INFO{ 32.f,  0.f, 5,  14 };
	m_vecNormalFontInfo[4]	= INFO{ 41.f,  0.f, 8, 14 };
	m_vecNormalFontInfo[5]	= INFO{ 52.f,  0.f, 6, 14 };
	m_vecNormalFontInfo[6]	= INFO{ 64.f,  0.f, 1,  14 };
	m_vecNormalFontInfo[7]	= INFO{ 74.f, 0.f, 3,  14 };
	m_vecNormalFontInfo[8]	= INFO{ 83.f, 0.f, 3,  14 };
	m_vecNormalFontInfo[9]	= INFO{ 92.f, 0.f, 5,  14 };
	m_vecNormalFontInfo[10] = INFO{ 102.f, 0.f, 5, 14 };
	m_vecNormalFontInfo[11] = INFO{ 114.f, 0.f, 2,  14 };
	m_vecNormalFontInfo[12] = INFO{ 122.f, 0.f, 5,  14 };
	m_vecNormalFontInfo[13] = INFO{ 134.f, 0.f, 1,  14 };
	m_vecNormalFontInfo[14] = INFO{ 143.f, 0.f, 4, 14 };

	// 0 ~ 9
	m_vecNormalFontInfo[15] = INFO{ 1.f,  14.f, 5, 14 };
	m_vecNormalFontInfo[16] = INFO{ 9.f,  14.f, 3, 14 };
	m_vecNormalFontInfo[17] = INFO{ 15.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[18] = INFO{ 22.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[19] = INFO{ 29.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[20] = INFO{ 36.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[21] = INFO{ 43.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[22] = INFO{ 50.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[23] = INFO{ 57.f, 14.f, 5, 14 };
	m_vecNormalFontInfo[24] = INFO{ 64.f, 14.f, 5, 14 };


	// a ~ z
	m_vecNormalFontInfo[25] = INFO{ 1.f,   28.f, 5, 14 };	
	m_vecNormalFontInfo[26] = INFO{ 8.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[27] = INFO{ 15.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[28] = INFO{ 22.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[29] = INFO{ 29.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[30] = INFO{ 36.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[31] = INFO{ 43.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[32] = INFO{ 50.f,  28.f, 5, 14 };	
	m_vecNormalFontInfo[33] = INFO{ 59.f, 28.f, 1, 14 };	
	m_vecNormalFontInfo[34] = INFO{ 64.f, 28.f, 3, 14 };	

	m_vecNormalFontInfo[35] = INFO{ 71.f, 28.f, 5,  14 };	
	m_vecNormalFontInfo[36] = INFO{ 80.f, 28.f, 1,  14 };	
	m_vecNormalFontInfo[37] = INFO{ 85.f, 28.f, 5, 14 };
	m_vecNormalFontInfo[38] = INFO{ 92.f, 28.f, 5,  14 };
	m_vecNormalFontInfo[39] = INFO{ 99.f, 28.f, 5,  14 };
	m_vecNormalFontInfo[40] = INFO{ 106.f, 28.f, 5,  14 };
	m_vecNormalFontInfo[41] = INFO{ 113.f, 28.f, 5,  14 };
	m_vecNormalFontInfo[42] = INFO{ 120.f, 28.f, 5,  14 };
	m_vecNormalFontInfo[43] = INFO{ 127.f, 28.f, 5,  14 };
	m_vecNormalFontInfo[44] = INFO{ 134.f, 28.f, 5,  14 };

	m_vecNormalFontInfo[45] = INFO{ 141.f, 28.f, 5, 14 };
	m_vecNormalFontInfo[46] = INFO{ 148.f, 28.f, 5, 14 };
	m_vecNormalFontInfo[47] = INFO{ 155.f, 28.f, 5, 14 };
	m_vecNormalFontInfo[48] = INFO{ 162.f, 28.f, 5, 14 };
	m_vecNormalFontInfo[49] = INFO{ 169.f, 28.f, 5, 14 };
	m_vecNormalFontInfo[50] = INFO{ 176.f, 28.f, 5, 14 };

	// A ~ Z
	m_vecNormalFontInfo[51] = INFO{ 2.f,   42.f, 5, 14 };
	m_vecNormalFontInfo[52] = INFO{ 11.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[53] = INFO{ 20.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[54] = INFO{ 29.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[55] = INFO{ 38.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[56] = INFO{ 47.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[57] = INFO{ 56.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[58] = INFO{ 65.f,  42.f, 5, 14 };
	m_vecNormalFontInfo[59] = INFO{ 74.f, 42.f, 5, 14 };
	m_vecNormalFontInfo[60] = INFO{ 83.f, 42.f, 5, 14 };

	m_vecNormalFontInfo[61] = INFO{ 92.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[62] = INFO{ 101.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[63] = INFO{ 110.f, 42.f, 5, 14 };
	m_vecNormalFontInfo[64] = INFO{ 119.f, 42.f, 5, 14 };
	m_vecNormalFontInfo[65] = INFO{ 128.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[66] = INFO{ 137.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[67] = INFO{ 146.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[68] = INFO{ 155.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[69] = INFO{ 164.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[70] = INFO{ 173.f, 42.f, 5, 14 };

	m_vecNormalFontInfo[71] = INFO{ 182.f, 42.f, 5, 14 };
	m_vecNormalFontInfo[72] = INFO{ 191.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[73] = INFO{ 200.f, 42.f, 5, 14 };
	m_vecNormalFontInfo[74] = INFO{ 209.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[75] = INFO{ 218.f, 42.f, 5,  14 };
	m_vecNormalFontInfo[76] = INFO{ 227.f, 42.f, 5,  14 };

	//	?
	m_vecNormalFontInfo[77] = INFO{ 151.f, 0.f, 5,  14 };

	//	==========================================Edge========================
	m_vecEdgeFontInfo.assign(78, INFO{});

	m_vecEdgeFontInfo[0] = INFO{ 3.f,   0.f, 3,  14 };
	m_vecEdgeFontInfo[1] = INFO{ 12.f,  0.f, 5,  14 };
	m_vecEdgeFontInfo[2] = INFO{ 21.f,  0.f, 8, 14 };
	m_vecEdgeFontInfo[3] = INFO{ 31.f,  0.f, 7,  14 };
	m_vecEdgeFontInfo[4] = INFO{ 40.f,  0.f, 10, 14 };
	m_vecEdgeFontInfo[5] = INFO{ 51.f,  0.f, 8, 14 };
	m_vecEdgeFontInfo[6] = INFO{ 63.f,  0.f, 3,  14 };
	m_vecEdgeFontInfo[7] = INFO{ 73.f, 0.f, 5,  14 };
	m_vecEdgeFontInfo[8] = INFO{ 82.f, 0.f, 5,  14 };
	m_vecEdgeFontInfo[9] = INFO{ 91.f, 0.f, 7,  14 };
	m_vecEdgeFontInfo[10] = INFO{ 101.f, 0.f, 7, 14 };
	m_vecEdgeFontInfo[11] = INFO{ 113.f, 0.f, 4,  14 };
	m_vecEdgeFontInfo[12] = INFO{ 121.f, 0.f, 7,  14 };
	m_vecEdgeFontInfo[13] = INFO{ 133.f, 0.f, 3,  14 };
	m_vecEdgeFontInfo[14] = INFO{ 142.f, 0.f, 6, 14 };

	// 0 ~ 9
	m_vecEdgeFontInfo[15] = INFO{ 0.f,  14.f, 7, 14 };
	m_vecEdgeFontInfo[16] = INFO{ 8.f,  14.f, 5, 14 };
	m_vecEdgeFontInfo[17] = INFO{ 14.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[18] = INFO{ 21.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[19] = INFO{ 28.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[20] = INFO{ 35.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[21] = INFO{ 42.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[22] = INFO{ 49.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[23] = INFO{ 56.f, 14.f, 7, 14 };
	m_vecEdgeFontInfo[24] = INFO{ 63.f, 14.f, 7, 14 };


	// a ~ z
	m_vecEdgeFontInfo[25] = INFO{ 0.f,   28.f, 7, 15 };
	m_vecEdgeFontInfo[26] = INFO{ 7.f,  28.f, 7,  15 };
	m_vecEdgeFontInfo[27] = INFO{ 14.f,  28.f, 7, 15 };
	m_vecEdgeFontInfo[28] = INFO{ 21.f,  28.f, 7, 15 };
	m_vecEdgeFontInfo[29] = INFO{ 28.f,  28.f, 7, 15 };
	m_vecEdgeFontInfo[30] = INFO{ 35.f,  28.f, 7, 15 };
	m_vecEdgeFontInfo[31] = INFO{ 42.f,  28.f, 7, 15 };
	m_vecEdgeFontInfo[32] = INFO{ 49.f,  28.f, 7, 15 };
	m_vecEdgeFontInfo[33] = INFO{ 58.f, 28.f, 3,  15 };
	m_vecEdgeFontInfo[34] = INFO{ 63.f, 28.f, 5,  15 };

	m_vecEdgeFontInfo[35] = INFO{ 70.f, 28.f, 7,  15 };
	m_vecEdgeFontInfo[36] = INFO{ 79.f, 28.f, 3,  15 };
	m_vecEdgeFontInfo[37] = INFO{ 84.f, 28.f, 7,  15 };
	m_vecEdgeFontInfo[38] = INFO{ 91.f, 28.f, 7,  15 };
	m_vecEdgeFontInfo[39] = INFO{ 98.f, 28.f, 7,  15 };
	m_vecEdgeFontInfo[40] = INFO{ 105.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[41] = INFO{ 112.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[42] = INFO{ 119.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[43] = INFO{ 126.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[44] = INFO{ 133.f, 28.f, 7, 15 };

	m_vecEdgeFontInfo[45] = INFO{ 140.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[46] = INFO{ 147.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[47] = INFO{ 154.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[48] = INFO{ 161.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[49] = INFO{ 168.f, 28.f, 7, 15 };
	m_vecEdgeFontInfo[50] = INFO{ 175.f, 28.f, 7, 15 };

	// A ~ Z
	m_vecEdgeFontInfo[51] = INFO{ 1.f,   43.f, 7, 14 };
	m_vecEdgeFontInfo[52] = INFO{ 10.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[53] = INFO{ 19.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[54] = INFO{ 28.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[55] = INFO{ 37.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[56] = INFO{ 46.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[57] = INFO{ 55.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[58] = INFO{ 64.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[59] = INFO{ 73.f,  43.f, 7, 14 };
	m_vecEdgeFontInfo[60] = INFO{ 82.f,  43.f, 7, 14 };

	m_vecEdgeFontInfo[61] = INFO{ 91.f,  43.f, 7,  14 };
	m_vecEdgeFontInfo[62] = INFO{ 100.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[63] = INFO{ 109.f, 43.f, 7, 14 };
	m_vecEdgeFontInfo[64] = INFO{ 118.f, 43.f, 7, 14 };
	m_vecEdgeFontInfo[65] = INFO{ 127.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[66] = INFO{ 136.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[67] = INFO{ 145.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[68] = INFO{ 154.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[69] = INFO{ 163.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[70] = INFO{ 172.f, 43.f, 7, 14 };

	m_vecEdgeFontInfo[71] = INFO{ 181.f, 43.f, 7, 14 };
	m_vecEdgeFontInfo[72] = INFO{ 190.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[73] = INFO{ 199.f, 43.f, 7, 14 };
	m_vecEdgeFontInfo[74] = INFO{ 208.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[75] = INFO{ 217.f, 43.f, 7,  14 };
	m_vecEdgeFontInfo[76] = INFO{ 226.f, 43.f, 7,  14 };
}
