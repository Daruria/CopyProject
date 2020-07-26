#pragma once
#include "Effect.h"
class CRevolutionEffect :
	public CEffect
{
protected:
	CRevolutionEffect();
public:
	virtual ~CRevolutionEffect();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CRevolutionEffect* Create(const D3DXVECTOR3& vPos);

private:
	CGameObject* m_pParent;

	float	m_fRadius;
};

