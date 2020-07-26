#pragma once
#include "MoveObj.h"
class CNPC :
	public CMoveObj
{
public:
	CNPC();
	virtual ~CNPC();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void Interaction();

protected:
	bool m_bDir;
	bool m_bCanInterAct;
	bool m_bIsOpen;

public:
	void SetDirection(bool bDir) { m_bDir = bDir; }
	void SetCanInteract(bool bCanInterAct) { m_bCanInterAct = bCanInterAct; }

public:
	const bool GetDirection() const { return m_bDir; }
};

