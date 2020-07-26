#pragma once
#include "BridgeEffect.h"
class CEffectImp;
class CMoveEffect :
	public CBridgeEffect
{
private:
	CMoveEffect(CEffectImp* pImp);
public:
	virtual ~CMoveEffect();

public:// Inherited via CBridgeEffect
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual void Die() override {};

public:
	static CMoveEffect* Create(CEffectImp* pImp, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, float fSpeed, RENDER_TYPE eRenderType);

private:
	float m_fSpeed;

};

