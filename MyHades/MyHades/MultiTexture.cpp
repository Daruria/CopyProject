#include "stdafx.h"
#include "MultiTexture.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO * CMultiTexture::GetTexInfo(const wstring & wstrStateKey, const WORD & wIndex) const
{
	auto iter = m_umapMulti.find(wstrStateKey);

	if (m_umapMulti.end() == iter)
		return nullptr;

	if (iter->second.size() <= wIndex)
		return nullptr;

	return iter->second[wIndex];
}

HRESULT CMultiTexture::LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrStateKey, const WORD & wCnt, const WORD& wStart)
{
	auto iter = m_umapMulti.find(wstrStateKey);
	
	if (m_umapMulti.end() != iter)
		return E_FAIL;

	D3DXIMAGE_INFO tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	HRESULT hr = 0;

	TCHAR szFullPath[MAX_STR] = L"";
	TEX_INFO* pTexInfo = nullptr;

	for (WORD i = wStart; i < wCnt + wStart; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		hr = D3DXGetImageInfoFromFile(szFullPath, &tImgInfo);

#ifdef _DEBUG
		wstring wstrMsg = szFullPath;
		wstrMsg += L"D3DXGetImageInfoFromFile Failed";
		FAILED_CHECK_MSG_RETURN(hr, wstrMsg.c_str(), E_FAIL);
#else
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);
#endif // _DEBUG

		hr = D3DXCreateTextureFromFileEx(pGraphicDev, szFullPath, tImgInfo.Width, tImgInfo.Height, tImgInfo.MipLevels, 0,
			tImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &pTexture);

#ifdef _DEBUG
		wstrMsg = szFullPath;
		wstrMsg += L"D3DXCreateTextureFromFileEx Failed";
		FAILED_CHECK_MSG_RETURN(hr, wstrMsg.c_str(), E_FAIL);
#else
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);
#endif // _DEBUG

		pTexInfo = new TEX_INFO;
		pTexInfo->pTexture = pTexture;
		pTexInfo->tImgInfo = tImgInfo;

		m_umapMulti[wstrStateKey].push_back(pTexInfo);
	}
	
	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& Pair : m_umapMulti)
	{
		for (auto& pTexInfo : Pair.second)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		}
		Pair.second.clear();
	}
	m_umapMulti.clear();
}

CMultiTexture * CMultiTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrStateKey, const WORD & wCnt, const WORD& wStart)
{
	CMultiTexture* pInstance = new CMultiTexture;

	if (FAILED(pInstance->LoadTexture(pGraphicDev, wstrFilePath, wstrStateKey, wCnt, wStart)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CMultiTexture::ReleaseStateTexture(const wstring & wstrStateKey)
{
	auto iter = m_umapMulti.find(wstrStateKey);

	if (m_umapMulti.end() == iter)
	{
		wstring wstrMsg = wstrStateKey + L"does not exist.";
		::MessageBox(g_hWnd, wstrMsg.c_str(), L"State Texture Release Failed", MB_OK);
		return E_FAIL;
	}

	for (auto& pTexInfo : iter->second)
	{
		pTexInfo->pTexture->Release();
		SafeDelete(pTexInfo);
	}
	iter->second.clear();
	m_umapMulti.erase(iter);

	return S_OK;
}

size_t CMultiTexture::GetStateSize(const wstring & wstrStateKey) const
{
	auto iter = m_umapMulti.find(wstrStateKey);
	if (m_umapMulti.end() == iter)
	{
		wstring wstrMsg = wstrStateKey + L" State does not exist.";
		::MessageBox(g_hWnd, wstrMsg.c_str(), L"Error", MB_OK);

		return 0;
	}

	return iter->second.size();
}

vector<TEX_INFO*>* CMultiTexture::GetStateVector(const wstring & wstrStateKey)
{
	// TODO: insert return statement here
	auto iter = m_umapMulti.find(wstrStateKey);
	if (m_umapMulti.end() == iter)
		return nullptr;

	return &(iter->second);
}
