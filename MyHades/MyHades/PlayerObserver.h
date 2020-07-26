#pragma once
#include "Observer.h"
class CPlayerObserver :
	public CObserver
{
public:
	enum MSG_TYPE {HP, MAX_HP, ATK, WRATH_POINT, WRATH_MAX_POINT, REFLECT_SKILL, DASH_SKILL, CALL, DATA_END };

private:
	CPlayerObserver();
public:
	virtual ~CPlayerObserver();
	
public:	// Inherited via CObserver
	virtual void Update(int iMsg, void * pData) override;

public:
	static CPlayerObserver* Create() { return new CPlayerObserver; }

public:
	const int GetPlayerHp()			const { return m_tStatus.iHp; }
	const int GetPlayerMaxHp()		const { return m_tStatus.iMaxHp; }
	const int GetPlayerAtk()		const { return m_tStatus.iAttack; }
	const int GetWrathPoint()		const { return m_iWrathPoint; }
	const int GetWrathMaxPoint()	const { return m_iWrathMaxPoint; }
	const bool GetCanReflectSkill()	const { return m_bCanReflectSkill; }
	const bool GetCanDashSkill()	const { return m_bCanDashSkill; }
	const bool GetCanCall()			const { return m_bCanCall; }

private:
	STATUS	m_tStatus;

	int		m_iWrathPoint;
	int		m_iWrathMaxPoint;

	bool	m_bCanReflectSkill;
	bool	m_bCanDashSkill;
	bool	m_bCanCall;
};

