#pragma once
#include "Scene.h"

class CAnimatedBGObject;
class CMiniMap;
class CWormPassage;
class CDungeonEntrance :
	public CScene
{
public:
	CDungeonEntrance();
	virtual ~CDungeonEntrance();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	CAnimatedBGObject*	m_pDoor;
	CMiniMap*			m_pMiniMap;
	CWormPassage*		m_pWormPassage;

	bool	m_bFastMove;
	int		m_iFastMoveDest;

	DWORD	m_dwFastMoveDelay;
	DWORD	m_dwFastMoveDelayMax;

};

