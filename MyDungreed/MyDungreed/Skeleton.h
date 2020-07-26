#pragma once
#include "Monster.h"

enum SKEL_TYPE	{ ST_SHORT, ST_GREAT, ST_BOW, ST_END };
enum SKEL_MOTION{ SM_IDLE, SM_WALK, SM_ATTACK, SM_DIE, SM_END };
class CSkeleton :
	public CMonster
{
public:
	CSkeleton();
	virtual ~CSkeleton();

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
	bool		m_bCanAttack;
	float		m_fAngle;
	SKEL_TYPE	m_eSkelType;
	INFO		m_tWeaponOffSet;
	DWORD		m_dwAttackCool;
	vector<string> m_vecFrameKey;

private:
	HDC MotionDC();
	HDC WeaponDC();
	void AI();

public:
	void SetSkelType(SKEL_TYPE eSkelType);
};

