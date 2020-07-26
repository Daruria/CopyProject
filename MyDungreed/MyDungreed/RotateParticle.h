#pragma once
#include "MoveObj.h"
class CRotateParticle :
	public CMoveObj
{
public:
	CRotateParticle();
	virtual ~CRotateParticle();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	POSITION	m_tDir;
	float		m_fAngle;	//	Radians
	int			m_iDir;
	float		m_fTorque;
	float		m_fForce;

public:
	void SetDirection(float  fDirX, float fDirY) { m_tDir.fx = fDirX; m_tDir.fy = fDirY + 0.1f; }
	void SetDir(bool bDir)	{ m_iDir = bDir ? 1 : -1; }
	void SetImage(const string& strKey, const wchar_t* pPath);

public:
	virtual void Stop();

};

