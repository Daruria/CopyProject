#pragma once
#include "Monster.h"
class CCrystal :
	public CMonster
{
public:
	enum STATE {IDLE, MOVE, ATTACK_CHARGING, ATTACK_FIRE, DIE, END};

private:
	CCrystal();
public:
	virtual ~CCrystal();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CCrystal* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale);

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack) override;

protected:
	virtual void KnockBack(const D3DXVECTOR3& vHitterPos);

private:	//	상태별 업데이트
	void StateUpdate();
	void IdleUpdate();
	void MoveUpdate();
	void AttackUpdate();
	void DieUpdate();
	

private:	//	상태 변화 함수
	void ChangeState();
	void IdleState();
	void MoveState();
	void AttackState();
	void DieState();

public:
	CMonster* GetBeam() { return m_pBeam; }
	void SetBeam(CMonster* pBeam) { m_pBeam = pBeam; }

private:
	CCrystal::STATE m_ePreState;
	CCrystal::STATE m_eCurState;

	vector<wstring> m_vecStateKey;

	float	m_fSpeed;
	float	m_fAngle;

	bool	m_bIsCharging;
	float	m_fCharingTime;
	bool	m_bIsFiring;
	float	m_fFireTime;
	float	m_fFireRotateSpeed;

	CMonster*	m_pBeam;
};

