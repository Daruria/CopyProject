#include "stdafx.h"
#include "BitmapManager.h"
#include "MyBitmap.h"

DEFINITION_SINGLE(CBitmapManager);

CBitmapManager::CBitmapManager()
{
}


CBitmapManager::~CBitmapManager()
{
	Release();
}

void CBitmapManager::Initialize()
{

}

void CBitmapManager::Release()
{
	for (auto iter : m_ImageMap)
	{
		SAFE_DELETE(iter.second);
	}
}

void CBitmapManager::InsertBitmap(const string& strKey, const wchar_t * pPath)
{
	if (m_ImageMap.find(strKey) != m_ImageMap.end())
		return;

	CMyBitmap* pBitmap = new CMyBitmap;
	pBitmap->Initialize();
	pBitmap->SetImage(pPath);
	m_ImageMap[strKey] = pBitmap;
	
}

CMyBitmap * CBitmapManager::FindBitmap(const string& strKey)
{
	if (m_ImageMap.find(strKey) == m_ImageMap.end())
		return nullptr;
	
	return m_ImageMap.find(strKey)->second;
}

HDC CBitmapManager::FindBitmapDC(const string& strKey)
{
	if (m_ImageMap.find(strKey) == m_ImageMap.end())
		return NULL;

	return m_ImageMap.find(strKey)->second->GetMemDC();
}

HDC CBitmapManager::FindRotatedDC(const string& strKey, const float fRadians)
{
	if (m_ImageMap.find(strKey) == m_ImageMap.end())
		return NULL;

	CMyBitmap* pBitmap = m_ImageMap.find(strKey)->second;
	pBitmap->RotateDC(fRadians);

	return pBitmap->GetRotatedDC();
}

SIZE CBitmapManager::GetBitmapSize(const string & strKey)
{
	CMyBitmap* pBitmap = FindBitmap(strKey);
	HBITMAP hBitmap = pBitmap->GetBitmap();
	BITMAP	bm;
	GetObject(hBitmap, sizeof(bm), &bm);

	return SIZE{ bm.bmWidth, bm.bmHeight };
}
