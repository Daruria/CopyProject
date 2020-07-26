#pragma once
#include "Monster.h"


enum GB_MOTION{GBM_IDLE, GBM_ATTACK, GBM_DIE, GBM_END};
class CGiantBat :
	public CMonster
{
public:
	CGiantBat();
	virtual ~CGiantBat();

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
	bool	m_bCanShoot;
	bool	m_bMotionChange;
	DWORD	m_dwMotionTime;		//	모션 시작 한 시간
	DWORD	m_dwChangeTime;		//	모션 지속 시간.
	int		m_iShootCount;
	float	m_fAngle;
	POSITION m_tMoveDir;


public:
	void SetAngle(float fAngle) { m_fAngle = fAngle; }

private:
	void	AI();
	void	Shoot(float fAngle);
	HDC MotionDC();


};

