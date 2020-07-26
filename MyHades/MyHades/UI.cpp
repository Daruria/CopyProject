#include "stdafx.h"
#include "UI.h"


CUI::CUI():
	m_pObjectMgr(CObjectMgr::GetInstance())
{
}


CUI::~CUI()
{
}

int CUI::Update()
{
	return 0;
}

void CUI::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CUI::Render()
{
}

HRESULT CUI::Initialize()
{
	return S_OK;
}

void CUI::Release()
{
}

float CUI::GetLayerStd()
{
	return 0.0f;
}

void CUI::Die()
{
}
