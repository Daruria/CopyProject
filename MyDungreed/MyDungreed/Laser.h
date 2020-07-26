#pragma once
#include "Monster.h"
class CLaser :
	public CMonster
{
public:
	CLaser();
	virtual ~CLaser();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;

public:
	virtual void AddAttackEffect(float fx, float fy);

public:
	void SetRightHand(float fx);
	void SetLeftHand(float fx);

private:
	int		m_iLaserLength;

};

