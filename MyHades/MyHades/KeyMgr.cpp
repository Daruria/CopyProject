#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr);
CKeyMgr::CKeyMgr():
	m_dwKeyDown(0), m_dwKeyUp(0), m_dwCurKey(0)
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Update()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurKey |= KEY_S;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurKey |= KEY_D;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurKey |= KEY_Q;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwCurKey |= KEY_E;
	if (GetAsyncKeyState('F') & 0x8000)
		m_dwCurKey |= KEY_F;


	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurKey |= KEY_ENTER;
	if (GetAsyncKeyState(VK_F1) & 0x8000)
		m_dwCurKey |= KEY_F1;
	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_dwCurKey |= KEY_F2;
	if (GetAsyncKeyState(VK_F3) & 0x8000)
		m_dwCurKey |= KEY_F3;
	if (GetAsyncKeyState(VK_F4) & 0x8000)
		m_dwCurKey |= KEY_F4;
	if (GetAsyncKeyState('5') & 0x8000)
		m_dwCurKey |= KEY_5;
	if (GetAsyncKeyState('6') & 0x8000)
		m_dwCurKey |= KEY_6;
	if (GetAsyncKeyState(VK_F7) & 0x8000)
		m_dwCurKey |= KEY_F7;
	if (GetAsyncKeyState(VK_F8) & 0x8000)
		m_dwCurKey |= KEY_F8;


	
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
		return true;

	return false;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 전에 누른적 없고 현재 눌렸을 때 TRUE
	if (!(m_dwKeyDown & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// 전에 누른적 있고 현재 누르지 않았을 때 FALSE
	if ((m_dwKeyDown & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// 전에 누른적 있고 현재 누르지 않았을 때 TRUE
	if ((m_dwKeyUp & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	// 전에 누른적 없고 현재 눌렸을 때 FALSE
	if (!(m_dwKeyUp & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyUp |= dwKey;
		return false;
	}

	return false;
}
