#pragma once

class CItem;
class CMyButton;
class CInvenSlot;
class CInventory
{
public:
	CInventory();
	~CInventory();

	DECLARE_SINGLE(CInventory)

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

private:
	bool				m_bInvenOn;
	bool				m_bEquipedWeapon;
	int					m_iPlayerGold;
	CMyButton*			m_pExitButton;
	RECT				m_tRect;
	vector<CInvenSlot*>	m_vecSlot;
	vector<CInvenSlot*>	m_vecEquipSlot;
	CInvenSlot*			m_pClickedSlot;

public:	//	Get Function
	const int GetPlayerGold() const { return m_iPlayerGold; }
	vector<CInvenSlot*>* GetInvenSlot() { return &m_vecSlot; }
	vector<CInvenSlot*>* GetEquipSlot() { return &m_vecEquipSlot; }
	CInvenSlot*	GetEquipSlot(const INVEN_SLOT_TYPE eInvenSlotType) { return m_vecEquipSlot[eInvenSlotType]; }

public:	//	단순 기능
	void ToggleInven();
	void OpenInven();
	void CloseInven();
	void EquipSlotUpdate(vector<CItem*>* pVecPlayerEquip);
	void EquipedWeaponUpdate(bool bEquipedWeapon) { m_bEquipedWeapon = bEquipedWeapon; }
	void AddGold(int iGold) { m_iPlayerGold += iGold; }
	bool AddItem(CItem* pItem);
	CItem* FindInventoryItem(const string& strName);
	CInvenSlot* FindItemSlot(const string& strName);


private:
	void EquipSlotSetting();
	void EquipSlotRender(HDC hDC);
	void ChangeSlotItem(CInvenSlot*& pSlot);
	void SlotUpdate(CInvenSlot*& pSlot);

public:
	void ApplyPlayerEquip();


};

