#include "stdafx.h"
#include "AnimReverse.h"


CAnimReverse::CAnimReverse() :
	m_wstrObjectKey(L"")
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CAnimReverse::~CAnimReverse()
{
	Release();
}

int CAnimReverse::Update()
{
	m_tFrame.fFrameTime -= m_pTimeMgr->GetDelta();
	//if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame = static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		//m_tFrame.fFrameTime = 0.f;

		if (m_tFrame.fCurFrame < 0.f)
		{
			m_tFrame.fCurFrame = 0.f;

			return DEAD_OBJ;
		}//if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	}//if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)

	return NO_EVENT;
}

void CAnimReverse::LateUpdate()
{
}

void CAnimReverse::Render(const D3DXMATRIX * pMatrix)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width*0.5f, pTexInfo->tImgInfo.Height*0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(pMatrix);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
}

HRESULT CAnimReverse::Initialize()
{
	return S_OK;
}

void CAnimReverse::Release()
{
}

CAnimReverse * CAnimReverse::Create(const wstring & wstrObjectKey, const wstring & wstrStateKey, const FRAME & tFrame, D3DCOLOR d3dColor)
{
	CAnimReverse* pInst = new CAnimReverse;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrObjectKey = wstrObjectKey;
	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_tFrame = tFrame;
	pInst->m_tFrame.fFrameTime = tFrame.fCurFrame * tFrame.fFrameSpeed;
	pInst->m_d3dColor = d3dColor;

	return pInst;
}
