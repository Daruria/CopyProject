#pragma once
class CFontManager
{
public:
	CFontManager();
	~CFontManager();

	DECLARE_SINGLE(CFontManager)

public:
	void Initialize();
	void Render(HDC hDC, float fx, float fy, FONT_TYPE eFontType, float fRatio, const string& strSentence, FONT_POS_X eFontPosX = FPX_LEFT, FONT_POS_Y eFontPosY = FPY_TOP);
	void Release();

public:
	const int GetFontTotalXSize(const string& strText, FONT_TYPE eFontType, float fRatio);
	const int GetFontTotalYSize(const string& strText, FONT_TYPE eFontType, float fRatio);

private:
	void FontInfoInit();

private:
	vector<INFO> m_vecNormalFontInfo;	//	{������x, ������y, ũ��w, ũ��h};
	vector<INFO> m_vecEdgeFontInfo;		//	{������x, ������y, ũ��w, ũ��h};
	vector<INFO> m_vecFieldFontInfo;	//	{������x, ������y, ũ��w, ũ��h};
};

