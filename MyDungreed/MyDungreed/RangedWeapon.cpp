#include "stdafx.h"
#include "RangedWeapon.h"


CRangedWeapon::CRangedWeapon():
	m_bReload(false),
	m_iRemainBullet(0),
	m_iMagazine(0),
	m_fAngle(0.f),
	m_dwReloadingTime(0),
	m_dwReloadTime(0)
{
}


CRangedWeapon::~CRangedWeapon()
{
	Release();
}

bool CRangedWeapon::Attack()
{
	return false;
}

bool CRangedWeapon::Attack(float fAttack)
{

	return false;
}

void CRangedWeapon::AddAttackEffect(float fx, float fy)
{
}

void CRangedWeapon::UseSkill()
{
	CWeapon::UseSkill();
}

void CRangedWeapon::Initialize()
{
}

int CRangedWeapon::Update()
{
	CWeapon::Update();
	return 0;
}

void CRangedWeapon::LateUpdate()
{
}

void CRangedWeapon::Render(HDC hDC)
{
}

void CRangedWeapon::Release()
{
}

void CRangedWeapon::DiscRender(HDC hDC, float fx, float fy)
{
}

void CRangedWeapon::SetLeftImage(const string & strLeftKey, const wchar_t * pPath)
{
	m_strLeftKey = strLeftKey;
	INSERT_BITMAP(m_strLeftKey, pPath);
}

void CRangedWeapon::Reload()
{
	DWORD dwCurTime = GetTickCount();

	m_fReloadRatio = float(dwCurTime - m_dwReloadingTime) / m_dwReloadTime;

}

void CRangedWeapon::Shoot()
{
	m_dwAttackCool = GetTickCount();
	m_bAttack = true;

	--m_iRemainBullet;
}

void CRangedWeapon::Shoot(float fAttack)
{
}
