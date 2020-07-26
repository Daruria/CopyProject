#include "stdafx.h"
#include "Beam.h"
#include "Player.h"
#include "LineHitBox.h"
#include "RevolutionEffect.h"
#include "Crystal.h"

CBeam::CBeam():
	m_pParentMon(nullptr),
	m_fTimeCount(0.f),
	m_fLifeTime(0.f),
	m_pLineHitBox(nullptr),
	m_pCloseEffect(nullptr),
	m_pFarEffect(nullptr),
	m_bPreFrameColl(false)
{
}


CBeam::~CBeam()
{
	Release();
}

int CBeam::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();

	if (m_fTimeCount >= m_fLifeTime)
		m_bIsDead = true;

	if (m_pParentMon->GetStatus().iHp <= 0)
		m_bIsDead = true;

	if (m_bIsDead)
	{
		dynamic_cast<CCrystal*>(m_pParentMon)->SetBeam(nullptr);

		if (m_pLineHitBox)
			m_pLineHitBox->Die();

		if (m_pCloseEffect)
			m_pCloseEffect->Die();

		if (m_pFarEffect)
			m_pFarEffect->Die();

		return DEAD_OBJ;
	}

	if (!m_bPreFrameColl)
		m_fLengthRatio = 1.f;

	m_tInfo.vLook = m_pParentMon->GetInfo().vLook;

	Animation();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CBeam::LateUpdate()
{
	m_bPreFrameColl = false;

	D3DXMATRIX matScale, matRotateZ, matTrans, matRevolZ;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixIdentity(&matRotateZ);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRevolZ, -CMathMgr::CalculateAngleMPIToPI(m_tInfo.vLook, D3DXVECTOR3(1.f, 0.f, 0.f)));

	m_tInfo.matWorld = matScale * matRotateZ * matTrans * matRevolZ * m_pParentMon->GetInfo().matWorld;
}

void CBeam::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"EnemyLaser", static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	RECT rc = {0, 0, static_cast<LONG>(pTexInfo->tImgInfo.Width * m_fLengthRatio), pTexInfo->tImgInfo.Height};
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//vPos.x += pTexInfo->tImgInfo.Width * 0.86f;
	//m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
	//vPos.x += pTexInfo->tImgInfo.Width * 0.86f;
	//m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));


}

HRESULT CBeam::Initialize()
{
	
	m_tInfo.vPos = { 171 * 1.5f, 0.f, 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 3.f, 1.f, 1.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_bIsDead = false;

	m_tFrame.eType = ANIM_REPEAT;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 20.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;

	m_fLifeTime = 4.f;

	return S_OK;
}

void CBeam::Release()
{
}

CBeam * CBeam::Create(CMonster * pParent)
{
	CBeam* pInst = new CBeam;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_pParentMon = pParent;
	pInst->m_tInfo.vLook = pParent->GetInfo().vLook;

	float fAngle = -CMathMgr::CalculateAngleMPIToPI(pInst->m_tInfo.vLook, D3DXVECTOR3(1.f, 0.f, 0.f));
	CLineHitBox* pLineHitBox = CLineHitBox::Create(pParent->GetInfo().vPos, 171 * 3.f, fAngle, pParent->GetStatus().iAttack, 5.f, pInst);
	pInst->m_pLineHitBox = pLineHitBox;
	pInst->m_pObjectMgr->AddObject(OBJECT_MONSTER_HITBOX, pLineHitBox);

	return pInst;
}

void CBeam::Collision(float fLength)
{
	float fOriginLength = 171 * 3.f;
	m_fLengthRatio = fLength / fOriginLength;
	//m_fLengthRatio /= 3.f;
	m_bPreFrameColl = true;

}
