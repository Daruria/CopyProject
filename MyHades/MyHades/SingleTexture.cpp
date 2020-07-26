#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture():
	m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO * CSingleTexture::GetTexInfo(const wstring & wstrStateKey, const WORD & wIndex) const
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrStateKey, const WORD & wCnt, const WORD& wStart)
{
	D3DXIMAGE_INFO tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	HRESULT hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);

#ifdef _DEBUG
	wstring wstrMsg = wstrFilePath + L"D3DXGetImageInfoFromFile Failed";
	FAILED_CHECK_MSG_RETURN(hr, wstrMsg.c_str(), E_FAIL);
#else // _DEBUG
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);
#endif
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	hr = D3DXCreateTextureFromFileEx(pGraphicDev, wstrFilePath.c_str(), tImgInfo.Width, tImgInfo.Height, tImgInfo.MipLevels, 0,
		tImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &pTexture);

#ifdef _DEBUG
	wstrMsg = wstrFilePath + L"D3DXCreateTextureFromFileEx Failed";
	FAILED_CHECK_MSG_RETURN(hr, wstrMsg.c_str(), E_FAIL);
#else // _DEBUG
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);
#endif

	m_pTexInfo = new TEX_INFO;
	m_pTexInfo->pTexture = pTexture;
	m_pTexInfo->tImgInfo = tImgInfo;

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	SafeDelete(m_pTexInfo);
}

CSingleTexture * CSingleTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrStateKey, const WORD & wCnt)
{
	CSingleTexture* pInstance = new CSingleTexture;

	if (FAILED(pInstance->LoadTexture(pGraphicDev, wstrFilePath, wstrStateKey, wCnt)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
