#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase 
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev(void);
	virtual ~CGraphicDev(void);

public:
	HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
	void	Render_Begin(D3DXCOLOR Color);
	void	Render_End(void);
	void	Render_End(HWND hWnd);

public:
	LPDIRECT3D9			GetSDK()	{ return m_pSDK; }
	LPDIRECT3DDEVICE9	GetDevice() { return m_pGraphicDev; }
	LPD3DXSPRITE		GetSprite() { return m_pSprite; }

private:
	LPDIRECT3D9			m_pSDK = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	LPD3DXSPRITE		m_pSprite = nullptr;

public:
	inline virtual void Free(void);	

};


END

#endif // GraphicDev_h__
