#pragma once
#include "EffectImp.h"
class CAnimReverse :
	public CEffectImp
{
private:
	CAnimReverse();
public:
	virtual ~CAnimReverse();

public:	// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CAnimReverse* Create(const wstring& wstrObjectKey, const wstring& wstrStateKey, const FRAME& tFrame, D3DCOLOR d3dColor);

private:
	D3DCOLOR	m_d3dColor;
	FRAME		m_tFrame;
	wstring		m_wstrObjectKey;
};

