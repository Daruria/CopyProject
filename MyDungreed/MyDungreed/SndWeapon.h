#pragma once
#include "Shield.h"
class CSndWeapon :
	public CShield
{
public:
	CSndWeapon();
	virtual ~CSndWeapon();

public:	
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void DiscRender(HDC hDC, float fx, float fy);

private:
	float m_fAttack;

};

