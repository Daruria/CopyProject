#pragma once
#include "Weapon.h"
class CRod :
	public CWeapon
{
public:
	CRod();
	virtual ~CRod();

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

private:
	bool	m_bIsTurning;
	float	m_fAngle;
	DWORD	m_dwTurningTime;
	DWORD	m_dwPrevTime;

};

