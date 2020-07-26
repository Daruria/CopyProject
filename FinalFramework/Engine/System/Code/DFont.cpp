#include "DFont.h"

Engine::CDFont::CDFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CDFont::~CDFont()
{

}

HRESULT Engine::CDFont::Ready_DFont(const wstring& wstrDFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC	DFont_Desc;
	ZeroMemory(&DFont_Desc, sizeof(D3DXFONT_DESC));

	DFont_Desc.CharSet = HANGUL_CHARSET;
	lstrcpy(DFont_Desc.FaceName, wstrDFontType.c_str());
	DFont_Desc.Width		= iWidth;
	DFont_Desc.Height	= iHeight;
	DFont_Desc.Weight	= iWeight;

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &DFont_Desc, &m_pDFont)))
	{
		MSG_BOX("DFont Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CDFont::Render_DFont(const wstring& wstrString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT rc{ _long(pPos->x), _long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDFont->DrawTextW(m_pSprite, wstrString.c_str(), (INT)wstrString.length(), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

Engine::CDFont* Engine::CDFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pDFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CDFont*	pInstance = new CDFont(pGraphicDev);

	if (FAILED(pInstance->Ready_DFont(pDFontType, iWidth, iHeight, iWeight)))
	{
		MSG_BOX("DFont Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CDFont::Free()
{
	Engine::Safe_Release(m_pDFont);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pGraphicDev);
}

