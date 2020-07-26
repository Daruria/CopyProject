#pragma once
#include "UIWindow.h"

class CInvenSlot;
class CShopSlot;
class CShopWindow :
	public CUIWindow
{
public:
	CShopWindow();
	virtual ~CShopWindow();

public:
	virtual void Initialize();
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	list<CShopSlot*> m_ItemSlotList;

public:
	list<CShopSlot*>* GetItemSlotList() { return &m_ItemSlotList; }

public:
	void AddItem(CItem* pItem);

public:
	virtual void WindowOn();
	virtual void WindowOff();
};

