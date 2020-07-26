#include "stdafx.h"
#include "NormalHitBox.h"


CNormalHitBox::CNormalHitBox():
	m_fSpeed(0.f)
{
}


CNormalHitBox::~CNormalHitBox()
{
	Release();
}

int CNormalHitBox::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();

	if (m_fTimeCount >= m_fLifeTime)
		return DEAD_OBJ;

	if (0.f != m_fSpeed)
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();

#ifdef _DEBUG
	CObjectMgr::GetInstance()->AddLayer(RENDER_UI, this);
#endif // _DEBUG


	return NO_EVENT;
}

void CNormalHitBox::LateUpdate()
{

}

void CNormalHitBox::Render()
{
#ifdef _DEBUG
	CMathMgr::DrawLineBox(m_pDeviceMgr, m_tInfo.vPos - CScrollMgr::GetScrollPos(), m_vSize, 2.f, D3DCOLOR_ARGB(255, 255, 0, 0));
#endif // _DEBUG
}

HRESULT CNormalHitBox::Initialize()
{
	m_tInfo.vDir = {1.f, 0.f, 0.f};
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 0.f };

	m_fSpeed = 0.f;
	m_fTimeCount = 0.f;

	m_eHitBoxType = CHitBox::HIT_BOX_RECT;

	return S_OK;
}

void CNormalHitBox::Release()
{
}

CNormalHitBox * CNormalHitBox::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 vSize, CGameObject * pParent, float fLifeTime, int iAttack)
{
	CNormalHitBox* pInst = new CNormalHitBox;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_vSize = vSize;
	pInst->m_pParent = pParent;
	pInst->m_fLifeTime = fLifeTime;
	pInst->m_iAttack = iAttack;

	return pInst;
}

CNormalHitBox * CNormalHitBox::Create(const INFO & tInfo, const D3DXVECTOR3 vSize, CGameObject * pParent, float fLifeTime, float iAttack, float fSpeed /*= 0.f*/)
{
	CNormalHitBox* pInst = new CNormalHitBox;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo = tInfo;
	pInst->m_vSize = vSize;
	pInst->m_pParent = pParent;
	pInst->m_fLifeTime = fLifeTime;
	pInst->m_fSpeed = fSpeed;
	pInst->m_iAttack = iAttack;


	return pInst;
}
