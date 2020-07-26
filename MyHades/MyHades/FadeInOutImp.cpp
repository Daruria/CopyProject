#include "stdafx.h"
#include "FadeInOutImp.h"


CFadeInOutImp::CFadeInOutImp() :
	m_fTimeCount(0.f),
	m_fFadeInTime(0.f),
	m_fShowTime(0.f),
	m_fFadeOutTime(0.f),
	m_eCurState(CFadeInOutImp::END),
	m_fAlpha(0.f),
	m_d3dColor(D3DCOLOR_ARGB(0, 255, 255, 255))
{

}

CFadeInOutImp::~CFadeInOutImp()
{
	Release();
}

int CFadeInOutImp::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();

	D3DCOLOR d3dColor = 0;
	d3dColor = m_d3dColor & 0x00ffffff;
	UINT uiAlpha = 0;

	switch (m_eCurState)
	{
	case CFadeInOutImp::FADE_IN:
		m_fAlpha += (255 / m_fFadeInTime) * m_pTimeMgr->GetDelta();
		uiAlpha = static_cast<UINT>(m_fAlpha) << 24;
		m_d3dColor = uiAlpha | d3dColor;
		
		if (m_fTimeCount >= m_fFadeInTime)
		{
			m_eCurState = CFadeInOutImp::SHOW;
			m_fTimeCount = 0.f;
			m_fAlpha = 255.f;
			uiAlpha = 255 << 24;
			m_d3dColor = uiAlpha | d3dColor;
		}
		break;
	case CFadeInOutImp::SHOW:
		if (m_fTimeCount >= m_fShowTime)
		{
			m_eCurState = CFadeInOutImp::FADE_OUT;
			m_fTimeCount = 0.f;
		}
		break;
	case CFadeInOutImp::FADE_OUT:
		m_fAlpha -= (255 / m_fFadeOutTime) * m_pTimeMgr->GetDelta();
		uiAlpha = static_cast<UINT>(m_fAlpha) << 24;
		m_d3dColor = uiAlpha | d3dColor;

		if (m_fTimeCount >= m_fFadeOutTime)
		{
			return DEAD_OBJ;
		}
		break;
	}

	return NO_EVENT;
}

void CFadeInOutImp::LateUpdate()
{
}

void CFadeInOutImp::Render(const D3DXMATRIX * pMatrix)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrStateKey);
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(pMatrix);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
}

HRESULT CFadeInOutImp::Initialize()
{
	m_eCurState = CFadeInOutImp::FADE_IN;
	float m_fAlpha = 0.f;

	return S_OK;
}

void CFadeInOutImp::Release()
{
}

CFadeInOutImp * CFadeInOutImp::Create(const wstring & wstrStateKey, float fFadeInTime, float fShowTime, float fFadeOutTime, D3DCOLOR d3dColor)
{
	CFadeInOutImp* pInst = new CFadeInOutImp;
	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_fFadeInTime = fFadeInTime;
	pInst->m_fShowTime = fShowTime;
	pInst->m_fFadeOutTime = fFadeOutTime;
	pInst->m_d3dColor = d3dColor;

	return  pInst;
}