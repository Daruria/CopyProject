#pragma once
#include "Stage.h"
class CAlecto;
class CMegaeraRoom :
	public CStage
{
public:
	enum PHASE { MEET, GREET, BATTLE, DIE, PHASE_END };
	enum TALK { Alecto0, Zagreus0, Alecto1, Zagreus1, Alecto2, TALK_END };

public:
	CMegaeraRoom();
	virtual ~CMegaeraRoom();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CMegaeraRoom)

public:
	void SetPhase(CMegaeraRoom::PHASE ePhase) { m_eCurPhase = ePhase; }

private:
	void PhaseUpdate();
	void MeetUpdate();
	void GreetUpdate();
	void BattleUpdate();
	void DieUpdate();
	void Scrolling();


private:
	CAlecto* m_pAlecto;
	CMegaeraRoom::PHASE m_eCurPhase;
	CMegaeraRoom::TALK	m_eCurTalk;

	bool	m_bScrollMoving;
	D3DXVECTOR3 m_vScroll;
	D3DXVECTOR3 m_vScrollTarget;
};

