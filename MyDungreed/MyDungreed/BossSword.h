#pragma once
#include "Monster.h"

enum BOSS_SWORD_STATE { BSS_CHARGE, BSS_SHOOT, BSS_STOP, BSS_END };
class CBossSword :
	public CMonster
{
public:
	CBossSword();
	virtual ~CBossSword();

public:
	virtual void Initialize()	override;
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(HDC hDC)override;
	virtual void Release()		override;

private:
	float			 m_fAngle;
	//float			 m_fFloorY;
	//float			 m_fFloorYDepth;
	BOSS_SWORD_STATE m_ePrevState;
	int				 m_iCurState;
	DWORD			 m_dwStateTime;
	DWORD			 m_dwStateMaxTime;
	POSITION		 m_tSwordBoundaryMin;
	POSITION		 m_tSwordBoundaryMax;
	vector<string>	 m_vecChargeKey;

private:
	void ChargeRender(HDC hDC); 
	void ChangeState();
	HDC	 StateDC();
};

