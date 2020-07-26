#pragma once
#include "Scene.h"

class CBGImage;
class CMiniMap;
class CWormPassage;
class CDungeon2 :
	public CScene
{
public:
	CDungeon2();
	virtual ~CDungeon2();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	static bool m_bSpawn;
	static bool m_bTresureBox;

private:
	CBGImage* m_pSecondFloor;
	CBGImage* m_pFirstFloor;
	CMiniMap* m_pMiniMap;

	CWormPassage*	m_pWormPassage;

	bool	m_bFastMove;
	int		m_iFastMoveDest;

	DWORD	m_dwFastMoveDelay;
	DWORD	m_dwFastMoveDelayMax;

public:
	void SpawnMonster();
public:
	static void SpawnReset() { m_bSpawn = false; m_bTresureBox = false; }
};

