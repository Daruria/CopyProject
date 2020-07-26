#pragma once
#include "Item.h"

enum WEAPON_TYPE { WT_MELEE, WT_RANGED, WT_END };

class CWeapon :
	public CItem
{
public:
	CWeapon();
	virtual ~CWeapon();

protected:
	bool	m_bIsSkill;			//	True : There is a Skill, False : There no Skill
	bool	m_bCanUseSkill;		//	True : 스킬 사용 가능. False : 스킬 사용 불가.
	bool	m_bRenderBack;		//	True : Render weapon behind Player, False : Render weapon a front of Player.
	bool	m_bAttack;
	bool	m_bEquip;
	int		m_iAttack;
	DWORD	m_dwSkillCool;
	DWORD	m_dwSkillCoolMax;
	DWORD	m_dwAttackCool;
	DWORD	m_dwAttackCoolMax;
	WEAPON_TYPE m_eWeaponType;
	string	m_strKey;
	string	m_strSkillIconKey;
	INFO	m_tAttackBox;
	

public:
	void SetAttacking(bool bAttack)						{ m_bAttack = bAttack; }
	void SetAttack(const int iAttack)					{ m_iAttack = iAttack; }
	void SetWeaponType(const WEAPON_TYPE eWeaponType)	{ m_eWeaponType = eWeaponType; }

public:
	const bool GetAttacking()			const { return m_bAttack; }
	const int GetAttack()				const { return m_iAttack; }
	const WEAPON_TYPE GetWeaponType()	const { return m_eWeaponType; }
	const bool IsThereSkill()			const { return m_bIsSkill; }
	const bool CanUseSkill()			const { return m_bCanUseSkill; }
	const DWORD GetSkillCool()			const { return m_dwSkillCool; }
	const DWORD GetSkillCoolMax()		const { return m_dwSkillCoolMax; }
	const DWORD GetAttackCool()			const { return m_dwAttackCool; }
	const DWORD GetAttackCoolMax()		const { return m_dwAttackCoolMax; }
	const string& GetSkillIcon()		const { return m_strSkillIconKey; }

public:	
	void SetRenderBack(bool bRenderback) { m_bRenderBack = bRenderback; }
	void SetImage(const string& strKey, const wchar_t* pPath);
	void Equip()	{ m_bEquip = true; }
	void Unequip()	{ m_bEquip = false; }
	
	virtual bool Attack();
	virtual bool Attack(float fAttack);
	virtual void AddAttackEffect(float fx, float fy);
	virtual void UseSkill();

public:
	const bool GetRenderBack()			const { return m_bRenderBack; }
	virtual const INFO GetAttackBox()	const { return m_tAttackBox; }

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void DiscRender(HDC hDC, float fx, float fy);

};

