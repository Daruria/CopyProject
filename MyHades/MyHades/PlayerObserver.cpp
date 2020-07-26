#include "stdafx.h"
#include "PlayerObserver.h"


CPlayerObserver::CPlayerObserver():
	m_iWrathMaxPoint(0),
	m_iWrathPoint(0),
	m_bCanReflectSkill(false),
	m_bCanDashSkill(false),
	m_bCanCall(false)
{
	ZeroMemory(&m_tStatus, sizeof(STATUS));
}

CPlayerObserver::~CPlayerObserver()
{
}

void CPlayerObserver::Update(int iMsg, void * pData)
{
	switch (iMsg)
	{
	case CPlayerObserver::HP:
		m_tStatus.iHp = *reinterpret_cast<int*>(pData);
		break;
	case CPlayerObserver::MAX_HP:
		m_tStatus.iMaxHp = *reinterpret_cast<int*>(pData);
		break;
	case CPlayerObserver::ATK:
		m_tStatus.iAttack = *reinterpret_cast<int*>(pData);
		break;
	case CPlayerObserver::WRATH_POINT:
		m_iWrathPoint = *reinterpret_cast<int*>(pData);
		break;
	case CPlayerObserver::WRATH_MAX_POINT:
		m_iWrathMaxPoint = *reinterpret_cast<int*>(pData);
		break;
	case CPlayerObserver::REFLECT_SKILL:
		m_bCanReflectSkill = *reinterpret_cast<bool*>(pData);
		break;
	case CPlayerObserver::DASH_SKILL:
		m_bCanDashSkill = *reinterpret_cast<bool*>(pData);
		break;
	case CPlayerObserver::CALL:
		m_bCanCall		= *reinterpret_cast<bool*>(pData);
		break;
	}
}