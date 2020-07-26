#ifndef VideoPlayer_h__
#define VideoPlayer_h__

#include "bandivid.h"

#include "BandiVideoLibrary.h"
#include "BandiVideoTexture_DX9.h"

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CVideoPlayer : public CBase
{
	DECLARE_SINGLETON(CVideoPlayer)

private:
	explicit CVideoPlayer();
	virtual ~CVideoPlayer();

public:
	HRESULT Ready_VideoPlayer();
	void	Play_Video(LPDIRECT3D9 pGraphic, LPDIRECT3DDEVICE9 pGrahpicDev, HWND hWnd, const wstring& wstrFilePath);
	void	Stop_Video();
	void	Render_Video();
	HRESULT	Get_VideoStatus(BVL_STATUS& rStatus);
	_bool	Is_CurVideoEnd();
	_bool	Is_VideoOpen() { return m_bOpen; }

private:
	CBandiVideoLibrary	m_BVLibrary;
	CBandiVideoTexture* m_pBVTexture = NULL;
	BVL_VIDEO_INFO		m_Info;
	_bool				m_bOpen = false;
	_bool				m_bLoop = false;

private:
	virtual void Free() override;

};

END

#endif // VideoPlayer_h__