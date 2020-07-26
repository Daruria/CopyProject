#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
{
	size_t iContainerSize = rhs.m_vecTexInfo.size();
	m_vecTexInfo.reserve(iContainerSize);

	m_vecTexInfo = rhs.m_vecTexInfo;

	for (size_t i = 0; i < iContainerSize; ++i)
		m_vecTexInfo[i].pTexture->AddRef();
}

Engine::CTexture::~CTexture()
{

}

HRESULT Engine::CTexture::Ready_Texture(const wstring& wstrPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	m_vecTexInfo.reserve(iCnt);

	//TEX_INFO* pTexInfo = nullptr;
	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		_tchar	szFileName[256] = L"";
		wsprintf(szFileName, wstrPath.c_str(), i);

		D3DXIMAGE_INFO tImgInfo;
		ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));
		FAILED_CHECK_RETURN(D3DXGetImageInfoFromFile(szFileName, &tImgInfo), E_FAIL);

		switch (eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		//pTexInfo = new TEX_INFO;
		TEX_INFO tTexInfo;
		tTexInfo.pTexture = pTexture;
		tTexInfo.tImgInfo = tImgInfo;

		m_vecTexInfo.push_back(tTexInfo);
	}

	return S_OK;
}

HRESULT CTexture::Ready_Texture(LPDIRECT3DTEXTURE9* ppTexture, TEXTURETYPE eType, const _uint & iCnt)
{
	m_vecTexInfo.reserve(iCnt);

	//TEX_INFO*		pTexInfo = nullptr;
	//D3DXIMAGE_INFO	tImgInfo;
	//ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));
	for (_uint i = 0; i < iCnt; ++i)
	{
		//pTexInfo = new TEX_INFO;
		TEX_INFO tTexInfo;
		tTexInfo.pTexture = ppTexture[i];
		ZeroMemory(&tTexInfo.tImgInfo, sizeof(D3DXIMAGE_INFO));
		m_vecTexInfo.push_back(tTexInfo);
		tTexInfo.pTexture->AddRef();
	}

	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexInfo.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexInfo[iIndex].pTexture);
}

void CTexture::Render_Texture(const char* pShaderTexture, LPD3DXEFFECT& pEffect, const _uint& iIndex)
{
	if (m_vecTexInfo.size() < iIndex)
		return;

	if (pEffect)
	{
		pEffect->SetTexture(pShaderTexture, m_vecTexInfo[iIndex].pTexture);
	}
}

const TEX_INFO * CTexture::Get_TexInfo(const _uint & iIndex, D3DXIMAGE_INFO * pOut) const
{
	if (pOut)
		memcpy(pOut, &m_vecTexInfo[iIndex].tImgInfo, sizeof(D3DXIMAGE_INFO));
	return &m_vecTexInfo[iIndex];
}

void CTexture::Set_Texture(LPD3DXEFFECT & pEffect, const char * pShaderName, const _uint & iIndex)
{
	if (m_vecTexInfo.size() < iIndex)
		return;

	pEffect->SetTexture(pShaderName, m_vecTexInfo[iIndex].pTexture);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*	pInst = new CTexture(pGraphicDev);

	if (FAILED(pInst->Ready_Texture(wstrPath, eType, iCnt)))
		Safe_Release(pInst);

	return pInst;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPDIRECT3DTEXTURE9* ppTexture, TEXTURETYPE eType, const _uint & iCnt)
{
	CTexture*	pInst = new CTexture(pGraphicDev);

	if (FAILED(pInst->Ready_Texture(ppTexture, eType, iCnt)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CTexture::Clone()
{
	return new CTexture(*this);
}

void Engine::CTexture::Free()
{
	for (auto& pTexInfo : m_vecTexInfo)
	{
		_ulong dwRef = Safe_Release(pTexInfo.pTexture);
		//if (0 == dwRef)
		//{
		//	delete pTexInfo;
		//	pTexInfo = nullptr;
		//}
	}

	m_vecTexInfo.clear();

	CResources::Free();
}

