#include "stdafx.h"
#include "TextureMgr.h"
#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr():
	m_fLoadingRate(0.f),
	m_bLoadingSuccess(false)
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO * CTextureMgr::GetTexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const WORD wIndex) const
{
	auto iter = m_umapTexture.find(wstrObjectKey);

	if (m_umapTexture.end() == iter)
		return nullptr;

	return iter->second->GetTexInfo(wstrStateKey, wIndex);
}

CTexture * CTextureMgr::GetTexture(const wstring & wstrObjectKey)
{
	auto iter = m_umapTexture.find(wstrObjectKey);
	if (m_umapTexture.end() == iter)
		return nullptr;

	return iter->second;
}

const int CTextureMgr::GetTextureMaxFrame(const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	auto iter = m_umapTexture.find(wstrObjectKey);
	if (m_umapTexture.end() == iter)
		return 0;

	return dynamic_cast<CMultiTexture*>(iter->second)->GetStateSize(wstrStateKey);
}

HRESULT CTextureMgr::LoadTextureFromPathInfo(LPDIRECT3DDEVICE9 pGrahicDev, const wstring & wstrFilePath)
{
	wifstream fin;

	fin.open(wstrFilePath);

	if (fin.fail())
		return E_FAIL;

	m_bLoadingSuccess = false;

	TCHAR szTotal[MAX_STR] = L"";
	
	fin.getline(szTotal, MAX_STR);

	int iTotalTexture = _ttoi(szTotal);
	int iReadTexture = 0;

	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";

	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szCount, MAX_STR, '|');
		fin.getline(szRelative, MAX_STR);

		if (fin.eof())
			break;

		HRESULT hr;

		if (wstring(szObjectKey) == L"SingleTexture")
			hr = LoadTexture(pGrahicDev, TEXTURE_SINGLE, szRelative, szStateKey);
		else
			hr = LoadTexture(pGrahicDev, TEXTURE_MULTI,	szRelative, szObjectKey, szStateKey, _ttoi(szCount), 1);

		++iReadTexture;

		m_fLoadingRate = float(iReadTexture) / iTotalTexture;

		if (FAILED(hr))
		{
			//fin.close();
			//return E_FAIL;
			continue;
		}
	}
	m_bLoadingSuccess = true;

	fin.close();
	return S_OK;
}

HRESULT CTextureMgr::LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURE_TYPE eTextureType, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const WORD & wCnt, const WORD& wStart)
{
	CTexture* pTexture = nullptr;

	auto iter = m_umapTexture.find(wstrObjectKey);

	HRESULT hr = 0;

	if (m_umapTexture.end() == iter)
	{
		switch (eTextureType)
		{
		case TEXTURE_SINGLE:
			pTexture = CSingleTexture::Create(pGraphicDev, wstrFilePath);
			break;
		case TEXTURE_MULTI:
			pTexture = CMultiTexture::Create(pGraphicDev, wstrFilePath, wstrStateKey, wCnt, wStart);
			break;
		}

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_umapTexture.insert(make_pair(wstrObjectKey, pTexture));
	}
	else
	{
		switch (eTextureType)
		{
		case TEXTURE_SINGLE:
			return E_FAIL;
		case TEXTURE_MULTI:
			hr = m_umapTexture[wstrObjectKey]->LoadTexture(pGraphicDev, wstrFilePath, wstrStateKey, wCnt, wStart);
			FAILED_CHECK_RETURN(hr, E_FAIL);
			break;
		}
	}

	return S_OK;
}

HRESULT CTextureMgr::ReleaseTexture(TEXTURE_TYPE eTextureType, const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	auto iter = m_umapTexture.find(wstrObjectKey);
	
	if (m_umapTexture.end() == iter)
	{
		wstring wstrMsg = wstrObjectKey + L" Release Failed";
		::MessageBox(g_hWnd, wstrMsg.c_str(), L"Release Failed", MB_OK);
		return E_FAIL;
	}

	switch (eTextureType)
	{
	case TEXTURE_SINGLE:
		SafeDelete(iter->second);
		m_umapTexture.erase(iter);
		break;
	case TEXTURE_MULTI:
		if (L"" == wstrStateKey)
		{
			SafeDelete(iter->second);
			m_umapTexture.erase(iter);
		}
		else
		{
			CMultiTexture* pMultiTexture = dynamic_cast<CMultiTexture*>(iter->second);
			HRESULT hr = pMultiTexture->ReleaseStateTexture(wstrStateKey);
			FAILED_CHECK_RETURN(hr, E_FAIL);

			if (0 == pMultiTexture->GetUmapMultiSize())
			{
				SafeDelete(iter->second);
				m_umapTexture.erase(iter);
			}
		}
		break;
	}



	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& Pair : m_umapTexture)
		SafeDelete(Pair.second);
	
	m_umapTexture.clear();
}
