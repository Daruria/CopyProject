#pragma once
#include "Weapon.h"
class CRangedWeapon :
	public CWeapon
{
public:
	CRangedWeapon();
	virtual ~CRangedWeapon();

public:
	virtual bool Attack();
	virtual bool Attack(float fAttack);
	virtual void AddAttackEffect(float fx, float fy);
	virtual const INFO GetAttackBox() const { return m_tAttackBox; }
	virtual void UseSkill();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void DiscRender(HDC hDC, float fx, float fy);

protected:
	bool	m_bReload;			//	True : ������ ��, False : ������ ���� �ƴ�.
	bool	m_bDir;				//	True : Right, False : False
	int		m_iRemainBullet;	//	���� �Ѿ�
	int		m_iMagazine;		//	źâ
	float	m_fReloadRatio;		//	������ ���� ����. 1.f �̸� �Ϸ�.
	float	m_fAngle;
	float	m_fLeftAngle;		//	���� �̹��� ����� ���� ��.
	DWORD	m_dwReloadingTime;	//	������ ���� �ð�.
	DWORD	m_dwReloadTime;		//	������ �Ϸ�Ǵµ� ���� �ɸ��� �ð�.
	INFO	m_tOffset;
	INFO	m_tReloadInfo;
	string	m_strLeftKey;	//	���� �̹��� Ű

public:
	const float GetReloadRatio() const { return m_fReloadRatio; }
	const int GetRemainBullet() const { return m_iRemainBullet; }
	const int GetMagazine()		const { return m_iMagazine; }
	const bool GetIsReloading() const { return m_bReload; }

public:
	void SetLeftImage(const string& strLeftKey, const wchar_t* pPath);
	void SetAttackCoolMax(DWORD dwAttackCoolMax) { m_dwAttackCoolMax = dwAttackCoolMax; }

protected:
	void Reload();
	virtual void Shoot();
	virtual void Shoot(float fAttack);
};

