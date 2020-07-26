#pragma once
#include "NPC.h"

class CAbilityWindow;
class CCommander :
	public CNPC
{
public:
	CCommander();
	virtual ~CCommander();

private:
	enum COMMANDER_STATE { IDLE, MOVE, END };

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void Interaction();


private:
	void ChangeState();
	HDC MotionDC();

private:
	COMMANDER_STATE m_ePreState;
	COMMANDER_STATE m_eCurState;

};

