#include "stdafx.h"
#include "Boom.h"
#include "NormalHitBox.h"

CBoom::CBoom():
	m_fTimeCount(0.f),
	m_fBoomTime(0.f)
{
}


CBoom::~CBoom()
{
	Release();
}

int CBoom::Update()
{
	switch (m_eCurState)
	{
	case CBoom::PREVIEW:
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fBoomTime)
		{
			m_eCurState = CBoom::FIRE;
			CNormalHitBox* pHitBox = CNormalHitBox::Create(m_tInfo.vPos, D3DXVECTOR3(300.f, 150.f, 0.f), this, 0.1f, 10);
			m_pObjectMgr->AddObject(OBJECT_MONSTER_HITBOX, pHitBox);

			CSoundMgr::GetInstance()->PlaySound(L"HellFireImpact1.wav", CSoundMgr::EFFECT);
		}
		break;
	case CBoom::FIRE:
		if (m_bOnceAnimFinish)
		{
			CScrollMgr::Shaking();
			return DEAD_OBJ;
		}
		Animation();
		break;
	}

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);
	

	return NO_EVENT;
}

void CBoom::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CBoom::Render()
{
	const TEX_INFO* pTexInfo = nullptr;
	D3DXVECTOR3 vCenter;
	switch (m_eCurState)
	{
	case CBoom::PREVIEW:
		pTexInfo = m_pTextureMgr->GetTexInfo(L"FuryHellfirePreviewDecal");
		NULL_CHECK(pTexInfo);
		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		break;
	case CBoom::FIRE:

		if (m_tFrame.fCurFrame < 44.f)
		{
			pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"RadialNova", static_cast<WORD>(m_tFrame.fCurFrame));
			NULL_CHECK(pTexInfo);
			vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };


			D3DXMATRIX matScale, matTrans;
			D3DXMatrixScaling(&matScale, 1.f, 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);

			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 0, 255));
		}
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"FuryHellfire", static_cast<WORD>(m_tFrame.fCurFrame));
		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.75f, 0.f };
		break;
	}
	

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	

}

HRESULT CBoom::Initialize()
{
	HRESULT hr = CMonster::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_fBoomTime = 1.f;

	m_eCurState = CBoom::PREVIEW;

	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 52.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;


	return S_OK;
}

void CBoom::Release()
{

}

CBoom * CBoom::Create(const D3DXVECTOR3 & vPos)
{
	CBoom* pInst = new CBoom;
	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;

	return pInst;
}
