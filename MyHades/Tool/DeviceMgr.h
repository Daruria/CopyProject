#pragma once
class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9 GetDevice()	const { return m_pGraphicDev; };
	LPD3DXSPRITE GetSprite()		const { return m_pSprite; };
	LPD3DXFONT		GetFont()		const { return m_pFont; }
	LPD3DXLINE		GetLine()		const {	return m_pLine;	}

public:
	HRESULT InitDevice(DISPLAY_MODE eMode);
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);
	void Release();

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
};

