#pragma once
#include "Monster.h"

enum BANSHEE_MOTION{BM_IDLE, BM_ATTACK, BM_END};
class CBanshee :
	public CMonster
{
public:
	CBanshee();
	virtual ~CBanshee();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;

public:
	virtual void Attack();
	virtual void Idle();
	virtual void Move();
	virtual void Die();
	virtual void Hit(const float fAttack, const INFO& tAttackerInfo);
	virtual void AddAttackEffect(float fx, float fy);

public:
	virtual void Dying(float fx, float fy);


private:
	bool		m_bCanShoot;
	bool		m_bActionChange;
	DWORD		m_dwActionTime;
	DWORD		m_dwActionCool;
	POSITION	m_tMoveDir;

private:
	HDC MotionDC();
	void AI();
	void Shoot(float fAngle);
};

