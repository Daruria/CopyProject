#include "stdafx.h"
#include "ThreadMgr.h"

IMPLEMENT_SINGLETON(CThreadMgr)

CThreadMgr::CThreadMgr():
	m_hLoadingThread(NULL),
	m_wstrPath(L""),
	m_hAstarThread(NULL),
	m_bAstarThreading(false)
{
	ZeroMemory(&m_vStartPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vGoalPos, sizeof(D3DXVECTOR3));
}

CThreadMgr::~CThreadMgr()
{
	if (m_hLoadingThread)
		CloseHandle(m_hLoadingThread);
	if (m_hAstarThread)
		CloseHandle(m_hAstarThread);
}

bool CThreadMgr::LoadingThread(const wstring& wstrPath)
{
	if (m_hLoadingThread)
		return false;

	m_wstrPath = wstrPath;

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThreadFunc, nullptr, 0, nullptr);
	NULL_CHECK_RETURN(m_hLoadingThread, false);

	return true;
}

bool CThreadMgr::AstarThread(const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos)
{
	if (m_bAstarThreading)
		return false;
	//if (m_hAstarThread)
	//	return false;

	m_bAstarThreading = true;

	m_vStartPos = vStartPos;
	m_vGoalPos = vGoalPos;

	m_hAstarThread = (HANDLE)_beginthreadex(nullptr, 0, AstarThreadFunc, nullptr, 0, nullptr);
	NULL_CHECK_RETURN(m_hAstarThread, false);

	return true;
}

void CThreadMgr::WaitForThread()
{
	WaitForSingleObject(m_hLoadingThread, INFINITE);

	if (m_hLoadingThread)
		CloseHandle(m_hLoadingThread);

	m_hLoadingThread = NULL;
}

void CThreadMgr::WaitForAstarThread()
{
	if (m_hAstarThread)
		WaitForSingleObject(m_hAstarThread, INFINITE);

	if (m_hAstarThread)
		CloseHandle(m_hAstarThread);

	m_hAstarThread = NULL;
}

unsigned CThreadMgr::LoadingThreadFunc(void * pParam)
{
	if (CThreadMgr::GetInstance()->GetPath().empty())
		return 1;

	wstring wstrPath = CThreadMgr::GetInstance()->GetPath();

	LPDIRECT3DDEVICE9 pGraphicDev = CDeviceMgr::GetInstance()->GetDevice();

	HRESULT hr = CTextureMgr::GetInstance()->LoadTextureFromPathInfo(pGraphicDev, wstrPath);
	FAILED_CHECK_MSG_RETURN(hr, L"Loading Failed", 1);

	CThreadMgr::GetInstance()->ClaerPath();

	return 0;
}

unsigned CThreadMgr::AstarThreadFunc(void * pParam)
{
	if (CAstarMgr::GetInstance()->AstarProceeding())
	{
		//if (m_pInstance->m_hAstarThread)
		//	CloseHandle(m_pInstance->m_hAstarThread);

		//m_pInstance->m_hAstarThread = NULL;
		m_pInstance->m_bAstarThreading = false;
		return 1;
	}

	CAstarMgr::GetInstance()->StartAstar(m_pInstance->m_vStartPos, m_pInstance->m_vGoalPos);

	//if (m_pInstance->m_hAstarThread)
	//	CloseHandle(m_pInstance->m_hAstarThread);

	//m_pInstance->m_hAstarThread = NULL;

	m_pInstance->m_bAstarThreading = false;
	return 0;
}
