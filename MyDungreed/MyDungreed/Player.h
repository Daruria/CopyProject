#pragma once
#include "MoveObj.h"

enum MOTION {IDLE, DIE, RUN, JUMP, MOTION_END};

class CItem;
class CPlayer :
	public CMoveObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;
	
private:
	bool	m_bGodMode;			//	무적 치트
	bool	m_bIsActiveUpdate;	//	스탯 or 장비 변경 후 업데이트
	bool	m_bHittable;		//	연속 피격 방지
	bool	m_bCanAction;		//	상호 작용중 키 입력 방지
	bool	m_bRender;			//	True : Render, False : Not Render
	bool	m_bInvincibility;	//	True : Unhittable, False : Hittable;
	bool	m_bDir;				//	True : Right, False : Left
	bool	m_bDust;
	bool	m_bWeaponChange;	//	True : WeaponChange Possible, False : Impossible
	bool	m_bEquipedWeapon;	//	True : Weapon 1, False : Weapon 2
	bool	m_bWeaponBack;		//	True : Back, False : Front
	bool	m_bDoubleJump;		//	True : Double Jump 가능 False: 더블점프 불가능
	
	bool	m_bLongJump;
	int		m_iLongJumpCheck;
	float	m_fLongJumpForce;
	float	m_fLongJumpAccel;
	
	int		m_iLevel;
	int		m_iExpMax;
	int		m_iExp;
	int		m_iGold;
	DWORD			m_dwHitTime;
	DWORD			m_dwHitBlankTime;
	DWORD			m_dwAttackCool;
	DWORD			m_dwAttackCoolMax;

	CHARACTER_INFO	m_tCharInfo;
	CHARACTER_INFO	m_tCharActiveInfo;
	vector<CItem*>	m_vecEquip;
	DASH			m_tDash;
	string			m_strAttackMonster;
	
	STATUS			m_tStatus;


public:
	void SetAttackMonster(const string& strMonster) { m_strAttackMonster = strMonster; }
	void SetCanAction(bool bAction)					{ m_bCanAction = bAction; }
	void SetDirRight()								{ m_bDir = true; }
	void SetDirLeft()								{ m_bDir = false; }
	void SetHittable(bool bHittable)				{ m_bHittable = bHittable; }
	void SetBoolRender(bool bRender)				{ m_bRender = bRender; }

public:
	const  int GetLevel()				const { return m_iLevel; }
	const bool GetDir()					const { return m_bDir; }
	const bool GetHittable()			const { return m_bHittable; }
	const CHARACTER_INFO& GetCharInfo() const { return m_tCharInfo; }
	const CHARACTER_INFO& GetActiveInfo() const { return m_tCharActiveInfo; }
	const DASH&		GetDash()			const { return m_tDash; }
	const string&	GetAttackMonster()	const { return m_strAttackMonster; }
	class CWeapon* GetEquipWeapon();
	const int GetExpMax()				const { return m_iExpMax; }
	const bool GetBoolRender()			const { return m_bRender; }
	const STATUS& GetStatus()			const { return m_tStatus; }
	const float	GetAttack()				const { return m_tCharActiveInfo.fAttack; }

public:
	bool Equip(CItem* pItem);
	bool Equip(CItem* pItem, EQUIP_SLOT eSlot);
	void Unequip(EQUIP_SLOT eSlot);
	void WeaponSwapOn() { m_bWeaponChange = true; }
	void GainExp(int iExp)		{ m_iExp += iExp; }
	void LevelUp()		{ ++m_iLevel; m_iExp = 0; }
	void Recovery()		{ m_tCharActiveInfo.iHP = m_tCharActiveInfo.iHPMax; }
	void Resetting();
	void ActiveInfoUpdate();


public:
	const vector<CItem*>& GetVecEquip() const { return m_vecEquip; }

private:
	DWORD CurrentMotion();
	HDC	 MotionDC();
	bool Direction();
	void EquipUpdate();
	void EquipRender(HDC hDC);
	void MoveLeft();
	void MoveRight();
	void Idle();
	void MakeDust();
	void Attack();
	void Die();
	void Dash();
	void Dashing();
	void DashCool();
	void LongJump();
	void KeyInput();
	void Blinking();
	void DoubleJump();
	
public:
	virtual void Stop();
	virtual void Jump();
	virtual void Hit(const float fAttack, const INFO& tAttackerInfo);
	virtual void AddAttackEffect(float fx, float fy);

public:		//	스탯 관련 함수
	void WrathUp();
	void SwiftnessUp();
	void PatienceUp();
	void MysticUp();
	void GreedUp();
	void StatusReset();
	
	const int GetUsedPoint();

};

