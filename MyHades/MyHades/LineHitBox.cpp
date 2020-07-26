#include "stdafx.h"
#include "LineHitBox.h"
#include "Effect.h"
#include "Beam.h"

CLineHitBox::CLineHitBox():
	m_fLength(0.f),
	m_fAngle(0.f),
	m_fLengthOrigin(0.f),
	m_bPreFrameColl(false)
{
}


CLineHitBox::~CLineHitBox()
{
	Release();
}

int CLineHitBox::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();

	if (m_fTimeCount >= m_fLifeTime)
	{
		//CBeam* pBeam = dynamic_cast<CBeam*>(m_pParent);
		//if (pBeam)
		//	pBeam->SetLineHitBox(nullptr);
		
		return DEAD_OBJ;
	}

	NULL_CHECK_RETURN(m_pParent, NO_EVENT);
	m_fAngle = -CMathMgr::CalculateAngleMPIToPI(m_pParent->GetInfo().vLook, D3DXVECTOR3(1.f, 0.f, 0.f));
	
	if (!m_bPreFrameColl)
		m_fLength = m_fLengthOrigin;

#ifdef _DEBUG
	CObjectMgr::GetInstance()->AddLayer(RENDER_UI, this);

#endif // _DEBUG
	return NO_EVENT;
}

void CLineHitBox::LateUpdate()
{
	m_bPreFrameColl = false;
}

void CLineHitBox::Render()
{
#ifdef _DEBUG
	CMathMgr::DrawLine(m_pDeviceMgr, m_tInfo.vPos - CScrollMgr::GetScrollPos(), m_fLength, m_fAngle, 2.f, D3DCOLOR_ARGB(255, 255, 0, 0));

#endif // _DEBUG
}

HRESULT CLineHitBox::Initialize()
{
	HRESULT hr = CHitBox::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_fAngle = 0.f;
	m_fLength = 0.f;

	m_eHitBoxType = CHitBox::HIT_BOX_LINE;

	return S_OK;
}

void CLineHitBox::Release()
{
}

void CLineHitBox::Hit()
{
}

void CLineHitBox::Hit(const D3DXVECTOR3 & vPos)
{
	CEffect* pEffect = CEffect::Create(vPos, D3DXVECTOR3(0.5f, 0.5f, 0.f), D3DXToDegree(m_fAngle),  L"Fx", L"ProjectileFlare", 15, 1/60.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255));
	NULL_CHECK(pEffect);

	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

	m_bPreFrameColl = true;

	D3DXVECTOR3 vDist = vPos - m_tInfo.vPos;
	m_fLength = D3DXVec3Length(&vDist);

	CBeam* pBeam = dynamic_cast<CBeam*>(m_pParent);
	NULL_CHECK(pBeam);
	pBeam->Collision(m_fLength);
}

CLineHitBox * CLineHitBox::Create(const D3DXVECTOR3 & vPos, float fLength, float fAngle, int iAttack, float fLifeTime, CGameObject* pParent)
{
	CLineHitBox* pInst = new CLineHitBox;
	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_fLength = fLength;
	pInst->m_fLengthOrigin = fLength;
	pInst->m_fAngle = fAngle;
	pInst->m_fLifeTime = fLifeTime;
	pInst->m_pParent = pParent;
	pInst->m_iAttack = iAttack;

	return pInst;
}

LINE CLineHitBox::GetLine()
{
	LINE tLine;
	tLine.P1 = { m_tInfo.vPos.x, m_tInfo.vPos.y };
	
	D3DXVECTOR3 vEndPos{ 0.f, 0.f, 0.f };
	vEndPos.x = m_fLength;
	D3DXMATRIX matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, m_fAngle);

	vEndPos = static_cast<D3DXVECTOR3>(static_cast<D3DXMATRIX>(vEndPos) * matRotateZ);
	vEndPos += m_tInfo.vPos;
	tLine.P2 = { vEndPos.x, vEndPos.y };

	return tLine;
}
