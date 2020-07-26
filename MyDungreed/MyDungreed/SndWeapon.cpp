#include "stdafx.h"
#include "SndWeapon.h"


CSndWeapon::CSndWeapon():
	m_fAttack(0.f)
{
}

CSndWeapon::~CSndWeapon()
{
	Release();
}

void CSndWeapon::Initialize()
{
	m_eType = IT_SHIELD;
	m_eShieldType = CShield::SECONDARY_WEAPON;



}

int CSndWeapon::Update()
{
	return 0;
}

void CSndWeapon::LateUpdate()
{
}

void CSndWeapon::Render(HDC hDC)
{
}

void CSndWeapon::Release()
{
}

void CSndWeapon::DiscRender(HDC hDC, float fx, float fy)
{
}
