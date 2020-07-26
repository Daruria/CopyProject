#ifndef DFont_h__
#define DFont_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CDFont : public CBase
{
private:
	explicit CDFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	~CDFont();

public:
	HRESULT Ready_DFont(const wstring& wstrDFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void	Render_DFont(const wstring& wstrString, const _vec2* pPos, D3DXCOLOR Color);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXFONT			m_pDFont;
	LPD3DXSPRITE		m_pSprite;

public:
	static CDFont* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pDFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);

private:
	virtual void Free();
};

END

#endif // DFont_h__
