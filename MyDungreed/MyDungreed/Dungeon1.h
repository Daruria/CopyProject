#pragma once
#include "Scene.h"
class CMiniMap;
class CDungeon1 :
	public CScene
{
public:
	CDungeon1();
	virtual ~CDungeon1();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:
	static bool	m_bSpawn;

private:
	CMiniMap* m_pMiniMap;

public:
	void SpawnMonster();

public:
	static void SpawnReset() { m_bSpawn = false; }
};

