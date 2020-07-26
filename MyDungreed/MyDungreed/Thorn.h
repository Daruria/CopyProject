#pragma once
#include "FieldObject.h"
class CThorn :
	public CFieldObject
{
public:
	CThorn();
	virtual ~CThorn();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	float	m_fAttack;
	int		m_iThornCount;

public:
	void SetAttack(float fAttack)		{ m_fAttack = fAttack; }
	void SetThornCount(int iThornCount);

public:
	const float GetAttack() const { return m_fAttack; }
};

