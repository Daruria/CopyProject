#pragma once
#include "GameObject.h"

class CEffectImp;
class CBridgeEffect :
	public CGameObject
{
protected:
	explicit CBridgeEffect(CEffectImp* pImp);

public:
	virtual ~CBridgeEffect();

protected:
	virtual void Release();

public:
	virtual float GetLayerStd() override { return 0.f; };

public:
	CEffectImp* GetImp() { return m_pImp; }

protected:
	CEffectImp* m_pImp;
	CObjectMgr*	m_pObjectMgr;
};

