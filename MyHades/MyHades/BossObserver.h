#pragma once
#include "Observer.h"
class CBossObserver :
	public CObserver
{
public:
	enum MSG_TYPE { HP, MAX_HP, ATK, NAME, DATA_END };

public:
	CBossObserver();
	virtual ~CBossObserver();

public:	// Inherited via CObserver
	virtual void Update(int iMsg, void * pData) override;

public:
	static CBossObserver* Create() { return new CBossObserver; }

public:
	const int GetBossHp()		const { return m_tStatus.iHp; }
	const int GetBossMaxHp()	const { return m_tStatus.iMaxHp; }
	const int GetBossAtk()		const { return m_tStatus.iAttack; }
	const wstring& GetNameKey() const { return m_wstrNameKey; }

private:
	STATUS m_tStatus;
	wstring m_wstrNameKey;
};

