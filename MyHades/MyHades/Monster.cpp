#include "stdafx.h"
#include "Monster.h"


CMonster::CMonster():
	m_bIsDead(false),
	m_wstrObjectKey(L"")
{
	ZeroMemory(&m_tStatus, sizeof(STATUS));
}

CMonster::~CMonster()
{
}

int CMonster::Update()
{
	return 0;
}

void CMonster::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMonster::Render()
{
}

HRESULT CMonster::Initialize()
{
	HRESULT hr = CMoveObject::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_bIsDead = false;
	m_wstrObjectKey = L"";
	m_tStatus.iAttack = 10;
	m_tStatus.iHp = 10;
	m_tStatus.iMaxHp = 10;

	return S_OK;
}

void CMonster::Release()
{
}

void CMonster::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	CMoveObject::Hit(vPos, iAttack);

	m_tStatus.iHp -= iAttack;
	if (m_tStatus.iHp < 0)
		m_tStatus.iHp = 0;
}
