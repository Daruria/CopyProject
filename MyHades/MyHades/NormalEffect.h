#pragma once
#include "BridgeEffect.h"
class CEffectImp;
class CNormalEffect :
	public CBridgeEffect
{
public:
	CNormalEffect(CEffectImp* pImp);
	virtual ~CNormalEffect();

public:// Inherited via CBridgeEffect
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void Die() override;

public:
	static CNormalEffect* Create(CEffectImp* pImp, const D3DXVECTOR3& vPos, RENDER_TYPE eRenderType);
	static CNormalEffect* Create(CEffectImp* pImp, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, RENDER_TYPE eRenderType);
	static CNormalEffect* Create(CEffectImp* pImp, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, float fRadian, RENDER_TYPE eRenderType);

private:
	float m_fAngle;
};

