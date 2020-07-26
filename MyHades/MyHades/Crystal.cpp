#include "stdafx.h"
#include "Crystal.h"
#include "Player.h"
#include "PreBeam.h"
#include "Beam.h"

CCrystal::CCrystal():
	m_ePreState(CCrystal::STATE::END),
	m_eCurState(CCrystal::STATE::END),
	m_fSpeed(0.f),
	m_fAngle(0.f),
	m_bIsCharging(false),
	m_bIsFiring(false),
	m_fCharingTime(0.f),
	m_fFireTime(0.f),
	m_fFireRotateSpeed(0.f),
	m_pBeam(nullptr)
{
}


CCrystal::~CCrystal()
{
	Release();
}

int CCrystal::Update()
{
	if (m_bIsDead)
	{
		return DEAD_OBJ;
	}

	if (CCrystal::DIE != m_eCurState &&	m_tStatus.iHp <= 0)
	{
		m_eCurState = CCrystal::DIE;
		m_bKnockBack = false;
		CSoundMgr::GetInstance()->PlaySound(L"CrystalPieceBreak1.wav", CSoundMgr::EFFECT);
		if (m_pBeam)
		{
			m_pBeam->Die();
			m_pBeam = nullptr;
		}
	}
	
	if (m_bKnockBack)
	{
		KnockBackUpdate();
		ChangeState();
		Animation();
	}
	else
	{
		ChangeState();
		StateUpdate();
		Animation();
	}

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CCrystal::LateUpdate()
{
	CMonster::LateUpdate();
}

void CCrystal::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_vecStateKey[m_eCurState], static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _DEBUG
	m_pDeviceMgr->GetSprite()->End();

	D3DXVECTOR3 vPos = m_tInfo.vPos + m_tHitBox.vOffset - CScrollMgr::GetScrollPos();

	D3DXVECTOR2 vPoint[5]
	{
		D3DXVECTOR2(vPos.x - m_tHitBox.vSize.x * 0.5f,vPos.y - m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x + m_tHitBox.vSize.x * 0.5f,vPos.y - m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x + m_tHitBox.vSize.x * 0.5f,vPos.y + m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x - m_tHitBox.vSize.x * 0.5f,vPos.y + m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x - m_tHitBox.vSize.x * 0.5f,vPos.y - m_tHitBox.vSize.y * 0.5f)
	};

	m_pDeviceMgr->GetLine()->SetWidth(2.f); // ¼±ÀÇ ±½±â
	m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
#endif // _DEBUG
}

HRESULT CCrystal::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_wstrObjectKey = L"Crystal";
	
	m_vecStateKey.resize(CCrystal::STATE::END);
	m_vecStateKey[CCrystal::IDLE] = m_vecStateKey[CCrystal::MOVE] = m_vecStateKey[CCrystal::ATTACK_CHARGING] = m_vecStateKey[CCrystal::ATTACK_FIRE] = L"HeavyRanged_Crystal_04A_Move";
	m_vecStateKey[CCrystal::DIE] = L"HeavyRanged_Crystal_04A_Shatter";

	m_eCurState = CCrystal::IDLE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.eType = ANIM_REPEAT;

	m_fSpeed = 200.f;

	m_tHitBox.vOffset = { 0.f, 0.f, 0.f };
	m_tHitBox.vSize = { 100.f, 100.f, 0.f };

	m_vFootPos = {0.f, 10.f, 0.f};

	m_tStatus.iHp = 50;
	m_tStatus.iMaxHp = 50;
	m_tStatus.iAttack = 3;

	m_fFireRotateSpeed = 0.3f;

	return S_OK;
}

void CCrystal::Release()
{
	if (m_pBeam)
	{
		m_pBeam->Die();
		m_pBeam = nullptr;
	}
}

CCrystal * CCrystal::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale)
{
	CCrystal* pInst = new CCrystal;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;

	return pInst;
}

void CCrystal::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	CMonster::Hit(vPos, iAttack);

	KnockBack(vPos);

	CSoundMgr::GetInstance()->PlaySound(L"CrystalHit1.wav", CSoundMgr::EFFECT);
}

void CCrystal::KnockBack(const D3DXVECTOR3 & vHitterPos)
{
	CMoveObject::KnockBack(vHitterPos);

	switch (m_eCurState)
	{
	case CCrystal::ATTACK_CHARGING:
		m_bIsCharging = false;
		break;
	case CCrystal::ATTACK_FIRE:
		m_bIsFiring = false;
		break;
	}

	m_eCurState = CCrystal::IDLE;

	if (m_pBeam)
	{
		m_pBeam->Die();
		m_pBeam = nullptr;
	}

}

void CCrystal::StateUpdate()
{
	switch (m_eCurState)
	{
	case CCrystal::IDLE:
		IdleUpdate();
		break;
	case CCrystal::MOVE:
		MoveUpdate();
		break;
	case CCrystal::ATTACK_CHARGING:
	case CCrystal::ATTACK_FIRE:
		AttackUpdate();
		break;
	case CCrystal::DIE:
		DieUpdate();
		break;
	}
}

void CCrystal::IdleUpdate()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	D3DXVECTOR3 vDist = pPlayer->GetInfo().vPos - m_tInfo.vPos;
	float fDist = D3DXVec3Length(&vDist);
	
	D3DXVec3Normalize(&vDist, &vDist);

	m_tInfo.vLook = vDist;
	if (fDist < 400.f)
		m_eCurState = CCrystal::ATTACK_CHARGING;
	else if (fDist < 800.f)
		m_eCurState = CCrystal::MOVE;
}

void CCrystal::MoveUpdate()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	m_tInfo.vDir = pPlayer->GetInfo().vPos - m_tInfo.vPos;
	float fDist = D3DXVec3Length(&m_tInfo.vDir);

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();

	m_tInfo.vLook = m_tInfo.vDir;

	if (fDist < 400.f)
		m_eCurState = CCrystal::ATTACK_CHARGING;
	else if (fDist < 800.f)
		m_eCurState = CCrystal::MOVE;
	else
	{
		m_eCurState = CCrystal::IDLE;
	}
}

void CCrystal::AttackUpdate()
{

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	D3DXVECTOR3 vDest = pPlayer->GetInfo().vPos - m_tInfo.vPos;

	D3DXVec3Normalize(&vDest, &vDest);
	float fAngle = CMathMgr::CalculateAngleMPIToPI(vDest, m_tInfo.vLook);

	float fDegree = D3DXToDegree(fAngle);
	
	D3DXMATRIX matRotateZ;
	if (fDegree > m_fFireRotateSpeed)
		D3DXMatrixRotationZ(&matRotateZ, -D3DXToRadian(m_fFireRotateSpeed));
	else if (-m_fFireRotateSpeed <= fDegree && fDegree <= m_fFireRotateSpeed)
		D3DXMatrixRotationZ(&matRotateZ, -fAngle);
	else if (fDegree < -m_fFireRotateSpeed)
		D3DXMatrixRotationZ(&matRotateZ, -D3DXToRadian(-m_fFireRotateSpeed));

	m_tInfo.vLook = static_cast<D3DXVECTOR3>((D3DXMATRIX)m_tInfo.vLook * matRotateZ);
	
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	switch (m_eCurState)
	{
	case CCrystal::ATTACK_CHARGING:
	{
		if (!m_bIsCharging)
		{
			m_bIsCharging = true;
			m_fCharingTime = 0.f;
			CPreBeam* pPreBeam = CPreBeam::Create(this);
			m_pObjectMgr->AddObject(OBJECT_MONSTER, pPreBeam);
			m_pBeam = pPreBeam;
		}
		else
		{
			m_fCharingTime += m_pTimeMgr->GetDelta();
			if (m_fCharingTime > 1.f)
			{
				m_eCurState = CCrystal::ATTACK_FIRE;
				m_bIsCharging = false;
				if (m_pBeam)
				{
					m_pBeam->Die();
					m_pBeam = nullptr;
				}
			}
		}
	}
		break;
	case CCrystal::ATTACK_FIRE:
		if (!m_bIsFiring)
		{
			m_bIsFiring = true;
			m_fFireTime = 0.f;
			CBeam* pBeam = CBeam::Create(this);
			m_pObjectMgr->AddObject(OBJECT_MONSTER, pBeam);
			m_pBeam = pBeam;
		}
		else
		{
			m_fFireTime += m_pTimeMgr->GetDelta();
			if (m_fFireTime > 3.f)
			{
				m_eCurState = CCrystal::IDLE;
				m_bIsFiring = false;
				if (m_pBeam)
				{
					m_pBeam->Die();
					m_pBeam = nullptr;
				}
			}
		}

		break;
	}
}

void CCrystal::DieUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_bIsDead = true;
	}
}

void CCrystal::ChangeState()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CCrystal::IDLE:
		IdleState();
		break;
	case CCrystal::MOVE:
		MoveState();
		break;
	case CCrystal::ATTACK_CHARGING:
		AttackState();
		break;
	case CCrystal::ATTACK_FIRE:
		AttackState();
		break;
	case CCrystal::DIE:
		DieState();
		break;
	}

	m_ePreState = m_eCurState;
}

void CCrystal::IdleState()
{
	if (CCrystal::STATE::DIE != m_ePreState)
		return;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.eType = ANIM_REPEAT;
}

void CCrystal::MoveState()
{
	if (CCrystal::STATE::DIE != m_ePreState)
		return;
	
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.eType = ANIM_REPEAT;
}

void CCrystal::AttackState()
{
	if (CCrystal::STATE::DIE != m_ePreState)
		return;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.eType = ANIM_REPEAT;
}

void CCrystal::DieState()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 17.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.eType = ANIM_ONCE;
}
