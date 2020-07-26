#pragma once
#include "UI.h"
class CUIWindow :
	public CUI
{
public:
	CUIWindow();
	virtual ~CUIWindow();

public:
	enum TALK_BOX_SORT_BY { LEFT, CENTER, RIGHT };

public:
	virtual void Initialize();
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

protected:
	bool	m_bLife;
	bool	m_bIsOn;	//	UI 창이 켜졌는지 안켜졌는지 체크.
	float	m_fRatio;

public:
	void SetIsOn(bool bIsOn)	{ m_bIsOn = bIsOn; }
	void SetLife(bool bLife)	{ m_bLife = bLife; }
	void SetRatio(float fRatio) { m_fRatio = fRatio; }

public:
	const bool GetLife()	const { return m_bLife; }
	const bool GetIsOn()	const { return m_bIsOn; }
	const float GetRaito()	const { return m_fRatio; }

public:
	virtual void WindowOn()		{ m_bIsOn = true; }
	virtual void WindowOff()	{ m_bIsOn = false; }

protected:
	void TalkBoxRender(HDC hDC, float fx, float fy, int icx, int icy, float fRatio, TALK_BOX_SORT_BY eXSortBy = CUIWindow::LEFT, TALK_BOX_SORT_BY eYSortBy = CUIWindow::LEFT);
};

