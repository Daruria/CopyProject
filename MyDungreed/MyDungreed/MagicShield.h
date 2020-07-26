#pragma once
#include "Shield.h"
class CMagicShield :
	public CShield
{
public:
	CMagicShield();
	virtual ~CMagicShield();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void DiscRender(HDC hDC, float fx, float fy);

public:
	void SetArmor(float fArmor) { m_fArmor = fArmor; }

public:
	const float GetArmor() const { return m_fArmor; }

private:
	bool	m_bDir;
	float	m_fArmor;
	POSITION m_tOffset;
};

