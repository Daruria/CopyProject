#pragma once
#include "Monster.h"

enum SKELL_BOSS_MOTION { SBM_IDLE, SBM_ATTACK, SBM_LASER, SBM_SWORD, SBM_DIE, SBM_DIE2, SBM_END };

class CBossSkelHand;
class CBGImage;
class CBossSkel :
	public CMonster
{
public:
	CBossSkel();
	virtual ~CBossSkel();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;

public:
	virtual void Attack();
	virtual void Idle();
	virtual void Hit(const float fAttack, const INFO& tAttackerInfo);
	virtual void AddAttackEffect(float fx, float fy);

public:
	virtual void Dying(float fx, float fy);

public:
	void ShootLaser();
	void SwordAttack();
	void HandPosition();
	void BackImageSetting();

private:
	HDC MotionDC();
	void ShootBall(float fAngle);
	void AddParticle();

private:
	bool		m_bStartPattern;
	bool		m_bLaser;
	bool		m_bSword;
	bool		m_bShootHand;	//	True : ¿À¸¥¼Õ, False : ¿Þ¼Õ
	float		m_fAngle;
	DWORD		m_dwPatternTime;
	DWORD		m_dwPatternMaxTime;
	DWORD		m_dwBallTime;
	DWORD		m_dwSwordTime;
	DWORD		m_dwDieTime;
	int			m_iPrevPattern;
	int			m_iLaserCount;
	int			m_iSwordCount;
	int			m_iDieEffectDist;
	CBossSkelHand* m_pRightHand;
	CBossSkelHand* m_pLeftHand;
	CBossSkelHand* m_pShootingHand;
	CBGImage*	m_pBackImage;

public:
	void SetStartPattern(bool bStartPattern) { m_bStartPattern = bStartPattern; }
};