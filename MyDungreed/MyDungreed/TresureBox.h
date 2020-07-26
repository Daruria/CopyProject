#pragma once
#include "MoveObj.h"
class CTresureBox :
	public CMoveObj
{
public:
	CTresureBox();
	virtual ~CTresureBox();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	bool	m_bCanOpen;
	bool	m_bOpenClose;	//	True : Open, False : Close;
	bool	m_bTakeOutItem;
	string	m_strOpenKey;
	CItem*	m_pItem;

public:
	void SetCanOpen(bool bCanOpen) { m_bCanOpen = bCanOpen; }
	void SetCloseImage(const string& strKey, const wchar_t* pPath);
	void SetOpenImage(const string& strKey, const wchar_t* pPath);
	void SetBox(int icx, int icy, float fRatio = 1.f);
	void SetItem(CItem* pItem) { m_pItem = pItem; }
};

