#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev()
{

}

Engine::CInputDev::~CInputDev()
{
	Free();
}

_bool CInputDev::KeyDown(_ubyte byKeyID)
{
	_bool	bCurKeyState = (m_byKeyState[byKeyID] & 0x80) ? true : false;

	//	전에 누른 적 없고 현재 눌렀을 때 true
	if (!(m_bKeyDown[byKeyID]) && (bCurKeyState))
	{
		m_bKeyDown[byKeyID] = true;
		return true;
	}
	
	//	전에 누른 적 있고 현재 누르지 않았을 때 false
	if (m_bKeyDown[byKeyID] && !bCurKeyState)
	{
		m_bKeyDown[byKeyID] = false;
		return false;
	}

	return false;
}

_bool CInputDev::KeyUp(_ubyte byKeyID)
{
	_bool	bCurKeyState = (m_byKeyState[byKeyID] & 0x80) ? true : false;

	//	전에 누른 적 있고 현재 누르지 않았을 때 true;
	if (m_bKeyUp[byKeyID] && !bCurKeyState)
	{
		m_bKeyUp[byKeyID] = false;
		return true;

	}

	if (!m_bKeyUp[byKeyID] && bCurKeyState)
	{
		m_bKeyUp[byKeyID] = true;
		return false;
	}

	return false;
}

_bool CInputDev::KeyPress(_ubyte byKeyID)
{
	return  (m_byKeyState[byKeyID] & 0x80) ? true : false;
}

_bool CInputDev::MouseDown(MOUSEKEYSTATE eMouse)
{
	_bool	bCurKeyState = (m_tMouseState.rgbButtons[eMouse] & 0x80) ? true : false;

	//	전에 누른 적 없고 현재 눌렀을 때 true
	if (!(m_bMouseDown[eMouse]) && (bCurKeyState))
	{
		m_bMouseDown[eMouse] = true;
		return true;
	}

	//	전에 누른 적 있고 현재 누르지 않았을 때 false
	if (m_bMouseDown[eMouse] && !bCurKeyState)
	{
		m_bMouseDown[eMouse] = false;
		return false;
	}

	return false;
}

_bool CInputDev::MouseUp(MOUSEKEYSTATE eMouse)
{
	_bool	bCurKeyState = (m_tMouseState.rgbButtons[eMouse] & 0x80) ? true : false;

	//	전에 누른 적 있고 현재 누르지 않았을 때 true;
	if (m_bMouseUp[eMouse] && !bCurKeyState)
	{
		m_bMouseUp[eMouse] = false;
		return true;

	}

	if (!m_bMouseUp[eMouse] && bCurKeyState)
	{
		m_bMouseUp[eMouse] = true;
		return false;
	}

	return false;
}

_bool CInputDev::MousePress(MOUSEKEYSTATE eMouse)
{
	return  (m_tMouseState.rgbButtons[eMouse] & 0x80) ? true : false;
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	FAILED_CHECK_RETURN(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();

	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();
	
	ZeroMemory(m_bKeyDown,	sizeof(_bool) * 256);
	ZeroMemory(m_bKeyUp,	sizeof(_bool) * 256);

	ZeroMemory(m_fMouseAccTime, sizeof(_float) * DIM_END);

	return S_OK;
}

void Engine::CInputDev::Set_InputDev(const _float & fTimeDelta)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	for (_int i = 0; i < DIM_END; ++i)
	{
		if (m_tMouseState.rgbButtons[i] & 0x80)
			m_fMouseAccTime[i] += fTimeDelta;
		else
			m_fMouseAccTime[i] = 0.f;
	}
}

void Engine::CInputDev::Free()
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}