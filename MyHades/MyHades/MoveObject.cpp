#include "stdafx.h"
#include "MoveObject.h"
#include "Effect.h"

CMoveObject::CMoveObject():
	m_bOnceAnimFinish(false),
	m_bCanColl(true),
	m_bKnockBack(false),
	m_fKnockBackPower(0.f),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pSubjectMgr(CSubjectMgr::GetInstance())
{
	ZeroMemory(&m_tHitBox, sizeof(HIT_BOX));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_vFootPos, sizeof(D3DXVECTOR3));
}


CMoveObject::~CMoveObject()
{
	Release();
}

int CMoveObject::Update()
{
	Animation();
	return 0;
}

void CMoveObject::LateUpdate()
{
}

void CMoveObject::Render()
{
}

HRESULT CMoveObject::Initialize()
{
	m_bOnceAnimFinish = false;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tHitBox.vOffset = {0.f, 0.f, 0.f};
	m_tHitBox.vSize = { 100.f, 100.f, 0.f };
	m_vFootPos = {0.f, 0.f, 0.f};

	m_tInfo.vDir = {1.f, 0.f, 0.f};
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 1.f };
	
	m_fKnockBackPower = 50.f;

	return S_OK;
}

void CMoveObject::Release()
{
}

void CMoveObject::Animation()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame += static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		m_tFrame.fFrameTime = 0.f;
		switch (m_tFrame.eType)
		{
		case ANIM_ONCE:
			if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
			{
				m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
				m_bOnceAnimFinish = true;
			}
			break;
		case ANIM_REPEAT:
			if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
				m_tFrame.fCurFrame = 0;
			break;
		}
	}
	
}

void CMoveObject::KnockBack(const D3DXVECTOR3& vHitterPos)
{
	m_vKnockBackDir = m_tInfo.vPos - vHitterPos;
	D3DXVec3Normalize(&m_vKnockBackDir, &m_vKnockBackDir);

	m_bKnockBack = true;
	m_fKnockBackPower = 200.f;

}

void CMoveObject::KnockBackUpdate()
{
	m_fKnockBackPower -= 200.f * m_pTimeMgr->GetDelta();

	m_tInfo.vPos += m_vKnockBackDir * m_pTimeMgr->GetDelta() * m_fKnockBackPower;
	if (m_fKnockBackPower < 1.f)
		m_bKnockBack = false;
}

void CMoveObject::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	INFO tInfo;
	tInfo.vPos =  (vPos + m_tInfo.vPos) * 0.5f;
	tInfo.vDir = { 1.f, 0.f, 0.f };
	tInfo.vLook = { 1.f, 0.f, 0.f };
	tInfo.vScale = { 1.f, 1.f, 0.f };

	CEffect* pEffect = CEffect::Create(tInfo, L"Fx", L"HitSparkA", 11, 1 / 15.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
}

const RECT CMoveObject::GetRect() const
{
	return RECT{
		static_cast<LONG>(m_tInfo.vPos.x + m_tHitBox.vOffset.x - m_tHitBox.vSize.x * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y + m_tHitBox.vOffset.y - m_tHitBox.vSize.y * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.x + m_tHitBox.vOffset.x + m_tHitBox.vSize.x * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y + m_tHitBox.vOffset.y + m_tHitBox.vSize.y * 0.5f)
	};
}
