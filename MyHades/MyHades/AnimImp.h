#pragma once
#include "EffectImp.h"
class CAnimImp :
	public CEffectImp
{
private:
	CAnimImp();
public:
	virtual ~CAnimImp();

public:	// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CAnimImp* Create(const wstring& wstrObjectKey, const wstring& wstrStateKey, const FRAME& tFrame, D3DCOLOR d3dColor);
	static CAnimImp* Create(const wstring& wstrObjectKey, const wstring& wstrStateKey, float fMaxFrame, float fFrameSpeed = 1/30.f, D3DCOLOR d3dColor = D3DCOLOR_ARGB(255, 255, 255, 255));

private:
	D3DCOLOR	m_d3dColor;
	FRAME		m_tFrame;
	wstring		m_wstrObjectKey;
};

