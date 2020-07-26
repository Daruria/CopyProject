#include "stdafx.h"
#include "Mine.h"
#include "Player.h"
#include "Effect.h"
#include "NormalHitBox.h"

CMine::CMine():
	m_ePreState(CMine::END),
	m_eCurState(CMine::END),
	m_bLightRender(false),
	m_fLightCount(0.f),
	m_fLightTime(0.f),
	m_fReadyTime(0.f),
	m_fReadyCount(0.f)
{
}


CMine::~CMine()
{
	Release();
}

int CMine::Update()
{
	if (m_tStatus.iHp <= 0)
		m_bIsDead = true;

	if (m_bIsDead) 
		return DEAD_OBJ;

	ChangeState();

	StateUpdate();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CMine::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMine::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"BloodMineBase");
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width*0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bLightRender)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"BloodMineLight");
		NULL_CHECK(pTexInfo);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

#ifdef _DEBUG
	CMathMgr::DrawLineBox(m_pDeviceMgr, m_tInfo.vPos + m_tHitBox.vOffset - CScrollMgr::GetScrollPos(), m_tHitBox.vSize, 2.f, D3DCOLOR_ARGB(255, 255, 0, 255));
#endif //_DEBUG

}

HRESULT CMine::Initialize()
{
	HRESULT hr = CMonster::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);
	
	m_eCurState = CMine::IDLE;
	m_fReadyTime = 2.f;
	m_fLightTime = 0.1f;

	m_tInfo.vScale = {0.5f, 0.5f, 0.f};

	return S_OK;
}

void CMine::Release()
{
}

CMine * CMine::Create(const D3DXVECTOR3 & vPos)
{
	CMine* pInst = new CMine;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;

	return pInst;
}

void CMine::ChangeState()
{
	if (m_ePreState == m_eCurState)
		return;

	m_ePreState = m_eCurState;
}

void CMine::StateUpdate()
{
	switch (m_eCurState)
	{
	case CMine::IDLE:
		IdleUpdate();
		break;
	case CMine::READY:
		ReadyUpdate();
		break;
	case CMine::FIRE:
		FireUpdate();
		break;
	case CMine::DIE:
		DieUpdate();
		break;
	case CMine::END:
		break;
	}


}

void CMine::IdleUpdate()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	
	POINT pt;
	pt.x = static_cast<LONG>(pPlayer->GetInfo().vPos.x);
	pt.y = static_cast<LONG>(pPlayer->GetInfo().vPos.y);
	
	RECT rc = {
		static_cast<LONG>(m_tInfo.vPos.x - 200.f),
		static_cast<LONG>(m_tInfo.vPos.y - 100.f),
		static_cast<LONG>(m_tInfo.vPos.x + 200.f),
		static_cast<LONG>(m_tInfo.vPos.y + 100.f)
	};

	if (PtInRect(&rc, pt))
	{
		m_eCurState = CMine::READY;

		m_bLightRender = true;
		
		INFO tInfo = m_tInfo;
		tInfo.vScale = {2.f, 2.f, 0.f};
		CEffect* pEffect = CEffect::Create(tInfo, L"Fx", L"MeteorTarget", 32, 1 / 30.f, m_fReadyTime, ANIM_REPEAT, D3DCOLOR_ARGB(255, 255, 255, 255));
		NULL_CHECK(pEffect);

		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

		CSoundMgr::GetInstance()->PlaySound(L"BloodMineBeepRising.wav", CSoundMgr::MONSTER);
	}
}

void CMine::ReadyUpdate()
{

	m_fLightCount += m_pTimeMgr->GetDelta();
	if (m_fLightCount >= m_fLightTime)
	{
		m_fLightCount = 0.f;
		m_bLightRender = !m_bLightRender;
	}

	m_fReadyCount += m_pTimeMgr->GetDelta();
	if (m_fReadyCount >= m_fReadyTime)
	{
		m_eCurState = CMine::FIRE;
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::GetInstance()->PlaySound(L"ExplosionVariants_8.wav", CSoundMgr::EFFECT);
		
	}

}
void CMine::FireUpdate()
{
	m_bIsDead = true;

	INFO tInfo = m_tInfo;
	tInfo.vScale = { 1.f, 1.f, 0.f };
	CEffect* pEffect = CEffect::Create(tInfo, L"Fx", L"MineExplosion", 30, 1 / 30.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255));
	NULL_CHECK(pEffect);

	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

	CNormalHitBox* pHitBox = CNormalHitBox::Create(m_tInfo, D3DXVECTOR3(400.f, 200.f, 0.f), nullptr, 0.5f, 10);
	NULL_CHECK(pHitBox);

	m_pObjectMgr->AddObject(OBJECT_MONSTER_HITBOX, pHitBox);
	CScrollMgr::Shaking();
}

void CMine::DieUpdate()
{
}