#pragma once
#include "GameObject.h"
class CUI :
	public CGameObject
{
protected:
	CUI();
public:
	virtual ~CUI();


public:
	// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual float GetLayerStd() override;
	virtual void Die() override;
	
protected:
	CObjectMgr*		m_pObjectMgr;
};

