#include "stdafx.h"
#include "Scene.h"


CScene::CScene():
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance())
{
}


CScene::~CScene()
{
}

void CScene::LoadMap(const TCHAR * pFullPath)
{
	HANDLE hFile = ::CreateFile(pFullPath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	m_pObjectMgr->LoadData(hFile, pFullPath);

	CloseHandle(hFile);
}
