#pragma once
#include "Item.h"

class CShield :
	public CItem
{
public:
	enum SHIELD_TYPE { SHIELD, SECONDARY_WEAPON, END };

public:
	CShield();
	virtual ~CShield();

public:	// Inherited via CItem
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void DiscRender(HDC hDC, float fx, float fy);
	
public:
	void SetShieldType(SHIELD_TYPE eType) { m_eShieldType = eType; }

public:
	const SHIELD_TYPE GetShieldType() const { return m_eShieldType; }

protected:
	SHIELD_TYPE	 m_eShieldType;
	
};

