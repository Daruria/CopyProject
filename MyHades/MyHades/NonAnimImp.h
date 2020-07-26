#pragma once
#include "EffectImp.h"
class CNonAnimImp :
	public CEffectImp
{
public:
	CNonAnimImp();
	virtual ~CNonAnimImp();

public:// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CNonAnimImp* Create(const wstring& wstrStateKey, float fLifeTime, D3DCOLOR d3dColor);

private:
	float m_fTimeCount;
	float m_fLifeTime;
	D3DCOLOR m_d3dColor;
};

