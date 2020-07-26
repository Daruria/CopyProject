#pragma once
#include "MyButton.h"

class CUIWindow;
class CSelectBox :
	public CMyButton
{
public:
	enum BOX_SORT_BY_X { LEFT, CENTER, RIGHT, X_END };
	enum BOX_SORT_BY_Y { TOP, MIDDLE, BOTTOM, Y_END};

public:
	CSelectBox();
	virtual ~CSelectBox();

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath, const string& strOnKey, const wchar_t* pOnPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void BoxSetting(const string& strText, const SIZE& tEdgeSize, float fRatio = 1.f, float fTextRatio = 1.f, BOX_SORT_BY_X eSortByX = CENTER, BOX_SORT_BY_Y eSortByY = MIDDLE);

public:
	void SetSortByX(BOX_SORT_BY_X eSortByX) { m_eSortByX = eSortByX; }
	void SetSortByY(BOX_SORT_BY_Y eSortByY) { m_eSortByY = eSortByY; }
	void SetEdgeSize(const SIZE& tSize)		{ m_tEdgeSize = tSize; }

	void SetTextRatio(float fTextRatio)		{ m_fTextRatio = fTextRatio; }
	void SetText(const string& strText)		{ m_strText = strText; }

	void SetParentWindow(CUIWindow* pParentWindow)	{ m_pParentWindow = pParentWindow; }
	void SetChildWindow(CUIWindow* pChildWindow)	{ m_pChildWindow = pChildWindow; }

public:
	const BOX_SORT_BY_X GetSortByX()	const { return m_eSortByX; }
	const BOX_SORT_BY_Y GetSortByY()	const { return m_eSortByY; }
	const SIZE&			GetEdgeSize()	const { return m_tEdgeSize; }

	const float			GetTextRatio()	const { return m_fTextRatio; }
	const string&		GetText()		const { return m_strText; }

	const int			GetSizeX()		const { return m_tSize.cx; }
	const int			GetSizeY()		const { return m_tSize.cy; }

private:
	const BOOL IsBoxOnMouse() const;

private:
	BOX_SORT_BY_X	m_eSortByX;
	BOX_SORT_BY_Y	m_eSortByY;
	SIZE			m_tEdgeSize;

	float			m_fTextRatio;
	string			m_strText;
	
	CUIWindow*		m_pParentWindow;
	CUIWindow*		m_pChildWindow;

};

