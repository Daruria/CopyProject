#include "stdafx.h"
#include "MoveEffect.h"
#include "EffectImp.h"

CMoveEffect::CMoveEffect(CEffectImp* pImp):
	CBridgeEffect(pImp),
	m_fSpeed(0.f)
{
}


CMoveEffect::~CMoveEffect()
{
	Release();
}

int CMoveEffect::Update()
{
	int iRet = m_pImp->Update();

	if (NO_EVENT == iRet)
	{
		m_pObjectMgr->AddLayer(m_eRenderType, this);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();
	}

	return iRet;
}

void CMoveEffect::LateUpdate()
{
	m_pImp->LateUpdate();

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMoveEffect::Render()
{
	m_pImp->Render(&m_tInfo.matWorld);
}

HRESULT CMoveEffect::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 0.f };

	return S_OK;
}

void CMoveEffect::Release()
{
}

CMoveEffect * CMoveEffect::Create(CEffectImp * pImp, const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vDir, float fSpeed, RENDER_TYPE eRenderType)
{
	CMoveEffect* pInst = new CMoveEffect(pImp);

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos		= vPos;
	pInst->m_tInfo.vDir		= vDir;
	pInst->m_fSpeed			= fSpeed;
	pInst->m_eRenderType	= eRenderType;

	return pInst;
}
