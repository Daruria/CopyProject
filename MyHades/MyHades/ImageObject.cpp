#include "stdafx.h"
#include "ImageObject.h"

CImageObject::CImageObject():
	m_wstrObjectKey(L""),
	m_wstrStateKey(L""),
	m_fSort(1.f),
	m_bIsDead(false),
	m_eImgObjType(CImageObject::IMG_OBJ_END),
	m_d3dColor(D3DCOLOR_ARGB(255, 255, 255, 255))
{
}

CImageObject::~CImageObject()
{
}