#include "stdafx.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "GameObject.h"

D3DXVECTOR3 CScrollMgr::m_vScrollPos = { 0.f, 0.f, 0.f };
D3DXVECTOR3 CScrollMgr::m_vScrollShake = { 0.f, 5.f, 0.f };
bool		CScrollMgr::m_bShaking = false;
float		CScrollMgr::m_fShakeTime = 0.f;

CScrollMgr::CScrollMgr()
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scrolling(CGameObject* pTargetObj)
{
	NULL_CHECK(pTargetObj);

	m_vScrollPos = { pTargetObj->GetInfo().vPos.x - WINCX * 0.5f, pTargetObj->GetInfo().vPos.y - WINCY * 0.5f, 0.f};

	if (!m_bShaking)
	{
		m_vScrollPos = { pTargetObj->GetInfo().vPos.x - WINCX * 0.5f, pTargetObj->GetInfo().vPos.y - WINCY * 0.5f, 0.f };
	}
	else
	{
		m_fShakeTime += CTimeMgr::GetInstance()->GetDelta();

		if (m_fShakeTime >= 0.5f)
		{
			m_fShakeTime = 0.f;
			m_bShaking = false;
			return;
		}

		m_vScrollPos = { pTargetObj->GetInfo().vPos.x - WINCX * 0.5f, pTargetObj->GetInfo().vPos.y - WINCY * 0.5f, 0.f };
		m_vScrollShake = -m_vScrollShake;
		m_vScrollPos += m_vScrollShake;
	}
}

void CScrollMgr::Scrolling(const D3DXVECTOR3 & vPos)
{
	if (!m_bShaking)
	{
		m_vScrollPos = { vPos.x - WINCX * 0.5f, vPos.y - WINCY * 0.5f, 0.f };
	}
	else
	{
		m_fShakeTime += CTimeMgr::GetInstance()->GetDelta();

		if (m_fShakeTime >= 0.5f)
		{
			m_fShakeTime = 0.f;
			m_bShaking = false;
			return;
		}

		m_vScrollPos = { vPos.x - WINCX * 0.5f, vPos.y - WINCY * 0.5f, 0.f };
		m_vScrollShake = -m_vScrollShake;
		m_vScrollPos += m_vScrollShake;
	}
}

void CScrollMgr::Shaking()
{
	m_bShaking = true;
	m_fShakeTime = 0.f;
}
