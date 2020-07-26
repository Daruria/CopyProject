#pragma once

#include "GameObject.h"
#include "ImageObject.h"

class CLayerSort
{
public:
	bool operator()(CGameObject* pSrc, CGameObject* pDest)
	{
		return pSrc->GetInfo().vPos.y < pDest->GetInfo().vPos.y;
	}
};

class CLayerSortStd
{
public:
	bool operator()(CGameObject* pSrc, CGameObject* pDest)
	{
		return pSrc->GetLayerStd() < pDest->GetLayerStd();
	}
};

class CImageObjectSort
{
public:
	bool operator()(CImageObject* pSrc, CImageObject* pDest)
	{
		return pSrc->GetSort() < pDest->GetSort();
	}
};