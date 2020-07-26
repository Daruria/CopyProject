#pragma once
#include "MoveObj.h"
class CMonster :
	public CMoveObj
{
public:
	CMonster();
	virtual	~CMonster();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;

protected:
	bool			m_bDir;			//	True : Right, False : Left
	CHARACTER_INFO	m_tCharInfo;
	string			m_strName;

public:
	void SetCharInfo(const CHARACTER_INFO& tCharInfo);
	void SetCharInfo(int iHp, int iHpMax, float fAttack, float fArmor);
	void SetName(const string& strName) { m_strName = strName; }

public:
	const CHARACTER_INFO& GetCharInfo() const { return m_tCharInfo; }
	const string& GetName()				const { return m_strName; }
	
public:
	virtual void Attack();
	virtual void Idle();
	virtual void Move();
	virtual void Die();
	virtual void Hit(const float fAttack, const INFO& tAttackerInfo);
	virtual void AddAttackEffect(float fx, float fy);

public:
	virtual void Dying(float fx, float fy);

protected:
	void AddGold(const int iGold);
	void AddParticle(const string& strKey, const wchar_t* pPath, float fDirX, float fDirY, float fRatio = 1.f, float fFriction = 0.9f, float fFallingSpeed = GRAVITY, float fJumpForce = -1.f, bool bBounce = true);
	void AddRotateParticle(const string& strKey, const wchar_t* pPath, float fDirX, float fDirY);
};

