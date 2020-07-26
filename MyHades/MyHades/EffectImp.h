#pragma once
class CEffectImp
{
protected:
	CEffectImp();
public:
	virtual ~CEffectImp();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(const D3DXMATRIX* pMatrix) PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

protected:
	CTimeMgr*		m_pTimeMgr;
	CTextureMgr*	m_pTextureMgr;
	CDeviceMgr*		m_pDeviceMgr;

	wstring			m_wstrStateKey;
};

