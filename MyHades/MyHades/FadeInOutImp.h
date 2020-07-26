#pragma once
#include "EffectImp.h"
class CFadeInOutImp :
	public CEffectImp
{
private:
	enum STATE { FADE_IN, SHOW, FADE_OUT, END};

private:
	CFadeInOutImp();
public:
	virtual ~CFadeInOutImp();

public:// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CFadeInOutImp* Create(const wstring& wstrStateKey, float fFadeInTime, float fShowTime, float fFadeOutTime, D3DCOLOR d3dColor);

private:
	float m_fTimeCount;
	float m_fFadeInTime;
	float m_fShowTime;
	float m_fFadeOutTime;

	CFadeInOutImp::STATE m_eCurState;
	
	float m_fAlpha;
	D3DCOLOR m_d3dColor;
};

