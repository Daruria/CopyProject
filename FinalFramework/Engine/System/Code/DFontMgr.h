#ifndef DFontMgr_h__
#define DFontMgr_h__

#include "DFont.h"

BEGIN(Engine)

class ENGINE_DLL CDFontMgr : public CBase
{
	DECLARE_SINGLETON(CDFontMgr)

private:
	explicit CDFontMgr();
	virtual ~CDFontMgr();

public:
	HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pFontTag, const wstring& wstrFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void	Render_Font(const wstring& wstrFontTag, const wstring& wstrString, const _vec2* pPos, D3DXCOLOR Color);

private:
	CDFont* Find_Font(const wstring& pFontTag);

private:
	map<wstring, CDFont*>	m_mapFont;

public:
	virtual void Free();
};

END

#endif // DFontMgr_h__
