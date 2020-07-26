#pragma once
#include "GameObject.h"
class CHitBox :
	public CGameObject
{
public:
	enum HIT_BOX_TYPE{ HIT_BOX_RECT, HIT_BOX_LINE, HIT_BOX_END };

public:
	CHitBox();
	virtual ~CHitBox();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual float GetLayerStd() override;
	
public:
	virtual void Hit();
	virtual void Hit(const D3DXVECTOR3& vPos);
	virtual void Die() { m_fTimeCount = m_fLifeTime; m_pParent = nullptr; }

public:
	const int GetAttack() const { return m_iAttack; }
	const HIT_BOX_TYPE GetBoxType() const { return m_eHitBoxType; }

public:
	const RECT GetRect() const;

protected:
	float	m_fTimeCount;
	float	m_fLifeTime;
	int		m_iAttack;

	D3DXVECTOR3 m_vSize;
	CGameObject* m_pParent;

	HIT_BOX_TYPE m_eHitBoxType;

	CObjectMgr*		m_pObjectMgr;

};

