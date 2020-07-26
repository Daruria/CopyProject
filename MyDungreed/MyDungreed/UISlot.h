#pragma once
#include "MyButton.h"

class CUISlot :
	public CMyButton
{
public:
	CUISlot();
	virtual ~CUISlot();

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath, const string& strOnKey, const wchar_t* pOnPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

protected:
	bool			m_bIconRender;
	CItem*			m_pItem;
	INVEN_SLOT_TYPE m_eSlotType;

public:
	void IconOn() { if (m_pItem) m_bIconRender = true; }
	void IconOff() { if (m_pItem) m_bIconRender = false; }

public:
	void SetItem(CItem* pItem) { m_pItem = pItem; }
	void SetSlotType(const INVEN_SLOT_TYPE eSlotType) { m_eSlotType = eSlotType; }

public:
	CItem* GetItem() const { return m_pItem; }
	const INVEN_SLOT_TYPE GetSlotType() const { return m_eSlotType; }

};

