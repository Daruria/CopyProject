#include "stdafx.h"
#include "BossObserver.h"


CBossObserver::CBossObserver()
{
	ZeroMemory(&m_tStatus, sizeof(STATUS));
}

CBossObserver::~CBossObserver()
{
}

void CBossObserver::Update(int iMsg, void * pData)
{
	switch (iMsg)
	{
	case HP:
		m_tStatus.iHp = *reinterpret_cast<int*>(pData);
		break;
	case MAX_HP:
		m_tStatus.iMaxHp = *reinterpret_cast<int*>(pData);
		break;
	case ATK:
		m_tStatus.iAttack = *reinterpret_cast<int*>(pData);
		break;
	case NAME:
		m_wstrNameKey = *reinterpret_cast<const wstring*>(pData);
		break;
	}
}
