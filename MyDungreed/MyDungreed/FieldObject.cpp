#include "stdafx.h"
#include "FieldObject.h"


CFieldObject::CFieldObject():
	m_eFieldObjectType(FOT_END)
{
}


CFieldObject::~CFieldObject()
{
	Release();
}

void CFieldObject::Initialize()
{
}

int CFieldObject::Update()
{
	return 0;
}

void CFieldObject::LateUpdate()
{
}

void CFieldObject::Render(HDC hDC)
{
}

void CFieldObject::Release()
{
}
