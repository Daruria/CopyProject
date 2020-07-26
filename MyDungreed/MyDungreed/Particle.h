#pragma once

#include "MoveObj.h"
class CParticle : public CMoveObj
{
public:
	CParticle();
	virtual ~CParticle();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	bool		m_bBounce;
	float		m_fMagnitude;
	float		m_fDirX;
	float		m_fDirY;
	float		m_fFriction;
	float		m_fFallingSpeed;
	POSITION	m_tDrawStartPos;

public:
	virtual void Gravity();

public:
	void SetBounce(bool bBounce)				{ m_bBounce = bBounce; }
	void SetDrawStartPos(float fx, float fy)	{ m_tDrawStartPos.fx = fx; m_tDrawStartPos.fy = fy; }
	void SetDirection(float  fDirX, float fDirY){ m_fDirX = fDirX; m_fDirY = fDirY  + 0.1f; }
	void SetFriction(float fFriction)			{ m_fFriction = fFriction; }
	void SetFallingSpeed(float fFallingSpeed)	{ m_fFallingSpeed = fFallingSpeed; }

public:
	virtual void Stop();

};

