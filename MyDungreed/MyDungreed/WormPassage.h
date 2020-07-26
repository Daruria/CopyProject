#pragma once
#include "NPC.h"
class CWormPassage :
	public CNPC
{

private:
	enum WORM_STATE {IDLE, EAT, DISGORGE, END};

public:
	CWormPassage();
	virtual ~CWormPassage();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void Interaction();

public:
	void Eat();
	void Disgorge();
	void SetDisgorgeEnd(bool bDisgorgeEnd) { m_bDisgorgeEnd = bDisgorgeEnd; }

public:
	const bool GetEatEnd()		const { return m_bEatEnd; }
	const bool GetDisgorgeEnd() const { return m_bDisgorgeEnd; }

private:
	void ChangeState();
	HDC MotionDC();

private:
	bool		m_bDisgorgeEnd;
	bool		m_bEatEnd;

	WORM_STATE m_ePreState;
	WORM_STATE m_eCurState;

};

