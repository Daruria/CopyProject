#include "DFontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CDFontMgr)

Engine::CDFontMgr::CDFontMgr()
{

}

Engine::CDFontMgr::~CDFontMgr()
{
	Free();
}

HRESULT Engine::CDFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFontTag, const wstring& wstrFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CDFont*	pFont = Find_Font(wstrFontTag);

	if (nullptr != pFont)
		return E_FAIL;

	pFont = CDFont::Create(pGraphicDev, wstrFontType, iWidth, iHeight, iWeight);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.emplace(wstrFontTag, pFont);

	return S_OK;
}

void Engine::CDFontMgr::Render_Font(const wstring& wstrFontTag, const wstring& wstrString, const _vec2* pPos, D3DXCOLOR Color)
{
	CDFont*	pFont = Find_Font(wstrFontTag);
	NULL_CHECK(pFont);

	pFont->Render_DFont(wstrString, pPos, Color);
}

Engine::CDFont* Engine::CDFontMgr::Find_Font(const wstring& pFontTag)
{
	auto iter = m_mapFont.find(pFontTag);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void Engine::CDFontMgr::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

