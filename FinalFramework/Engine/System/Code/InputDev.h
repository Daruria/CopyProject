#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	_byte Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState) { return *(((_long*)&m_tMouseState) + eMouseState); }

	_bool KeyDown(_ubyte byKeyID);
	_bool KeyUp(_ubyte byKeyID);
	_bool KeyPress(_ubyte byKeyID);

	_bool MouseDown(MOUSEKEYSTATE eMouse);
	_bool MouseUp(MOUSEKEYSTATE eMouse);
	_bool MousePress(MOUSEKEYSTATE eMouse);

public:
	HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void		Set_InputDev(const _float& fTimeDelta);
	_float		Get_MouseAccTime(MOUSEKEYSTATE eMouse) { return m_fMouseAccTime[eMouse]; }

private:
	LPDIRECTINPUT8	m_pInputSDK			= nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;

private:
	_byte			m_byKeyState[256];
	_bool			m_bKeyDown[256];
	_bool			m_bKeyUp[256];
	DIMOUSESTATE	m_tMouseState;
	_bool			m_bMouseDown[DIM_END];
	_bool			m_bMouseUp[DIM_END];

	_float			m_fMouseAccTime[DIM_END];


public:
	virtual void Free();
};

END

#endif // InputDev_h__
