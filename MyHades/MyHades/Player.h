#pragma once
#include "MoveObject.h"
class CPlayer :
	public CMoveObject
{
public:
	enum STATE 
	{
		IDLE,
		RUN,
		DASH,
		ATTACK,
		DASH_ATTACK,
		PARRY,
		RETURN_TO_IDLE,
		ON_HIT,
		DEAD_WALK,
		END
	};

	enum AUTO_STATE { A_IDLE, A_RUN, A_DASH, A_ATTACK, A_PARRY, A_CALL, AUTO_END };

public:
	CPlayer();
	virtual ~CPlayer();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CPlayer)

private:
	virtual void Animation();

public:
	const bool GetDeadEffect() const { return m_bDeadEffect; }
	const CPlayer::STATE GetState() const { return m_eCurState; }

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack) override;
	virtual void Die();
	void Reset();
	void SetCanKeyInput(bool bCanKeyInput)	{ m_bCanKeyInput = bCanKeyInput; }
	void SetState(CPlayer::STATE eState)	{ m_eCurState = eState; }
	void SetRender(bool bRender)			{ m_bRender = bRender; }
	void SetCanDashSkill(bool bDashSkill);
	void SetCanReflectSkill(bool bReflectSkill);
	void SetCanCall(bool bCanCall);


private:
	void KeyInput();
	void KeyInputMove();
	void KeyInputAttack();
	void KeyInputDash();
	void KeyInputDashAttack();
	void KeyInputParry();
	void KeyInputCall();
	void AutoKeyInput();

private:
	void RunAction();
	void AttackAction(const D3DXVECTOR3& vClickPos);
	void DashAction();
	void DashAttackAction();
	void ParryAction();
	void CallAction();


private:
	void AutoAttack(const D3DXVECTOR3& vNearest);
	void AutoMove(float fDist, const D3DXVECTOR3& vNearestDir);
	void AutoMoveAstar(const D3DXVECTOR3& vNearest);
	void AutoDash(const D3DXVECTOR3& vNearest);
	void AutoDashAttack(float fDist);
	void AutoParry(float fDist);
	void AutoCall(float fDist);
	const D3DXVECTOR3 AstarNearPos(const D3DXVECTOR3& vPos);

private:
	//	플레이어 안에서만 사용할 함수
	void StateUpdate();
	void AttackUpdate();
	void RunUpdate();
	void DashUpdate();
	void DashAttackUpdate();
	void ParryUpdate();
	void OnHitUpdate();
	void ChangeState();
	void HitCoolDown();
	void DeadEffect();
	int AngleToDir();
	float RadianForAngle3(int iDir);
	void DeadUpdate();
	void WrathFill();
	void Calling();
	void Bleeding(const D3DXVECTOR3& vPos);
	void AutoEffect();
	void CallEffect();

private:	//	상태 프레임 설정 함수
	void IdleState();
	void RunState();
	void DashState();
	void AttackState();
	void DashAttackState();
	void ParryState();
	void ReturnToIdleState();
	void OnHitState();
	void DeadWalkState();

private:
	wstring			m_wstrObjectKey;
	vector<wstring>	m_vecStateKey;
	vector<int>		m_vecDirCnt;
	vector<int>		m_vecStateMaxFrame;
	vector<D3DXVECTOR3> m_vecRenderOffset;

	STATE		m_ePreState;
	STATE		m_eCurState;

	float		m_fSpeed;

	bool		m_bCanKeyInput;
	bool		m_bCanAttack;
	bool		m_bIsEffect;
	int			m_iAttackCount;
	
	float		m_fDashSpeed;

	STATUS		m_tStatus;

	bool	m_bCanHit;
	float	m_fHitCoolDown;
	float	m_fHitCoolTime;

	bool	m_bNoDie;
	bool	m_bDeadEffect;
	bool	m_bRender;

	bool	m_bCanDashSkill;
	bool	m_bCanReflectSkill;
	bool	m_bCanCall;

	bool	m_bCalling;
	bool	m_bZeusCalled;

	int		m_iWrathPoint;
	int		m_iWrathMaxPoint;

	float	m_fFootStepTimeCount;
	float	m_fFootStepTime;

	bool	m_bAutoPlay;
	bool	m_bAttackInput;
	bool	m_bDashInput;
	bool	m_bDashAttackInput;
	bool	m_bParryInput;
	bool	m_bCallInput;

	CPlayer::AUTO_STATE m_eAutoState;

	bool	m_bAstarMoving;

private:
	CKeyMgr*	m_pKeyMgr;
	CAstarMgr*	m_pAstarMgr;
};

