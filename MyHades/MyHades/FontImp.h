#pragma once
#include "EffectImp.h"
class CFontImp :
	public CEffectImp
{
public:
	CFontImp();
	virtual ~CFontImp();

public:// Inherited via CEffectImp
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pMatrix) override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CFontImp* Create(const wstring& wstrText, const D3DXVECTOR3& vSize, float fLifeTime, D3DCOLOR d3dColor);

private:
	wstring m_wstrText;
	float	m_fLifeTime;
	float	m_fTimeCount;
	D3DXVECTOR3 m_vSize;
	D3DCOLOR m_d3dColor;

	CFontMgr* m_pFontMgr;
};

