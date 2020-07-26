#include "stdafx.h"
#include "PreBeam.h"
#include "Player.h"
#include "Crystal.h"

CPreBeam::CPreBeam():
	m_pParentMon(nullptr)
{
}

CPreBeam::~CPreBeam()
{
	Release();
}

int CPreBeam::Update()
{
	if (m_bOnceAnimFinish)
		m_bIsDead = true;

	if (m_pParentMon->GetStatus().iHp <= 0)
		m_bIsDead = true;

	if (m_bIsDead)
	{
		dynamic_cast<CCrystal*>(m_pParentMon)->SetBeam(nullptr);
		return DEAD_OBJ;
	}

	m_tInfo.vLook = m_pParentMon->GetInfo().vLook;

	Animation();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CPreBeam::LateUpdate()
{
	D3DXMATRIX matScale, matRotateZ, matTrans, matRevolZ;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixIdentity(&matRotateZ);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	float fAngle = CMathMgr::CalculateAngleMPIToPI(m_tInfo.vLook, D3DXVECTOR3(1.f, 0.f, 0.f));
	D3DXMatrixRotationZ(&matRevolZ, -fAngle);

	m_tInfo.matWorld = matScale * matRotateZ * matTrans * matRevolZ * m_pParentMon->GetInfo().matWorld;
}

void CPreBeam::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"EnemyLaserPreview", static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


}

HRESULT CPreBeam::Initialize()
{
	m_tInfo.vPos = { 250.f, 0.f, 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_bIsDead = false;

	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;


	return S_OK;
}

void CPreBeam::Release()
{
}

CPreBeam * CPreBeam::Create(CMonster* pParent)
{
	CPreBeam* pInst = new CPreBeam;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_pParentMon = pParent;
	pInst->m_tInfo.vLook = pParent->GetInfo().vLook;

	return pInst;
}
