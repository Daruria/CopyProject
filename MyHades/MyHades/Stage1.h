#pragma once
#include "Stage.h"
class CDoor;
class CStage1 :
	public CStage
{
private:
	enum PHASE { PHASE_START, PHASE1, PHASE2, PHASE3, END };

private:
	CStage1();
public:
	virtual ~CStage1();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CStage1)

private:
	void PhaseUpdate();
	void PhaseStart();
	void Phase1Update();
	void Phase2Update();
	void Phase3Update();
	void TitleCheck();


private:
	CStage1::PHASE m_eCurPhase;
	CDoor*	m_pDoor;
	
	float	m_fTimeCount;
	float	m_fPhaseDelay;

	static bool	m_bShowTitle;

	float	m_fTitleDelay;
};

