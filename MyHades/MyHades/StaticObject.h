#pragma once
#include "GameObject.h"
class CStaticObject :
	public CGameObject
{
public:
	CStaticObject();
	virtual ~CStaticObject();

public:	// Inherited via CGameObject
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

public:
	virtual void Die() override { };

public:
	virtual float GetLayerStd() override { return m_tInfo.vPos.y; }

protected:
	CObjectMgr* m_pObjectMgr;
};

