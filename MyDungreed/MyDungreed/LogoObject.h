#pragma once
#include "MoveObj.h"
class CLogoObject :
	public CMoveObj
{
public:
	CLogoObject();
	virtual ~CLogoObject();

private:
	float m_fDirAngle;	//	-PI ~ PI Radians

public:
	void SetDirAngle(const float fDirAngle) { m_fDirAngle = fDirAngle; }

public:
	const float GetDirAngle() { return m_fDirAngle; }

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

