#include "stdafx.h"
#include "MainGame.h"
#include "Mouse.h"

CMainGame::CMainGame():
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pFrameMgr(CFrameMgr::GetInstance()),
	m_pMouse(CMouse::GetInstance()),
	m_pFontMgr(CFontMgr::GetInstance()),
	m_pSubjectMgr(CSubjectMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance()),
	m_iEvent(NO_EVENT)
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Update()
{
	m_pTimeMgr->Update();
	m_pKeyMgr->Update();
	m_iEvent = m_pSceneMgr->Update();
	m_pSoundMgr->UpdateSound();
}

void CMainGame::LateUpdate()
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	m_pSceneMgr->LateUpdate();
}

void CMainGame::Render()
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	m_pDeviceMgr->Render_Begin();

	m_pSceneMgr->Render();

#ifdef _DEBUG
	m_pFrameMgr->RenderFPS();
#endif // _DEBUG

	m_pMouse->Render();

	m_pDeviceMgr->Render_End();
}

HRESULT CMainGame::Initialize()
{
	HRESULT hr = m_pDeviceMgr->InitDevice(MODE_WIN);
	FAILED_CHECK_MSG_RETURN(hr, L"InitDevice Failed", E_FAIL);

	hr = m_pSoundMgr->Initialize();
	FAILED_CHECK_MSG_RETURN(hr, L"SoundMgr Initialize Failed", E_FAIL);

	hr = m_pSceneMgr->SceneChange(SCENE_END, SCENE_INTRO);
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Change Failed", E_FAIL);

	hr = m_pMouse->Initalize();
	FAILED_CHECK_MSG_RETURN(hr, L"Mouse Initialize Failed", E_FAIL);

	hr = m_pFontMgr->Initialize();
	FAILED_CHECK_MSG_RETURN(hr, L"FontMgr Initialize Failed", E_FAIL);

	srand(static_cast<unsigned int>(time(NULL)));

	return S_OK;
}

void CMainGame::Release()
{
	CAstarMgr::GetInstance()->DestroyInstance();
	m_pSoundMgr->DestroyInstance();
	CThreadMgr::GetInstance()->DestroyInstance();
	m_pFontMgr->DestroyInstance();
	m_pMouse->DestroyInstance();
	m_pFrameMgr->DestroyInstance();
	m_pSceneMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pSubjectMgr->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

CMainGame * CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
