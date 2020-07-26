#include "stdafx.h"
#include "ImageObject.h"

CImageObject::CImageObject():
	m_wstrObjectKey(L""),
	m_wstrStateKey(L""),
	m_bIsDead(false),
	m_fSort(1.f),
	m_eImgObjType(IMG_OBJ_END),
	m_bRenderListPush(false),
	m_fDegree(0.f)
{
}

CImageObject::~CImageObject()
{
}