#include "stdafx.h"
#include "Spawner.h"
#include "Swarmer.h"
#include "Effect.h"

CSpawner::CSpawner():
	m_ePreState(CSpawner::END),
	m_eCurState(CSpawner::END),
	m_wstrStateKey(L""),
	m_bSpawn(false)
{
}


CSpawner::~CSpawner()
{
	Release();
}

int CSpawner::Update()
{
	if (m_tStatus.iHp <= 0)
	{
		CEffect* pEffect = CEffect::Create(m_tInfo, m_wstrObjectKey, L"DeathVFX", 33, 1 / 30.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
		m_bIsDead = true;
		CSoundMgr::GetInstance()->PlaySound(L"LightSpawner_EmoteDying_10.wav", CSoundMgr::EFFECT);
	}

	if (m_bIsDead)
		return DEAD_OBJ;

	ChangeState();

	StateUpdate();

	Animation();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CSpawner::LateUpdate()
{
	CMonster::LateUpdate();

}

void CSpawner::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, 
		static_cast<WORD>(m_tFrame.fCurFrame) + static_cast<int>(m_vecStartFrame[m_eCurState]));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _DEBUG
	CMathMgr::DrawLineBox(m_pDeviceMgr, m_tInfo.vPos + m_tHitBox.vOffset - CScrollMgr::GetScrollPos(), m_tHitBox.vSize, 2.f, D3DCOLOR_ARGB(255, 255, 0, 255));
#endif //_DEBUG


}

HRESULT CSpawner::Initialize()
{
	HRESULT hr = CMonster::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_wstrObjectKey = L"LightSpawner";
	m_eCurState = CSpawner::APEAR;

	m_vecStartFrame.resize(CSpawner::END);

	m_vecStartFrame[CSpawner::APEAR] = 0.f;
	m_vecStartFrame[CSpawner::IDLE] = 25.f;
	m_vecStartFrame[CSpawner::SPAWN] = 50.f;
	m_vecStartFrame[CSpawner::DIE] = 0.f;

	
	return S_OK;
}

void CSpawner::Release()
{
}

CSpawner * CSpawner::Create(const D3DXVECTOR3 vPos)
{
	CSpawner* pInst = new CSpawner;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;

	return pInst;
}

void CSpawner::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	CMonster::Hit(vPos, iAttack);

	CSoundMgr::GetInstance()->PlaySound(L"BloodSplatter combo.wav", CSoundMgr::EFFECT);
}

void CSpawner::ChangeState()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CSpawner::APEAR:
		ApearState();
		break;
	case CSpawner::IDLE:
		IdleState();
		break;
	case CSpawner::SPAWN:
		SpawnState();
		break;
	case CSpawner::DIE:
		DieState();
		break;
	}

	m_ePreState = m_eCurState;
}

void CSpawner::ApearState()
{
	m_wstrStateKey = L"Idle";
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 25.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CSpawner::IdleState()
{
	m_wstrStateKey = L"Idle";
	m_tFrame.eType = ANIM_REPEAT;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 25.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CSpawner::SpawnState()
{
	m_wstrStateKey = L"Idle";
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 50.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CSpawner::DieState()
{
	m_wstrStateKey = L"DeathVFX";
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 33.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CSpawner::StateUpdate()
{
	switch (m_eCurState)
	{
	case CSpawner::APEAR:
		ApearUpdate();
		break;
	case CSpawner::IDLE:
		IdleUpdate();
		break;
	case CSpawner::SPAWN:
		SpawnUpdate();
		break;
	case CSpawner::DIE:
		DieUpdate();
		break;
	}
}

void CSpawner::ApearUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_eCurState = CSpawner::IDLE;
		m_bOnceAnimFinish = false;
	}
}

void CSpawner::IdleUpdate()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->size() < 10)
	{
		m_eCurState = CSpawner::SPAWN;
	}
}

void CSpawner::SpawnUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_eCurState = CSpawner::IDLE;
		m_bOnceAnimFinish = false;
	}

	if (!m_bSpawn && 39 == static_cast<int>(m_tFrame.fCurFrame))
	{
		CSwarmer* pSwarmer = CSwarmer::Create(m_tInfo.vPos);
		NULL_CHECK(pSwarmer);

		m_pObjectMgr->AddObject(OBJECT_MONSTER, pSwarmer);
		m_bSpawn = true;
	}
	else if (40 == static_cast<int>(m_tFrame.fCurFrame))
		m_bSpawn = false;
}

void CSpawner::DieUpdate()
{
	if (m_bOnceAnimFinish)
		m_bIsDead = true;
}