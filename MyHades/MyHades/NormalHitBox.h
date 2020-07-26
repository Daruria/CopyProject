#pragma once
#include "HitBox.h"
class CNormalHitBox :
	public CHitBox
{
private:
	CNormalHitBox();
public:
	virtual ~CNormalHitBox();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CNormalHitBox* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3 vSize, CGameObject* pParent, float fLifeTime, int iAttack);
	static CNormalHitBox* Create(const INFO& tInfo, const D3DXVECTOR3 vSize, CGameObject* pParent, float fLifeTime, float iAttack, float fSpeed = 0.f);

private:
	float	m_fSpeed;

};

