#pragma once
#include "EffectImp.h"
class CAnimForwardReverseImp :
	public CEffectImp
{
public:
	enum STATE {FORWARD, STOP, REVERSE, END};

private:
	CAnimForwardReverseImp();
public:
	virtual ~CAnimForwardReverseImp();

public:// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CAnimForwardReverseImp* Create(const wstring& wstrObjectKey, const wstring& wstrStateKey, const FRAME& tFrame, float fStopTime, D3DCOLOR d3dColor);

private:
	int StateUpdate();

	int ForwardUpdate();
	int StopUpdate();
	int ReverseUpdate();

private:
	D3DCOLOR	m_d3dColor;
	FRAME		m_tFrame;
	wstring		m_wstrObjectKey;

	float		m_fStopTime;
	float		m_fStopTimeCount;

	CAnimForwardReverseImp::STATE m_eCurState;
};

