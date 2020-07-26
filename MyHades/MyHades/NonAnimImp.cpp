#include "stdafx.h"
#include "NonAnimImp.h"


CNonAnimImp::CNonAnimImp():
	m_fLifeTime(0.f),
	m_fTimeCount(0.f),
	m_d3dColor(D3DCOLOR_ARGB(255, 255, 255, 255))
{
}


CNonAnimImp::~CNonAnimImp()
{
	Release();
}

int CNonAnimImp::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();

	if (m_fTimeCount >= m_fLifeTime)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CNonAnimImp::LateUpdate()
{
}

void CNonAnimImp::Render(const D3DXMATRIX * pMatrix)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrStateKey);
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width* 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(pMatrix);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);

}

HRESULT CNonAnimImp::Initialize()
{


	return S_OK;
}

void CNonAnimImp::Release()
{
}

CNonAnimImp * CNonAnimImp::Create(const wstring & wstrStateKey, float fLifeTime, D3DCOLOR d3dColor)
{
	CNonAnimImp* pInst = new CNonAnimImp;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_fLifeTime = fLifeTime;
	pInst->m_d3dColor = d3dColor;

	return pInst;
}
