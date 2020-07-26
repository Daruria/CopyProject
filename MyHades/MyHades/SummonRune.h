#pragma once
#include "Effect.h"
class CMonster;
class CSummonRune :
	public CEffect
{
protected:
	CSummonRune();
public:
	virtual ~CSummonRune();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CSummonRune* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, CMonster* pMonster);

private:
	bool		m_bDidMonsterPush;
	CMonster*	m_pMonster;

};