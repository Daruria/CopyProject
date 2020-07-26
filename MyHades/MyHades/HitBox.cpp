#include "stdafx.h"
#include "HitBox.h"


CHitBox::CHitBox():
	m_pParent(nullptr),
	m_fTimeCount(0.f),
	m_fLifeTime(0.f),
	m_iAttack(0),
	m_eHitBoxType(CHitBox::HIT_BOX_END),
	m_pObjectMgr(CObjectMgr::GetInstance())
{
	ZeroMemory(&m_vSize, sizeof(D3DXVECTOR3));
}


CHitBox::~CHitBox()
{
}

int CHitBox::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();

	if (m_fTimeCount >= m_fLifeTime)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CHitBox::LateUpdate()
{
}

void CHitBox::Render()
{
#ifdef _DEBUG
	CMathMgr::DrawLineBox(m_pDeviceMgr, m_tInfo.vPos, m_vSize, 2.f, D3DCOLOR_ARGB(255, 255, 0, 0));
#endif // _DEBUG
}

HRESULT CHitBox::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 0.f };

	m_fTimeCount = 0.f;

	return S_OK;
}

void CHitBox::Release()
{
}

float CHitBox::GetLayerStd()
{
	return 0.0f;
}

void CHitBox::Hit()
{
	m_fTimeCount = m_fLifeTime;
}

void CHitBox::Hit(const D3DXVECTOR3 & vPos)
{
	
}

const RECT CHitBox::GetRect() const
{
	return RECT{ 
		static_cast<LONG>(m_tInfo.vPos.x - m_vSize.x * 0.5f), 
		static_cast<LONG>(m_tInfo.vPos.y - m_vSize.y * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.x + m_vSize.x * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y + m_vSize.y * 0.5f)
	};
}
