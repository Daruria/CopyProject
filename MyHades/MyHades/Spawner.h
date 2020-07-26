#pragma once
#include "Monster.h"
class CSpawner :
	public CMonster
{
public:
	enum STATE { APEAR, IDLE, SPAWN, DIE, END};

protected:
	CSpawner();
public:
	virtual ~CSpawner();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CSpawner* Create(const D3DXVECTOR3 vPos);

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack) override;

private:
	void ChangeState();
	void ApearState();
	void IdleState();
	void SpawnState();
	void DieState();

private:
	void StateUpdate();
	void ApearUpdate();
	void IdleUpdate();
	void SpawnUpdate();
	void DieUpdate();

private:
	CSpawner::STATE m_ePreState;
	CSpawner::STATE m_eCurState;

	wstring m_wstrStateKey;
	vector <float> m_vecStartFrame;

	bool m_bSpawn;
};