#include "stdafx.h"
#include "FieldItem.h"
#include "Item.h"

CFieldItem::CFieldItem():
	m_bIsStop(false)
{
}


CFieldItem::~CFieldItem()
{
	Release();
}

void CFieldItem::Initialize()
{
	CMoveObj::Initialize();

	m_iFrameDir = 1;
	m_dwFrameMaxTime = 100;

	m_fJumpForce = 10.f;
	m_fGravityTime = 0.f;
	m_bJump = true;
	m_bPhysics = true;
	m_bPassible = false;
	m_bLife = true;
	m_fSpeed = (rand() % 100) / 20.f;
	m_tDir.fx = rand() % 10 - 5.f;
	m_tDir.fy = static_cast<float>(rand() % 5);
	CMath::NormalizeXY(m_tDir.fx, m_tDir.fy);
}

int CFieldItem::Update()
{
	CMoveObj::Update();

	m_tInfo.fx += m_tDir.fx * m_fSpeed;
	m_tInfo.fy -= m_tDir.fy * m_fSpeed;

	if (m_bJump)	Jump();
	if (m_bPhysics) Gravity();

	return 0;
}

void CFieldItem::LateUpdate()
{
	CMoveObj::LateUpdate();
}

void CFieldItem::Render(HDC hDC)
{
	CMoveObj::Render(hDC);

#ifdef _DEBUG
	POSITION tPos = CScrollManager::WorldToScreen(GetHitBoxWorldInfo());
	CMath::DrawBox(hDC, static_cast<int>(tPos.fx - m_tHitBox.icx / 2), static_cast<int>(tPos.fy - m_tHitBox.icy / 2), m_tHitBox.icx, m_tHitBox.icy);

#endif // _DEBUG
}

void CFieldItem::Release()
{
	if (m_pItem && m_pItem->GetType() == IT_GOLD_COIN)
		SAFE_DELETE(m_pItem);

	if (m_bLife)
		SAFE_DELETE(m_pItem);
}

void CFieldItem::Stop()
{
	m_bIsStop = true;
	m_fGravityTime = 0.f;
	m_fJumpForce *= 0.f;
	m_fSpeed *= 0.f;
}
