#include "stdafx.h"
#include "Weapon.h"


CWeapon::CWeapon()	:
	m_bIsSkill(false),
	m_bCanUseSkill(true),
	m_bAttack(false),
	m_bRenderBack(false),
	m_strKey(""),
	m_iAttack(0),
	m_dwSkillCool(0),
	m_dwSkillCoolMax(0),
	m_dwAttackCool(0),
	m_bEquip(true),
	m_tAttackBox(INFO{})
{
}


CWeapon::~CWeapon()
{
}

void CWeapon::SetImage(const string & strKey, const wchar_t * pPath)
{
	m_strKey = strKey;
	INSERT_BITMAP(strKey, pPath);
	SIZE tSize = GET_BITMAP_SIZE(strKey);
	m_tInfo.icx = tSize.cx;
	m_tInfo.icy = tSize.cy;

}

bool CWeapon::Attack()
{
	return true;
}

bool CWeapon::Attack(float fAttack)
{
	return false;
}

void CWeapon::AddAttackEffect(float fx, float fy)
{
}

void CWeapon::UseSkill()
{
	m_dwSkillCool = GetTickCount();
	m_bCanUseSkill = false;
}

void CWeapon::Initialize()
{
	INSERT_BITMAP("ItemDiscWind", L"Image/Shop/ItemDiscWind.bmp");
	INSERT_BITMAP("IconWhite", L"Image/Icon/IconWhite.bmp");
}

int CWeapon::Update()
{
	if (m_dwSkillCool + m_dwSkillCoolMax < GetTickCount())
	{
		m_bCanUseSkill = true;
	}

	return 0;
}

void CWeapon::LateUpdate()
{
}

void CWeapon::Render(HDC hDC)
{
#ifdef _DEBUG
	if (m_bAttack)
	{
		POSITION tPos = CScrollManager::WorldToScreen(m_tAttackBox);
		CMath::DrawBox(hDC,
			static_cast<int>(tPos.fx - m_tAttackBox.icx * 0.5f),
			static_cast<int>(tPos.fy - m_tAttackBox.icy * 0.5f),
			m_tAttackBox.icx, m_tAttackBox.icy);
	}
#endif // _DEBUG

}

void CWeapon::Release()
{
}

void CWeapon::DiscRender(HDC hDC, float fx, float fy)
{
}
