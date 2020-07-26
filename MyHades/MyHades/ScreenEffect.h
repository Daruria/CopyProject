#pragma once
#include "BridgeEffect.h"
class CEffectImp;
class CScreenEffect :
	public CBridgeEffect
{
public:
	explicit CScreenEffect(CEffectImp* pImp);
	virtual ~CScreenEffect();

public:// Inherited via CBridgeEffect
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual float GetLayerStd() override { return m_fSort; };
	virtual void Die() override {};

public:
	static CScreenEffect* Create(CEffectImp* pImp, const D3DXVECTOR3& vPos, RENDER_TYPE eRenderType, float fSort = 0.f);
	static CScreenEffect* Create(CEffectImp* pImp, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, RENDER_TYPE eRenderType, float fSort = 0.f);

private:
	float	m_fSort;
};

