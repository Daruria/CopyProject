#include "stdafx.h"
#include "SceneMgr.h"
#include "Intro.h"
#include "MainTitle.h"
#include "Stage.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "MegaeraRoom.h"
#include "House.h"
#include "Ending.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr():
	m_pScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(SCENE_TYPE eCurType, SCENE_TYPE eNextType)
{
	if (eCurType != eNextType)
	{
		SafeDelete(m_pScene);
		CObjectMgr::GetInstance()->RenderListClear();
		switch (eNextType)
		{
		case SCENE_INTRO:
			m_pScene = CIntro::Create();
			break;
		case SCENE_MAIN_TITLE:
			m_pScene = CMainTitle::Create();
			break;
		case SCENE_STAGE1:
			m_pScene = CStage1::Create();
			break;
		case SCENE_STAGE2:
			m_pScene = CStage2::Create();
			break;
		case SCENE_STAGE3:
			m_pScene = CStage3::Create();
			break;
		case SCENE_MEGAERA:
			m_pScene = CMegaeraRoom::Create();
			break;
		case SCENE_HOUSE:
			m_pScene = CHouse::Create();
			break;
		case SCENE_ENDING:
			m_pScene = CEnding::Create();
			break;
		case SCENE_END:
			DestroyWindow(g_hWnd);
			break;
		default:
			break;
		}

		NULL_CHECK_RETURN(m_pScene, E_FAIL);
	}

	return S_OK;
}

int CSceneMgr::Update()
{
	int iEvent = 0;

	if (m_pScene)
		iEvent = m_pScene->Update();

	return iEvent;
}

void CSceneMgr::LateUpdate()
{
	if (m_pScene)
		m_pScene->LateUpdate();
}

void CSceneMgr::Render()
{
	if (m_pScene)
		m_pScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}
