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
	DWORD 	m_dwMotion;			//	���� ��� enum
	float	m_fRatio;			//	�̹��� ��� ����.
	int		m_iFrame;			//	���� ������
	int		m_iFrameDir;		//	�ִϸ��̼� ���� ���� (1 or -1)
	int		m_iMaxFrame;		//	�ִϸ��̼��� �ִ� ���������ΰ�
	DWORD	m_dwFrameTime;		//	���� �������� �ð�.
	DWORD	m_dwFrameMaxTime;	//	�����Ӱ� ������ ������ �ð�

protected:
	bool		m_bAttack;				//	AttackBox ��� ����
	short		m_sAttackLife;			//	���� �� ���� ������ ��
	short		m_sAttackBoxLifeTime;	//	���� �ڽ� ����
	POSITION	m_tPrevPos;
	INFO		m_tHitBox;				//	INFO ���� ��� ��ġ �ǰݿ�
	INFO		m_tAttackBox;			//	���ݿ�
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

