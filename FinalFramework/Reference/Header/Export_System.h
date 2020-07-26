#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Defines.h"
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "DFontMgr.h"
#include "InputDev.h"
#include "SoundMgr.h"
#include "VideoPlayer.h"

BEGIN(Engine)

//	GraphicDev
//	Get
//	Set
//	General
inline HRESULT	Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
inline void		Render_Begin(D3DXCOLOR Color);
inline void		Render_End(void);
inline void		Render_End(HWND hWnd);

//	TimerMgr
//	Get
inline void		Set_TimeDelta(const wstring& wstrTimerTag);
//	Set
inline _float	Get_TimeDelta(const wstring& wstrTimerTag);
inline _float	Get_CummulativeTime(const wstring& wstrTimerTag);
//	General
inline HRESULT	Ready_Timer(const wstring& wstrTimerTag);



//	FrameMgr
//	Get
//	Set
//	General
inline _bool	IsPermit_Call(const wstring& wstrFrameTag, const _float& fTimeDelta);
inline HRESULT	Ready_Frame(const wstring& wstrFrameTag, const _float& fCallLimit);

//	FontMgr
//	Get
//	Set
//	General
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFontTag, const wstring& wstrFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
inline void		Render_Font(const wstring& wstrFontTag, const wstring& wstrString, const _vec2* pPos, D3DXCOLOR Color);

//	InputDev
//	Get
inline _byte Get_DIKeyState(_ubyte byKeyID);
inline _byte Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
inline _float Get_MouseAccTime(MOUSEKEYSTATE eMouse);
// Set
inline void	Set_InputDev(const _float& fTimeDelta);
// General
inline HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline _bool	KeyDown(_ubyte byKeyID);
inline _bool	KeyUp(_ubyte byKeyID);
inline _bool	KeyPress(_ubyte byKeyID);
inline _bool	MouseDown(MOUSEKEYSTATE eMouse);
inline _bool	MouseUp(MOUSEKEYSTATE eMouse);
inline _bool	MousePress(MOUSEKEYSTATE eMouse);

//	SoundMgr
//	Get
//	Set
//	General
inline HRESULT Initialize_Sound();
inline HRESULT LoadSoundFile(const string& strSoundPath);
inline void Update_Sound();
inline void Play_Sound(const wstring& wstrSoundKey, CSoundMgr::CHANNEL_ID eID);
inline void Play_BGM(const wstring& wstrSoundKey);
inline void Stop_Sound(CSoundMgr::CHANNEL_ID eID);
inline void Stop_All();
inline void Set_Volume(CSoundMgr::CHANNEL_ID eID, float fVol);

//	VideoPlayer
//	General
inline HRESULT	Ready_VideoPlayer();
inline void		Play_Video(LPDIRECT3D9 pSDK, LPDIRECT3DDEVICE9 pGrahpicDev, HWND hWnd, const wstring& wstrFilePath);
inline void		Stop_Video();
inline void		Render_Video();
inline HRESULT	Get_VideoStatus(BVL_STATUS& rStatus);
inline _bool	Is_CurVideoEnd();
inline _bool	Is_VideoOpen();


//	Release System
inline void Release_System();

#include "Export_System.inl"

END

#endif // Export_System_h__
