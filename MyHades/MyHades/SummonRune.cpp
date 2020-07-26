#include "stdafx.h"
#include "SummonRune.h"
#include "Monster.h"

CSummonRune::CSummonRune():
	m_bDidMonsterPush(false),
	m_pMonster(nullptr)
{
}


CSummonRune::~CSummonRune()
{
	Release();
}

int CSummonRune::Update()
{
	if (m_bIsDead)
	{
		m_pObjectMgr->AddObject(OBJECT_MONSTER, m_pMonster);
		m_bDidMonsterPush = true;
		return DEAD_OBJ;
	}

	Animation();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CSummonRune::LateUpdate()
{
	CEffect::LateUpdate();
}

void CSummonRune::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"EnemySummonRune", m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = {pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height* 0.5f, 0.f};
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);

	if (m_tFrame.fCurFrame >= 13)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"EnemySummonSmokeBack", m_tFrame.fCurFrame - 13.f);
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height* 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
	}

	if (m_tFrame.fCurFrame >= 12)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"EnemySummonBackFlame", m_tFrame.fCurFrame - 12.f);
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height* 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
	}

	if (m_tFrame.fCurFrame >= 7)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"EnemySummonSmokeFront", m_tFrame.fCurFrame - 7.f);
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height* 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
	}



}

HRESULT CSummonRune::Initialize()
{
	m_tInfo.vDir = {1.f, 0.f, 0.f};
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_bIsDead = false;
	m_wstrObjectKey = L"Fx";
	
	m_d3dColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fFrameTime = 0.f;

	return S_OK;
}

void CSummonRune::Release()
{
	if (!m_bDidMonsterPush)
		SafeDelete(m_pMonster);
}

CSummonRune * CSummonRune::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, CMonster * pMonster)
{
	CSummonRune* pInst = new CSummonRune;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_pMonster = pMonster;
	
	

	return pInst;
}
