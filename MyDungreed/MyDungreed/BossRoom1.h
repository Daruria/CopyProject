#pragma once
#include "Scene.h"

enum BOSS_SCENE {BOSS_SCENE_APPEAR, BOSS_SCENE_DISAPPEAR, BOSS_SCENE_END};
class CBGImage;
class CBossUI;
class CStele;
class CMiniMap;
class CBossRoom1 :
	public CScene
{
public:
	CBossRoom1();
	virtual ~CBossRoom1();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;

private:
	static bool	m_bBoss;

private:
	bool			m_bBossName;
	bool			m_bFlash;
	BOSS_SCENE		m_eBossScene;
	DWORD			m_dwSceneStartTime;
	DWORD			m_dwPrevTime;
	DWORD			m_dwSceneEndTime;
	CBGImage*		m_pLongFloor;
	CBGImage*		m_pMiddleFloor;
	list<CBGImage*> m_ShortFloorList;
	list<CBGImage*> m_TorchList;
	CBossUI*		m_pBossLifeUI;
	CMiniMap*		m_pMiniMap;
	INFO			m_tUpCut;
	INFO			m_tDownCut;

private:
	void ImageSetting();
	void BGImageRender(HDC hDC);
	void TorchUpdate();

public:
	static void SpawnReset() { m_bBoss = false; }
};

