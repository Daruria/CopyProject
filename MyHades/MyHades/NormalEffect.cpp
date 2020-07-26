#include "stdafx.h"
#include "NormalEffect.h"
#include "EffectImp.h"

CNormalEffect::CNormalEffect(CEffectImp* pImp):
	CBridgeEffect(pImp),
	m_fAngle(0.f)
{
}


CNormalEffect::~CNormalEffect()
{
	Release();
}

int CNormalEffect::Update()
{
	int iRet = m_pImp->Update();

	if (NO_EVENT == iRet)
		m_pObjectMgr->AddLayer(m_eRenderType, this);

	return iRet;
}

void CNormalEffect::LateUpdate()
{
	m_pImp->LateUpdate();

	D3DXMATRIX matScale, matRotateZ, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixRotationZ(&matRotateZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);

	m_tInfo.matWorld = matScale * matRotateZ * matTrans;
}

void CNormalEffect::Render()
{
	m_pImp->Render(&m_tInfo.matWorld);
}

HRESULT CNormalEffect::Initialize()
{
	m_tInfo.vScale = { 1.f, 1.f, 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	return S_OK;
}

void CNormalEffect::Release()
{
}

void CNormalEffect::Die()
{
}

CNormalEffect * CNormalEffect::Create(CEffectImp * pImp, const D3DXVECTOR3 & vPos, RENDER_TYPE eRenderType)
{
	CNormalEffect* pInst = new CNormalEffect(pImp);

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}
	
	pInst->m_tInfo.vPos		= vPos;
	pInst->m_eRenderType	= eRenderType;

	return pInst;
}

CNormalEffect * CNormalEffect::Create(CEffectImp * pImp, const D3DXVECTOR3 & vPos, const D3DXVECTOR3& vScale, RENDER_TYPE eRenderType)
{
	CNormalEffect* pInst = new CNormalEffect(pImp);

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_eRenderType = eRenderType;

	return pInst;
}

CNormalEffect * CNormalEffect::Create(CEffectImp * pImp, const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, float fRadian, RENDER_TYPE eRenderType)
{
	CNormalEffect* pInst = new CNormalEffect(pImp);

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_fAngle = fRadian;
	pInst->m_eRenderType = eRenderType;

	return pInst;
}
