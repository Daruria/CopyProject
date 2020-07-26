#include "Export_System.h"
//	GraphicDev
//	Get
//	Set
//	General
HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void	Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void	Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}
void	Render_End(HWND hWnd)
{
	CGraphicDev::GetInstance()->Render_End(hWnd);
}

//	TimerMgr
//	Get
//	Set
//	General
void Set_TimeDelta(const wstring& wstrTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(wstrTimerTag);
}
_float Get_TimeDelta(const wstring& wstrTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(wstrTimerTag);
}
_float	Get_CummulativeTime(const wstring& wstrTimerTag)
{
	return CTimerMgr::GetInstance()->Get_CummulativeTime(wstrTimerTag);
}

HRESULT Ready_Timer(const wstring& wstrTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(wstrTimerTag);
}

//	FrameMgr
//	Get
//	Set
//	General
_bool IsPermit_Call(const wstring& wstrFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(wstrFrameTag, fTimeDelta);
}
HRESULT Ready_Frame(const wstring& wstrFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(wstrFrameTag, fCallLimit);
}

//	FontMgr
//	Get
//	Set
//	General
HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFontTag, const wstring& wstrFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return CDFontMgr::GetInstance()->Ready_Font(pGraphicDev, wstrFontTag, wstrFontType, iWidth, iHeight, iWeight);
}
void	Render_Font(const wstring& wstrFontTag, const wstring& wstrString, const _vec2* pPos, D3DXCOLOR Color)
{
	CDFontMgr::GetInstance()->Render_Font(wstrFontTag, wstrString, pPos, Color);
}

//	InputDev
//	Get
_byte Get_DIKeyState(_ubyte byKeyID) 
{ 
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte Get_DIMouseState(MOUSEKEYSTATE eMouse) 
{ 
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{ 
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}
_float Get_MouseAccTime(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_MouseAccTime(eMouse);
}
// Set
void	Set_InputDev(const _float& fTimeDelta)
{
	CInputDev::GetInstance()->Set_InputDev(fTimeDelta);
}
// General
HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}

_bool	KeyDown(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyDown(byKeyID);
}
_bool	KeyUp(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyUp(byKeyID);
}
_bool	KeyPress(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->KeyPress(byKeyID);
}

_bool	MouseDown(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MouseDown(eMouse);
}
_bool	MouseUp(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MouseUp(eMouse);
}
_bool	MousePress(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->MousePress(eMouse);
}

//	SoundMgr
//	Get
//	Set
//	General
HRESULT Initialize_Sound()
{
	return CSoundMgr::GetInstance()->Initialize();
}

HRESULT LoadSoundFile(const string& strSoundPath)
{
	return CSoundMgr::GetInstance()->LoadSoundFile(strSoundPath);
}

void Update_Sound()
{
	CSoundMgr::GetInstance()->Update_Sound();
}

void Play_Sound(const wstring& wstrSoundKey, CSoundMgr::CHANNEL_ID eID)
{
	CSoundMgr::GetInstance()->Play_Sound(wstrSoundKey, eID);
}

void Play_BGM(const wstring& wstrSoundKey)
{
	CSoundMgr::GetInstance()->Play_BGM(wstrSoundKey);
}

void Stop_Sound(CSoundMgr::CHANNEL_ID eID)
{
	CSoundMgr::GetInstance()->Stop_Sound(eID);
}

void Stop_All()
{
	CSoundMgr::GetInstance()->Stop_All();
}

void Set_Volume(CSoundMgr::CHANNEL_ID eID, float fVol)
{
	CSoundMgr::GetInstance()->Set_Volume(eID, fVol);
}

//	VideoPlayer
//	General
HRESULT	Ready_VideoPlayer()
{
	return CVideoPlayer::GetInstance()->Ready_VideoPlayer();
}
void Play_Video(LPDIRECT3D9 pSDK, LPDIRECT3DDEVICE9 pGrahpicDev, HWND hWnd, const wstring& wstrFilePath)
{
	CVideoPlayer::GetInstance()->Play_Video(pSDK, pGrahpicDev, hWnd, wstrFilePath);
}
void Stop_Video()
{
	CVideoPlayer::GetInstance()->Stop_Video();
}
void		Render_Video()
{
	CVideoPlayer::GetInstance()->Render_Video();
}

HRESULT	Get_VideoStatus(BVL_STATUS& rStatus)
{
	return CVideoPlayer::GetInstance()->Get_VideoStatus(rStatus);
}

_bool	Is_CurVideoEnd()
{
	return CVideoPlayer::GetInstance()->Is_CurVideoEnd();
}

inline _bool Is_VideoOpen()
{
	return CVideoPlayer::GetInstance()->Is_VideoOpen();
}




//	Release System
void Release_System()
{
	CVideoPlayer::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CInputDev::GetInstance()->DestroyInstance();
	CDFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}