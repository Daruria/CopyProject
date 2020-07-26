#include "stdafx.h"
#include "TextureMgr.h"
#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
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

int CTextureMgr::GetTextureMaxFrame(const wstring & wstrObjectKey, const wstring & wstrStateKey) const
{
	auto iter = m_umapTexture.find(wstrObjectKey);
	if (m_umapTexture.end() == iter)
		return 0;

	return dynamic_cast<CMultiTexture*>(iter->second)->GetStateSize(wstrStateKey);
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
