#include "stdafx.h"
#include "Shield.h"


CShield::CShield():
	m_eShieldType(CShield::END)
{
}

CShield::~CShield()
{
	Release();
}

void CShield::Initialize()
{

}

int CShield::Update()
{
	
	return 0;
}

void CShield::LateUpdate()
{
}

void CShield::Render(HDC hDC)
{

}

void CShield::Release()
{
}

void CShield::DiscRender(HDC hDC, float fx, float fy)
{

}
