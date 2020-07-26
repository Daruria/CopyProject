#pragma once
#include "Item.h"
class CGoldCoin :
	public CItem
{
public:
	CGoldCoin();
	virtual ~CGoldCoin();

public:
	// Inherited via CItem
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int		m_iGold;
	string	m_strKey;

public:
	void SetGold(const int iGold) { m_iGold = iGold; }
	const int GetGold() const { return m_iGold; }
};

