#pragma once
#include "Monster.h"
class CScene;
class CBossUI;
class CAlecto :
	public CMonster
{
private:
	enum STATE { STATE_IDLE, STATE_RUN, STATE_LUNGE, STATE_BOOM, STATE_WHIP_WHIRL, DIE, STATE_END };
	enum PATTERN { PATTERN_LUNGE, PATTERN_BOOM, PATTERN_WHIP_WHIRL, PATTERN_END };
	
private:
	CAlecto();
public:
	virtual ~CAlecto();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CAlecto* Create(const D3DXVECTOR3& vPos, CScene* pScene);

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack) override;

public:
	void Start();

private:
	void ChangeState();
	void IdleState();
	void RunState();
	void LungeState();
	void BoomState();
	void WhipWhirlState();
	void DieState();

private:
	void StateUpdate();
	void IdleUpdate();
	void RunUpdate();
	void LungeUpdate();
	void BoomUpdate();
	void WhipWhirlUpdate();
	void DieUpdate();

private:
	int GetDir();

private:
	CAlecto::STATE m_ePreState;
	CAlecto::STATE m_eCurState;

	CAlecto::PATTERN m_eCurPattern;

	wstring m_wstrStateKey;

	bool	m_bPlayStart;
	bool	m_bAddHitBox;

	float	m_fIdleCount;
	float	m_fIdleTime;

	float	m_fSpeed;

	CScene* m_pScene;
	CBossUI* m_pBossUI;
};

