#pragma once
#include "Monster.h"

class CLineHitBox;
class CRevolutionEffect;
class CBeam :
	public CMonster
{
public:
	CBeam();
	virtual ~CBeam();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBeam* Create(CMonster* pParent);

public:
	void Collision(float fLength);
	void SetLineHitBox(CLineHitBox* pLineHitBox) { m_pLineHitBox = pLineHitBox; }

private:
	CMonster* m_pParentMon;
	float	m_fTimeCount;
	float	m_fLifeTime;
	CLineHitBox*		m_pLineHitBox;
	CRevolutionEffect*	m_pCloseEffect;
	CRevolutionEffect*	m_pFarEffect;

	bool	m_bPreFrameColl;
	float	m_fLengthRatio;
};

