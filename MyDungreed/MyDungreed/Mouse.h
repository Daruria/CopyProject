#pragma once
#include "UI.h"

class CItem;
class CMouse :
	public CUI
{
public:
	CMouse();
	virtual ~CMouse();

	DECLARE_SINGLE(CMouse)

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void SetCursor(const string& strKey, const wchar_t* pPath);

private:
	int		m_iCursor;	//	Basic : 0, Shooting : 1
	CItem*	m_pItem;

public:
	void ChangeCursor(int iCursor);
	void SetItem(CItem* pItem) { m_pItem = pItem; }
	void ItemNullSet() { m_pItem = nullptr; }

public:
	const int GetCursor() const { return m_iCursor; }
	CItem* GetItem() const { return m_pItem; }
};

