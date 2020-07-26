#pragma once
#include "MoveObj.h"

enum BULLET_TYPE { BT_ARROW, BT_BALL, BT_NOTE, BT_BOSSBALL, BT_GUN, BT_GUIDED_MISSILE, BT_END};
class CBullet :
	public CMoveObj
{
public:
	CBullet();
	virtual ~CBullet();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void Dying(float fx, float fy);

protected:
	float		m_fAngle;
	float		m_fAttack;
	float		m_fRangeDist;
	POSITION	m_tStartPos;
	BULLET_TYPE m_eType;
	string		m_strName;

public:
	void SetAttack(float fAttack)			{ m_fAttack = fAttack; }
	void SetAngle(float fAngle)				{ m_fAngle = fAngle; }
	void SetBulletType(BULLET_TYPE eType)	{ m_eType = eType; }
	void SetImage(const string& strKey, const wchar_t* pPath);
	void SetStartPos()						{ m_tStartPos = {m_tInfo.fx, m_tInfo.fy}; }
	void SetRangeDist(const float fDist)	{ m_fRangeDist = fDist; }
	void SetName(const string& strName)		{ m_strName = strName; }

public:
	const float GetAttack()				const { return m_fAttack; }
	const BULLET_TYPE GetBulletType()	const { return m_eType; }
	const string& GetName()				const { return m_strName; }
};

