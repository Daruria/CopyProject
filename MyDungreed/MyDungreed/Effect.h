#pragma once

#include "Obj.h"

class CEffect : public CObj
{
public:
	CEffect();
	virtual ~CEffect();

protected:
	bool	m_bLife;
	float	m_fRatio;

public:
	const bool	IsDie()	const { return !m_bLife; }

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetRatio(float fRatio) { m_fRatio = fRatio; }
};

