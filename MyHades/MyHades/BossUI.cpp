#include "stdafx.h"
#include "BossUI.h"
#include "BossObserver.h"

#define BOSS_HP_BAR_X WINCX * 0.5f
#define BOSS_HP_BAR_Y 100.f

CBossUI::CBossUI():
	m_pSubjectMgr(CSubjectMgr::GetInstance()),
	m_pObserver(nullptr),
	m_wstrBossHp(L""),
	m_bIsDead(false),
	m_bRender(false),
	m_fSpeed(0.f),
	m_eShowState(CBossUI::SHOW_END)
{
	ZeroMemory(&m_vHpBarPos, sizeof(D3DXVECTOR3));
}


CBossUI::~CBossUI()
{
	Release();
}

int CBossUI::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;
	
	switch (m_eShowState)
	{
	case CBossUI::COME_IN:
	{
		bool bHpBar = ComeInHpBar();
		if (bHpBar)
			m_eShowState = CBossUI::SHOW;
	}
		m_pObjectMgr->AddLayer(RENDER_UI, this);
		break;
	case CBossUI::SHOW:
		m_pObjectMgr->AddLayer(RENDER_UI, this);
		break;
	case CBossUI::GO_OUT:
		break;
	case CBossUI::NO_SHOW:
		break;
	}

	return NO_EVENT;
}

void CBossUI::LateUpdate()
{
}

void CBossUI::Render()
{
	D3DXMATRIX matScale, matRotateZ, matTrans, matWorld;

	//	Boss Hp Bar Frame
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixIdentity(&matRotateZ);
	D3DXMatrixTranslation(&matTrans, m_vHpBarPos.x, m_vHpBarPos.y, 0.f);

	matWorld = matScale * matRotateZ * matTrans;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"EnemyHealthBarBoss");
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//	Boss Hp Gage
	m_iBossHp = m_pObserver->GetBossHp();
	m_iBossMaxHp = m_pObserver->GetBossMaxHp();

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vHpBarPos.x, m_vHpBarPos.y, 0.f);

	matWorld = matScale * matTrans;

	pTexInfo = m_pTextureMgr->GetTexInfo(L"EnemyHealthBarFillBoss");
	NULL_CHECK(pTexInfo);

	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	float fRatio = static_cast<float>(m_iBossHp) / m_iBossMaxHp;
	RECT rc = { 0, 0, pTexInfo->tImgInfo.Width * fRatio, pTexInfo->tImgInfo.Height };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 0, 0));


	//	Boss Name
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vHpBarPos.x, m_vHpBarPos.y - 50.f, 0.f);

	matWorld = matScale * matTrans;

	wstring wstrNameKey = m_pObserver->GetNameKey();
	pTexInfo = m_pTextureMgr->GetTexInfo(wstrNameKey);
	NULL_CHECK(pTexInfo);

	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

HRESULT CBossUI::Initialize()
{
	m_pObserver = CBossObserver::Create();
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);
	
	m_pSubjectMgr->Subscribe(L"Boss", m_pObserver);

	m_vHpBarPos = { BOSS_HP_BAR_X, BOSS_HP_BAR_Y - 200.f, 0.f };

	m_eShowState = CBossUI::NO_SHOW;

	m_fSpeed = 200.f;

	return S_OK;
}

void CBossUI::Release()
{
	m_pSubjectMgr->Unsubscribe(L"Boss", m_pObserver);
	SafeDelete(m_pObserver);
}

CBossUI * CBossUI::Create()
{
	CBossUI* pInst = new CBossUI;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	return pInst;
}

float CBossUI::GetLayerStd()
{
	return 0.0f;
}

void CBossUI::Die()
{
	m_bIsDead = true;
}

void CBossUI::UIComeIn()
{
	switch (m_eShowState)
	{
	case CBossUI::COME_IN:
	case CBossUI::SHOW:
		return;
	case CBossUI::GO_OUT:
	case CBossUI::NO_SHOW:
		m_eShowState = CBossUI::COME_IN;
		break;
	}
}

bool CBossUI::ComeInHpBar()
{
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };

	vDir.x = BOSS_HP_BAR_X - m_vHpBarPos.x;
	vDir.y = BOSS_HP_BAR_Y - m_vHpBarPos.y;

	bool bX, bY;

	if (fabsf(vDir.x) < 10.f)
	{
		bX = true;
		m_vHpBarPos.x = BOSS_HP_BAR_X;
	}
	else
		bX = false;

	if (fabsf(vDir.y) < 10.f)
	{
		bY = true;
		m_vHpBarPos.y = BOSS_HP_BAR_Y;
	}
	else
		bY = false;

	if (bX && bY)
		return true;

	if (D3DXVec3Length(&vDir) > 1.f)
		D3DXVec3Normalize(&vDir, &vDir);

	m_vHpBarPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;

	return false;
}