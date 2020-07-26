#pragma once
#include "HitBox.h"
class CLineHitBox :
	public CHitBox
{
protected:
	CLineHitBox();

public:
	virtual ~CLineHitBox();


public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void Hit();
	virtual void Hit(const D3DXVECTOR3& vPos);
	virtual void Die() { m_fTimeCount = m_fLifeTime; m_pParent = nullptr; }

public:
	static CLineHitBox* Create(const D3DXVECTOR3& vPos, float fLength, float fAngle, int iAttack, float fLifeTime, CGameObject* pParent);

public:
	LINE GetLine();

private:
	bool	m_bPreFrameColl;
	float	m_fLengthOrigin;
	float	m_fLength;
	float	m_fAngle;

	
};

