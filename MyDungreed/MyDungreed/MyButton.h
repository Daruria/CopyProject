#pragma once

#include "UI.h"
class CMyButton : public CUI
{
public:
	CMyButton();
	virtual ~CMyButton();

protected:
	bool	m_bIsOnMouse;
	float	m_fRatio;
	SIZE	m_tOnSize;
	string	m_strOnKey;
	

public:
	void SetRatio(float fRatio)				{ m_fRatio = fRatio; }
	void SetOnMouse(const bool& bOnMouse)	{ m_bIsOnMouse = bOnMouse; }
	void SetOnSize(LONG lcx, LONG lcy)		{ m_tOnSize.cx = lcx; m_tOnSize.cy = lcy; }
	void SetOnSize(const SIZE& tSize)		{ m_tOnSize = tSize; }
	void SetOnKey(const string& strOnKey)	{ m_strOnKey = strOnKey; }

public:
	const float GetRatio() const	{ return m_fRatio; }
	const bool GetOnMouse() const	{ return m_bIsOnMouse; }
	const SIZE& GetOnSize()	const	{ return m_tOnSize; }
	const string& GetOnKey() const	{ return m_strOnKey; }

protected:
	const BOOL IsOnMouse();

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath, const string& strOnKey, const wchar_t* pOnPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

};

