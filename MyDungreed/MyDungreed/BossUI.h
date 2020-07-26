#pragma once
#include "UI.h"
class CBossUI :
	public CUI
{
public:
	CBossUI();
	virtual ~CBossUI();

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void Initialize();

private:
	bool	m_bLife;
	int		m_iHp;
	int		m_iMaxHp;
	float	m_fRatio;
};

