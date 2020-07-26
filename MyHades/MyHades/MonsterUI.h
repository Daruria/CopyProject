#pragma once
#include "UI.h"

class CMonsterObserver;
class CMonsterUI :
	public CUI
{
private:
	CMonsterUI();
public:
	virtual ~CMonsterUI();

public:
	// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMonsterUI* Create();

private:
	CMonsterObserver* m_pObserver;
	
};

