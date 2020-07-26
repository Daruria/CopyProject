#include "stdafx.h"
#include "KeyManager.h"

DEFINITION_SINGLE(CKeyManager);

CKeyManager::CKeyManager()
{
}


CKeyManager::~CKeyManager()
{
	Release();
}

void CKeyManager::Initialize()
{
	memset(m_bKeyState, false, sizeof(m_bKeyState));
}

void CKeyManager::Release()
{
}

bool CKeyManager::KeyPressing(int iKey)
{
	if (GetAsyncKeyState(iKey) & 0x8000)
		return true;
	return false;
}

bool CKeyManager::KeyDown(int iKey)
{
	if (!m_bKeyState[iKey] && (GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}

	return false;
}

bool CKeyManager::KeyUp(int iKey)
{
	if (m_bKeyState[iKey] && !(GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}
	return false;
}

void CKeyManager::KeyUpdate()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}
