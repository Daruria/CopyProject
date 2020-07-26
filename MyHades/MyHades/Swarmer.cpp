#include "stdafx.h"
#include "Swarmer.h"
#include "NormalHitBox.h"
#include "Player.h"
#include "Effect.h"

CSwarmer::CSwarmer():
	m_wstrStateKey(L""),
	m_ePreState(CSwarmer::STATE::END),
	m_eCurState(CSwarmer::STATE::END),
	m_bAttack(false),
	m_fSpeed(0.f),
	m_eAtkPhase(CSwarmer::ATTACK_STATE::ATK_END),
	m_fShake(0.f),
	m_fDashSpeed(0.f),
	m_fEyeBlinkTime(0.f),
	m_fEyeBlinkTimeCount(0.f),
	m_fAttackCoolCount(0.f),
	m_fAttackCoolTime(0.f),
	m_iDirCount(0)
{
}

CSwarmer::~CSwarmer()
{
	Release();
}

int CSwarmer::Update()
{
	if (m_tStatus.iHp <= 0)
	{
		CEffect* pEffect = CEffect::Create(m_tInfo, m_wstrObjectKey, L"DeathVFX", 30, 1 / 30.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
		m_bIsDead = true;
		CSoundMgr::GetInstance()->PlaySound(L"LightSpawner_EmoteDying_1.wav", CSoundMgr::EFFECT);
	}

	if (m_bIsDead)
		return DEAD_OBJ;

	ChangeState();

	StateUpdate();

	Animation();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CSwarmer::LateUpdate()
{
	CMonster::LateUpdate();
}

void CSwarmer::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, 
		static_cast<WORD>(m_tFrame.fCurFrame) + CMathMgr::AngleTo16Dir(D3DXVECTOR3(1.f, 0.f, 0.f), m_tInfo.vDir) * m_iDirCount);
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _DEBUG
	CMathMgr::DrawLineBox(m_pDeviceMgr, m_tInfo.vPos + m_tHitBox.vOffset - CScrollMgr::GetScrollPos(), m_tHitBox.vSize, 2.f, D3DCOLOR_ARGB(255, 255, 0, 255));
#endif //_DEBUG


}

HRESULT CSwarmer::Initialize()
{
	CMonster::Initialize();
	m_eCurState = CSwarmer::IDLE;
	m_fSpeed = 200.f;

	m_tInfo.vScale = { 0.5f, 0.5f, 0.f };
	m_tHitBox.vSize = { 50.f, 50.f, 0.f };

	m_wstrObjectKey = L"WretchSwarmer";
	m_fEyeBlinkTime = 2.5f;
	m_fAttackCoolTime = 3.f;

	m_fShake = 1.f;
	m_fDashSpeed = 1000.f;

	return S_OK;
}

void CSwarmer::Release()
{
}

CSwarmer * CSwarmer::Create(const D3DXVECTOR3 & vPos)
{
	CSwarmer* pInst = new CSwarmer;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		nullptr;
	}

	pInst->m_tInfo.vPos = vPos;

	return pInst;
}

void CSwarmer::ChangeState()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CSwarmer::IDLE:
		IdleState();
		break;
	case CSwarmer::ATTACK:
		AttackState();
		break;
	case CSwarmer::DIE:
		DieState();
		break;
	}
	m_ePreState = m_eCurState;
}

void CSwarmer::IdleState()
{
	m_wstrStateKey = L"Idle";
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 1 / 15.f;
	m_tFrame.fFrameTime = 0.f;
	m_iDirCount = 3;
}

void CSwarmer::AttackState()
{
	m_wstrStateKey = L"Attack";
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;
	m_tFrame.fFrameSpeed = 1 / 15.f;
	m_tFrame.fFrameTime = 0.f;
	m_iDirCount = 3;
}

void CSwarmer::DieState()
{
	m_wstrStateKey = L"DeathVFX";
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_iDirCount = 0;
}

void CSwarmer::StateUpdate()
{
	switch (m_eCurState)
	{
	case CSwarmer::IDLE:
		IdleUpdate();
		break;
	case CSwarmer::ATTACK:
		AttackUpdate();
		break;
	case CSwarmer::DIE:
		DieUpdate();
		break;
	}
}

void CSwarmer::IdleUpdate()
{
	//	юс╫ц 
	m_fEyeBlinkTimeCount += m_pTimeMgr->GetDelta();
	if (m_bOnceAnimFinish)
	{
		if (m_fEyeBlinkTimeCount >= m_fEyeBlinkTime)
		{
			m_fEyeBlinkTimeCount = 0;
			m_bOnceAnimFinish = false;
			m_tFrame.fCurFrame = 0.f;
		}
	}

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);
	
	D3DXVECTOR3 vDir = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	float fLength = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	
	if (fLength < 300.f)
	{
		m_tInfo.vDir = vDir;
		m_eCurState = CSwarmer::ATTACK;
		m_eAtkPhase = CSwarmer::JUMP;
	}
	else if (fLength < 500.f)
	{
		m_tInfo.vDir = vDir;
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();
	}
}

void CSwarmer::AttackUpdate()
{
	switch (m_eAtkPhase)
	{
	case CSwarmer::JUMP:
		if (m_bOnceAnimFinish)
		{
			m_bOnceAnimFinish = false;
			m_eAtkPhase = CSwarmer::SHAKE;
			m_tFrame.fCurFrame = 0.f;
			m_wstrStateKey = L"Idle";
			m_tFrame.fMaxFrame = 1.f;
			m_tFrame.fFrameTime = 0.f;
			m_tFrame.fFrameSpeed = 0.5f;
		}
		break;
	case CSwarmer::SHAKE:
		m_fShake *= -1.f;
		m_tInfo.vPos.x += m_fShake * 5.f;
		if (m_bOnceAnimFinish)
		{
			m_bOnceAnimFinish = false;
			m_eAtkPhase = CSwarmer::DASH;
			m_tFrame.fCurFrame = 2.f;
			m_wstrStateKey = L"Attack";
			m_tFrame.fMaxFrame = 1.f;
			m_tFrame.fFrameTime = 0.f;
			m_tFrame.fFrameSpeed = 0.5f;
			
			CNormalHitBox* pHitBox = CNormalHitBox::Create(m_tInfo, m_tHitBox.vSize, this, 0.5f, m_tStatus.iHp, m_fDashSpeed);
			NULL_CHECK(pHitBox);
			m_pObjectMgr->AddObject(OBJECT_MONSTER_HITBOX, pHitBox);
			CSoundMgr::GetInstance()->PlaySound(L"LightSpawner_EmoteAttacking_1.wav", CSoundMgr::EFFECT);
		}
		break;
	case CSwarmer::DASH:
		m_tInfo.vPos += m_tInfo.vDir * m_fDashSpeed * m_pTimeMgr->GetDelta();

		if (m_bOnceAnimFinish)
		{
			m_bOnceAnimFinish = false;
			m_eCurState = CSwarmer::IDLE;
		}
		break;
	}
}

void CSwarmer::DieUpdate()
{
	if (m_bOnceAnimFinish)
		m_bIsDead = true;
}

void CSwarmer::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	CMonster::Hit(vPos, iAttack);


}

void CSwarmer::Die()
{
	
}