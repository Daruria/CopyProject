#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
protected:
	CEffect();

public:
	virtual ~CEffect();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual float GetLayerStd() override { return m_tInfo.vPos.y; }

public:
	static CEffect* Create(const INFO& tInfo, const wstring& wstrObjectKey, const wstring& wstrStateKey, const WORD& wMaxFrame, float fFrameSpeed, ANIM_TYPE eAnimationType, D3DCOLOR d3dColor);
	static CEffect* Create(const INFO& tInfo, const wstring& wstrObjectKey, const wstring& wstrStateKey, const WORD& wMaxFrame, float fFrameSpeed, float fLifeTime, ANIM_TYPE eAnimationType, D3DCOLOR d3dColor);
	static CEffect* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, float fAngle, const wstring& wstrObjectKey, const wstring& wstrStateKey, const WORD& wMaxFrame, float fFrameSpeed, ANIM_TYPE eAnimationType, D3DCOLOR d3dColor);

public:
	virtual void Die() { m_bIsDead = true; };

protected:
	void Animation();

protected:
	bool		m_bIsDead;
	FRAME		m_tFrame;
	D3DCOLOR	m_d3dColor;
	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;
	float		m_fAngle;

	float	m_fTimeCount;
	float	m_fLifeTime;

protected:
	CObjectMgr*	m_pObjectMgr;
};

