#include "stdafx.h"
#include "ScreenEffect.h"
#include "EffectImp.h"

CScreenEffect::CScreenEffect(CEffectImp* pImp):
	CBridgeEffect(pImp),
	m_fSort(0.f)
{
}


CScreenEffect::~CScreenEffect()
{
	Release();
}

int CScreenEffect::Update()
{
	int iRet = m_pImp->Update();

	if (NO_EVENT == iRet)
		m_pObjectMgr->AddLayer(m_eRenderType, this);

	return iRet;
}

void CScreenEffect::LateUpdate()
{
	m_pImp->LateUpdate();

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CScreenEffect::Render()
{
	m_pImp->Render(&m_tInfo.matWorld);
}

HRESULT CScreenEffect::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 1.f };

	m_fSort = 0.f;

	return S_OK;
}

void CScreenEffect::Release()
{
	
}

CScreenEffect * CScreenEffect::Create(CEffectImp * pImp, const D3DXVECTOR3 & vPos, RENDER_TYPE eRenderType, float fSort)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CScreenEffect* pInst = new CScreenEffect(pImp);

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_pImp = pImp;
	pInst->m_tInfo.vPos = vPos;
	pInst->m_eRenderType = eRenderType;
	pInst->m_fSort = fSort;

	return pInst;
}

CScreenEffect * CScreenEffect::Create(CEffectImp * pImp, const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, RENDER_TYPE eRenderType, float fSort)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CScreenEffect* pInst = new CScreenEffect(pImp);

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_pImp = pImp;
	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_eRenderType = eRenderType;
	pInst->m_fSort = fSort;

	return pInst;
}
