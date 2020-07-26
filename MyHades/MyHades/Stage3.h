#pragma once
#include "Stage.h"

class CDoor;
class CStage3 :
	public CStage
{
private:
	enum PHASE { PHASE_START, PHASE1, PHASE2, PHASE3, END };

protected:
	CStage3();
public:
	virtual ~CStage3();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CStage3)

private:
	void PhaseUpdate();
	void PhaseStart();
	void Phase1Update();
	void Phase2Update();
	void Phase3Update();


private:
	CStage3::PHASE m_eCurPhase;
	CDoor*	m_pDoor;

	float	m_fTimeCount;
	float	m_fPhaseDelay;
};

