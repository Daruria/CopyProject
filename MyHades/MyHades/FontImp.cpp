#include "stdafx.h"
#include "FontImp.h"


CFontImp::CFontImp():
	m_wstrText(L""),
	m_fLifeTime(0.f),
	m_fTimeCount(0.f),
	m_pFontMgr(CFontMgr::GetInstance())
{
}


CFontImp::~CFontImp()
{
	Release();
}

int CFontImp::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();
	if (m_fTimeCount >= m_fLifeTime)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CFontImp::LateUpdate()
{

}

void CFontImp::Render(const D3DXMATRIX * pMatrix)
{
	m_pFontMgr->Render(pMatrix, m_vSize, m_wstrText, m_d3dColor);
}

HRESULT CFontImp::Initialize()
{


	return S_OK;
}

void CFontImp::Release()
{
}

CFontImp * CFontImp::Create(const wstring & wstrText, const D3DXVECTOR3& vSize, float fLifeTime, D3DCOLOR d3dColor)
{
	CFontImp* pInst = new CFontImp;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrText = wstrText;
	pInst->m_fLifeTime = fLifeTime;
	pInst->m_vSize = vSize;
	pInst->m_d3dColor = d3dColor;

	return pInst;
}
