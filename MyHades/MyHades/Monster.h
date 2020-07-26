#pragma once
#include "MoveObject.h"
class CMonster :
	public CMoveObject
{
protected:
	CMonster();
public:
	virtual ~CMonster();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack) override;

protected:
	virtual void KnockBack(const D3DXVECTOR3& vHitterPos) {}

public:
	virtual void Die() { m_bIsDead = true; }

public:
	const STATUS& GetStatus() const { return m_tStatus; }

protected:
	bool		m_bIsDead;
	wstring		m_wstrObjectKey;
	STATUS		m_tStatus;
};