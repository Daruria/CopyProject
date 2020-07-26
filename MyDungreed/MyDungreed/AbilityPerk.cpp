#include "stdafx.h"
#include "AbilityPerk.h"


CAbilityPerk::CAbilityPerk():
	m_bIsActive(false)
{
}


CAbilityPerk::~CAbilityPerk()
{
	Release();
}

void CAbilityPerk::Initialize(const string & strKey, const wchar_t * pPath, const string & strOnKey, const wchar_t * pOnPath)
{
	CMyButton::Initialize(strKey, pPath, strOnKey, pOnPath);
	INSERT_BITMAP("AbilityPerkSelected", L"Image/UI/Ability/Icon/BMP/AbilityPerkSelected.bmp");
	INSERT_BITMAP("DescBack", L"Image/UI/Rect/DescBack.bmp");

	INSERT_BITMAP("a5", L"Image/UI/Ability/Icon/Level/a5.bmp");
	INSERT_BITMAP("a10", L"Image/UI/Ability/Icon/Level/a10.bmp");
	INSERT_BITMAP("a20", L"Image/UI/Ability/Icon/Level/a20.bmp");
}

int CAbilityPerk::Update()
{
	m_bIsOnMouse = IsOnMouse() ? true : false;


	return 0;
}

void CAbilityPerk::LateUpdate()
{
}

void CAbilityPerk::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC(m_bIsActive ? m_strOnKey : m_strKey);

	SIZE tSize = GET_BITMAP_SIZE(m_bIsActive ? m_strOnKey : m_strKey);

	GdiTransparentBlt(hDC,
		static_cast<int>(m_tPoint.x - tSize.cx * m_fRatio * 0.5f),
		static_cast<int>(m_tPoint.y - tSize.cy * m_fRatio * 0.5f),
		static_cast<int>(tSize.cx * m_fRatio),
		static_cast<int>(tSize.cy * m_fRatio),
		hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	string strLevel[3] = {"a5", "a10", "a20"};

	hMemDC = FIND_BITMAP_DC(strLevel[m_eLevel]);
	SIZE tLevelSize = GET_BITMAP_SIZE(strLevel[m_eLevel]);
	float fRatio = 2.f;

	GdiTransparentBlt(hDC,
		static_cast<int>(m_tPoint.x - tLevelSize.cx * fRatio * 0.5f),
		static_cast<int>(m_tPoint.y - tSize.cy * m_fRatio * 0.5f - tLevelSize.cy * fRatio),
		static_cast<int>(tLevelSize.cx * fRatio),
		static_cast<int>(tLevelSize.cy * fRatio),
		hMemDC, 0, 0, tLevelSize.cx, tLevelSize.cy, RGB(255, 0, 255));

	if (m_bIsOnMouse)
	{
		hMemDC = FIND_BITMAP_DC("AbilityPerkSelected");

		tSize = GET_BITMAP_SIZE("AbilityPerkSelected");

		GdiTransparentBlt(hDC,
			static_cast<int>(m_tPoint.x - tSize.cx * m_fRatio * 0.5f),
			static_cast<int>(m_tPoint.y - tSize.cy * m_fRatio * 0.5f),
			static_cast<int>(tSize.cx * m_fRatio),
			static_cast<int>(tSize.cy * m_fRatio),
			hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

		DescriptionRender(hDC);
	}
}

void CAbilityPerk::Release()
{
}

void CAbilityPerk::AddDescription(const string & strText)
{
	string str = strText;
	m_DescList.push_back(str);
}

void CAbilityPerk::DescriptionRender(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("DescBack");

	SIZE tSize = GET_BITMAP_SIZE("DescBack");

	int iFontSizeX = 0;
	for (auto& strDesc : m_DescList)
	{
		int iTempSize = GET_SINGLE(CFontManager)->GetFontTotalXSize(strDesc, FT_NORMAL, 2.f);
		if (iFontSizeX < iTempSize)
			iFontSizeX = iTempSize;
	}

	int iSingleSizeY = GET_SINGLE(CFontManager)->GetFontTotalYSize(m_DescList.front(), FT_NORMAL, 2.f);
	int iFontSizeY = iSingleSizeY * m_DescList.size();

	//int iFontSizeX = GET_SINGLE(CFontManager)->GetFontTotalXSize(m_strDesc, FT_NORMAL, 2.f);
	//int iFontSizeY = GET_SINGLE(CFontManager)->GetFontTotalYSize(m_strDesc, FT_NORMAL, 2.f);

	int iRenderStartX = m_tPoint.x - (iFontSizeX + 10) / 2;
	if (iRenderStartX < 0)
		iRenderStartX = 0;
	
	int iRenderEndX = iRenderStartX + iFontSizeX + 10;
	if (iRenderEndX > WINCX)
		iRenderStartX = WINCX - (iFontSizeX + 10);

	int iRenderStartY = static_cast<int>(m_tPoint.y - m_tSize.cy / 2 - iFontSizeY - 10 - m_tSize.cy * m_fRatio * 0.5f);
	if (iRenderStartY < 0)
		iRenderStartY = 0;

	int iRenderEndY = iRenderStartY + iFontSizeY + 10;
	if (iRenderEndY > WINCY)
		iRenderStartY = WINCY - (iFontSizeY + 10);

	GdiTransparentBlt(hDC, iRenderStartX, iRenderStartY, iFontSizeX + 10, iFontSizeY + 10, hMemDC,
		0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	int iCount = 0;
	for (auto& strDesc : m_DescList)
	{
		GET_SINGLE(CFontManager)->Render(hDC, 
			static_cast<float>(iRenderStartX + (10 + iFontSizeX) / 2), 
			static_cast<float>(iRenderStartY + 5 + iSingleSizeY * iCount), 
			FT_NORMAL, 2.f, strDesc, FPX_CENTER, FPY_TOP);
		++iCount;
	}

	//GET_SINGLE(CFontManager)->Render(hDC, iRenderStartX + 5, iRenderStartY + 5, FT_NORMAL, 2.f, m_strDesc, FPX_LEFT, FPY_TOP);


}