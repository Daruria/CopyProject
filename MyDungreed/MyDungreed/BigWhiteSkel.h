#pragma once
#include "Monster.h"

enum BWS_MOTION {BWS_IDLE, BWS_ATTACK, BWS_MOVE, BWS_DIE, BWS_END};
class CBigWhiteSkel :
	public CMonster
{
public:
	CBigWhiteSkel();
	virtual ~CBigWhiteSkel();

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
	void MoveLeft();
	void MoveRight();

private:
	bool			m_bCanChangeAction;
	DWORD			m_dwActionTime;
	DWORD			m_dwActionChangeTime;
	vector<INFO>	m_vecMotionInfo;

private:
	HDC MotionDC();

public:
	virtual void Dying(float fx, float fy);
};

