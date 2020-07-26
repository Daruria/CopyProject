#pragma once
#include "Monster.h"

enum SKELL_BOSS_HAND_PATTERN {SBHP_IDLE, SBHP_MOVE, SBHP_LASER, SBHP_END };
enum SKELL_BOSS_HAND_MOTION {SBHM_IDLE, SBHM_ATTACK, SBHM_END };
enum HAND_RIGHT_LEFT { HRL_RIGHT, HRL_LEFT, HRL_END };
class CBossSkelHand :
	public CMonster
{
public:
	CBossSkelHand();
	virtual ~CBossSkelHand();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;

public:
	virtual void Attack();
	virtual void Idle();

public:
	void MoveForShoot(float fy);

private:
	HDC MotionDC();
	void LaserShot();

private:
	bool			m_bChangePattern;
	bool			m_bMoveForShoot;
	bool			m_bCanShoot;
	bool			m_bLaserFinish;
	int				m_iMoveCurTime;
	int				m_iMoveTotalTime;
	float			m_fMovePosY;
	float			m_fMoveSpeed;
	HAND_RIGHT_LEFT m_eHandType;
	
public:
	void SetHandType(HAND_RIGHT_LEFT eHandType) { m_eHandType = eHandType; }
	void SetLaserFinish(bool bFinish) { m_bLaserFinish = bFinish; }

public:
	bool	AttackFinish() { return m_bLaserFinish; }
	void	Die() { m_bLife = false; }

};

