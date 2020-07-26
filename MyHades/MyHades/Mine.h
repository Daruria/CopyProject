#pragma once
#include "Monster.h"
class CMine :
	public CMonster
{
public:
	enum STATE {IDLE, READY, FIRE, DIE, END};


protected:
	CMine();
public:
	virtual ~CMine();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMine* Create(const D3DXVECTOR3& vPos);

private:
	void ChangeState();

private:
	void StateUpdate();
	void IdleUpdate();
	void ReadyUpdate();
	void FireUpdate();
	void DieUpdate();

private:
	CMine::STATE m_ePreState;
	CMine::STATE m_eCurState;

	bool	m_bLightRender;
	float	m_fLightCount;
	float	m_fLightTime;

	float	m_fReadyCount;
	float	m_fReadyTime;

};

