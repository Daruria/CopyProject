#include "stdafx.h"
#include "SceneManager.h"
#include "JusinLogo.h"
#include "MainLogo.h"
#include "Scene.h"
#include "MyEdit.h"
#include "Town.h"
#include "DungeonEntrance.h"
#include "Dungeon1.h"
#include "Dungeon2.h"
#include "Dungeon3.h"
#include "DungeonShop.h"
#include "BossRoom1.h"

DEFINITION_SINGLE(CSceneManager);

CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	m_pScene = new CJusinLogo;
	m_pScene->Initialize();
}

int CSceneManager::Update()
{
	switch (m_pScene->Update())
	{
	case SC_NONE:
		break;
	case SC_JUSIN:
		SAFE_DELETE(m_pScene);
		m_pScene = new CJusinLogo;
		m_pScene->Initialize();
		break;
	case SC_LOGO:
		SAFE_DELETE(m_pScene);
		m_pScene = new CMainLogo;
		m_pScene->Initialize();
		break;
	case SC_TOWN:
		SAFE_DELETE(m_pScene);
		m_pScene = new CTown;
		m_pScene->Initialize();
		break;
	case SC_DUNGEON_ENTRANCE:
		SAFE_DELETE(m_pScene);
		m_pScene = new CDungeonEntrance;
		m_pScene->Initialize();
		break;
	case SC_STAGE1:
		SAFE_DELETE(m_pScene);
		m_pScene = new CDungeon1;
		m_pScene->Initialize();
		break;
	case SC_STAGE2:
		SAFE_DELETE(m_pScene);
		m_pScene = new CDungeon2;
		m_pScene->Initialize();
		break;
	case SC_STAGE3:
		SAFE_DELETE(m_pScene);
		m_pScene = new CDungeon3;
		m_pScene->Initialize();
		break;
	case SC_SHOP:
		SAFE_DELETE(m_pScene);
		m_pScene = new CDungeonShop;
		m_pScene->Initialize();
		break;
	case SC_BOSS1:
		SAFE_DELETE(m_pScene);
		m_pScene = new CBossRoom1;
		m_pScene->Initialize();
		break;
	case SC_EDIT:
		SAFE_DELETE(m_pScene);
		m_pScene = new CMyEdit;
		m_pScene->Initialize();
		break;
	case SC_END:
		PostQuitMessage(0);
		break;
	}
	return 0;
}

void CSceneManager::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneManager::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneManager::Release()
{
	SAFE_DELETE(m_pScene);
}
