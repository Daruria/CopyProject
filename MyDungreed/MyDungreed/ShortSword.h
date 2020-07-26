#pragma once
#include "Weapon.h"
class CShortSword :
	public CWeapon
{
public:
	CShortSword();
	~CShortSword();

private:
	bool		m_bState;		//	True : Up, False : Down
	float		m_fAngle;
	float		m_fSwingAngle;
	POSITION	m_tDir;
	POSITION	m_tOffSet;
	string		m_strDownKey;

public:
	void	WeaponUp()		{ m_bState = true; }
	void	WeaponDown()	{ m_bState = false; }
	void	SetDownImage(const string& strDownKey, const wchar_t* pPath);
	void	Swing();
	virtual void AddAttackEffect(float fx, float fy);
	virtual bool Attack();
	virtual bool Attack(float fAttack);

public:
	const bool GetWeaponUpDown() const { return m_bState; }	//	True : Up, False : Down


public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void DiscRender(HDC hDC, float fx, float fy);

private:
	void SwingEffect();
};

