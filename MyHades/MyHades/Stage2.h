#pragma once
#include "Stage.h"
class CDoor;
class CStage2 :
	public CStage
{
private:
	enum PHASE { PHASE_START, PHASE1, PHASE2, END };
protected:
	CStage2();
public:
	virtual ~CStage2();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CStage2)

private:
	void PhaseUpdate();
	void PhaseStart();
	void Phase1Update();
	void Phase2Update();

private:
	CStage2::PHASE m_eCurPhase;
	CDoor*	m_pDoor;

	float	m_fTimeCount;
	float	m_fPhaseDelay;

};