#pragma once
#include "Obj.h"

#define FALLING_PHYSICS 0x01;
#define FALLING_FALL	0x02;
#define FALLING_JUMP	0x04;

class CMoveObj :
	public CObj
{
public:
	CMoveObj();
	virtual ~CMoveObj();

protected:
	bool	m_bJump;
	bool	m_bPhysics;
	bool	m_bFalling;
	bool	m_bPassible;
	float	m_fSpeed;
	float	m_fGravityTime;
	float	m_fJumpForce;

protected:
	DWORD 	m_dwMotion;			//	현재 모션 enum
	float	m_fRatio;			//	이미지 출력 비율.
	int		m_iFrame;			//	현재 프레임
	int		m_iFrameDir;		//	애니메이션 진행 방향 (1 or -1)
	int		m_iMaxFrame;		//	애니메이션이 최대 몇프레임인가
	DWORD	m_dwFrameTime;		//	현재 프레임의 시간.
	DWORD	m_dwFrameMaxTime;	//	프레임과 프레임 사이의 시간

protected:
	bool		m_bAttack;				//	AttackBox 사용 유무
	short		m_sAttackLife;			//	공격 후 지난 프레임 수
	short		m_sAttackBoxLifeTime;	//	공격 박스 수명
	POSITION	m_tPrevPos;
	INFO		m_tHitBox;				//	INFO 기준 상대 위치 피격용
	INFO		m_tAttackBox;			//	공격용
	string		m_strKey;

public:
	void SetJumpForce(float fJumpForce)		{ m_fJumpForce = fJumpForce; }
	void SetFrame(const int iFrame)			{ m_iFrame = iFrame; }
	void SetMaxFrame(const int iMaxFrame)	{ m_iMaxFrame = iMaxFrame; }
	void SetMotion(const DWORD dwMotion)	{ m_dwMotion = dwMotion; }
	void SetRatio(const float fRatio)		{ m_fRatio = fRatio; }
	void SetSpeed(const float fSpeed)		{ m_fSpeed = fSpeed; }
	void SetPrevPos(const POSITION& tPos)	{ m_tPrevPos.fx = tPos.fx; m_tPrevPos.fy = tPos.fy; }
	void SetPrevPos(const INFO& tInfo)		{ m_tPrevPos.fx = tInfo.fx; m_tPrevPos.fy = tInfo.fy; }
	void SetKey(const string& strKey)		{ m_strKey = strKey; }
	void SetAttacking(bool bAttack)			{ m_bAttack = bAttack; }
	void SetHitBox(const float fx, const float fy, const int icx, const int icy);

public:
	const int GetFrame()	const { return m_iFrame; }
	const int GetMaxFrame() const { return m_iMaxFrame; }
	const DWORD GetMotion() const { return m_dwMotion; }
	const float GetRatio()	const { return m_fRatio; }
	const float GetSpeed()	const { return m_fSpeed; }
	const POSITION& GetPrevPos() const { return m_tPrevPos; }
	const string& GetKey()	const { return m_strKey; }
	const bool GetPassible() const { return m_bPassible; }
	const bool GetPhysics() const { return m_bPhysics; }
	const bool GetFalling() const { return m_bFalling; }
	const bool GetJump()	const { return m_bJump; }
	const INFO& GetHitBox() const { return m_tHitBox; }
	const INFO GetHitBoxWorldInfo();
	const INFO GetHitBoxPrevIWorldInfo();
	const int GetPushDistTop();
	const int GetPushDistBottom();
	const int GetPushDistLeft();
	const int GetPushDistRight();
	const bool GetAttacking() const { return m_bAttack; }
	const INFO& GetAttackBox() const { return m_tAttackBox; }

public:
	virtual void Stop() { m_bJump = false; m_bFalling = false; m_fGravityTime = 0.f; }
	void BangingHead() { m_bJump = false; m_fGravityTime = 0.f; }
	virtual void Jump();
	virtual void Hit(const float fAttack, const INFO& tAttackerInfo);
	void QuitAttack() { m_bAttack = false; }
	virtual void Gravity();
	virtual void Dying(float fx, float fy);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

