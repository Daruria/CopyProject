#pragma once
#include "Monster.h"
class CSwarmer :
	public CMonster
{
public:
	enum STATE { IDLE, ATTACK, DIE, END};
	enum ATTACK_STATE { JUMP, SHAKE, DASH, ATK_END };

protected:
	CSwarmer();

public:
	virtual ~CSwarmer();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CSwarmer* Create(const D3DXVECTOR3& vPos);

private:
	void ChangeState();
	void IdleState();
	void AttackState();
	void DieState();

private:
	void StateUpdate();
	void IdleUpdate();
	void AttackUpdate();
	void DieUpdate();

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack) override;
	virtual void Die() override;

private:
	wstring m_wstrStateKey;

	CSwarmer::STATE m_ePreState;
	CSwarmer::STATE m_eCurState;

	bool	m_bAttack;

	int		m_iDirCount;

	float	m_fSpeed;
	float	m_fEyeBlinkTime;
	float	m_fEyeBlinkTimeCount;

	CSwarmer::ATTACK_STATE m_eAtkPhase;
	float	m_fShake;
	float	m_fDashSpeed;
	float	m_fAttackCoolCount;
	float	m_fAttackCoolTime;

};

