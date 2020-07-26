#pragma once
#include "Scene.h"
class CBGImage;
class CMiniMap;
class CDungeon3 :
	public CScene
{
public:
	CDungeon3();
	virtual ~CDungeon3();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	static bool m_bSpawn;
	CBGImage* m_Floor;
	CMiniMap* m_pMiniMap;

public:
	void SpawnMonster();
public:
	static void SpawnReset() { m_bSpawn = false; }
};

