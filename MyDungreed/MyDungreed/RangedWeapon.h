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
	bool	m_bReload;			//	True : 재장전 중, False : 재장전 중이 아님.
	bool	m_bDir;				//	True : Right, False : False
	int		m_iRemainBullet;	//	남은 총알
	int		m_iMagazine;		//	탄창
	float	m_fReloadRatio;		//	재장전 진행 비율. 1.f 이면 완료.
	float	m_fAngle;
	float	m_fLeftAngle;		//	왼쪽 이미지 출력을 위한 각.
	DWORD	m_dwReloadingTime;	//	재장전 시작 시간.
	DWORD	m_dwReloadTime;		//	재장전 완료되는데 까지 걸리는 시간.
	INFO	m_tOffset;
	INFO	m_tReloadInfo;
	string	m_strLeftKey;	//	왼쪽 이미지 키

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

