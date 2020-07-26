#include "stdafx.h"
#include "StaticObject.h"


CStaticObject::CStaticObject() :
	m_pObjectMgr(CObjectMgr::GetInstance())
{
}


CStaticObject::~CStaticObject()
{
}
