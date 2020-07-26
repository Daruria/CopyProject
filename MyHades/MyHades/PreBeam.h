#pragma once
#include "Monster.h"
class CPreBeam :
	public CMonster
{
private:
	CPreBeam();
public:
	virtual ~CPreBeam();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CPreBeam* Create(CMonster* pParent);

private:
	CMonster* m_pParentMon;
};

