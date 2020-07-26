#pragma once
#include "HitBox.h"
class CRotatedHitBox :
	public CHitBox
{
protected:
	CRotatedHitBox();
public:
	virtual ~CRotatedHitBox();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CRotatedHitBox* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vSize, CGameObject* pParent, float fLifeTime);

private:
	float m_fAngle;
};

