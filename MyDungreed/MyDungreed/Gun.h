#pragma once
#include "RangedWeapon.h"
class CGun :
	public CRangedWeapon
{
public:
	CGun();
	virtual ~CGun();

public:
	virtual bool Attack();
	virtual bool Attack(float fAttack);
	virtual void AddAttackEffect(float fx, float fy);
	virtual const INFO GetAttackBox() const { return m_tAttackBox; }

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void DiscRender(HDC hDC, float fx, float fy);

protected:
	virtual void Shoot();
	virtual void Shoot(float fAttack);

};

