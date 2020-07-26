#include "stdafx.h"
#include "AnimForwardReverseImp.h"


CAnimForwardReverseImp::CAnimForwardReverseImp():
	m_wstrObjectKey(L""),
	m_d3dColor(D3DCOLOR_ARGB(255, 255, 255, 255)),
	m_eCurState(CAnimForwardReverseImp::END),
	m_fStopTime(0.f),
	m_fStopTimeCount(0.f)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CAnimForwardReverseImp::~CAnimForwardReverseImp()
{
	Release();
}

int CAnimForwardReverseImp::Update()
{
	
	return StateUpdate();
}

void CAnimForwardReverseImp::LateUpdate()
{
}

void CAnimForwardReverseImp::Render(const D3DXMATRIX * pMatrix)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width*0.5f, pTexInfo->tImgInfo.Height*0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(pMatrix);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
}

HRESULT CAnimForwardReverseImp::Initialize()
{
	m_eCurState = CAnimForwardReverseImp::FORWARD;

	return S_OK;
}

void CAnimForwardReverseImp::Release()
{
}

CAnimForwardReverseImp * CAnimForwardReverseImp::Create(const wstring & wstrObjectKey, const wstring & wstrStateKey, const FRAME & tFrame, float fStopTime, D3DCOLOR d3dColor)
{
	CAnimForwardReverseImp* pInst = new CAnimForwardReverseImp;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrObjectKey = wstrObjectKey;
	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_tFrame = tFrame;
	pInst->m_d3dColor = d3dColor;
	pInst->m_fStopTime = fStopTime;
}

int CAnimForwardReverseImp::StateUpdate()
{
	switch (m_eCurState)
	{
	case CAnimForwardReverseImp::FORWARD:
		return ForwardUpdate();
	case CAnimForwardReverseImp::STOP:
		return StopUpdate();
	case CAnimForwardReverseImp::REVERSE:
		return ReverseUpdate();
	}

	return NO_EVENT;
}

int CAnimForwardReverseImp::ForwardUpdate()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	//if (m_tFrame.fFrameTime >= m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame = static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		//m_tFrame.fFrameTime = 0.f;
		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
			m_eCurState = CAnimForwardReverseImp::STOP;
			m_tFrame.fFrameTime = m_tFrame.fCurFrame * m_tFrame.fFrameSpeed;
		}
	}

	return NO_EVENT;
}

int CAnimForwardReverseImp::StopUpdate()
{
	m_fStopTimeCount += m_pTimeMgr->GetDelta();

	if (m_fStopTimeCount >= m_fStopTime)
		m_eCurState = CAnimForwardReverseImp::REVERSE;

	return NO_EVENT;
}

int CAnimForwardReverseImp::ReverseUpdate()
{
	m_tFrame.fFrameTime -= m_pTimeMgr->GetDelta();
	//if (m_tFrame.fFrameTime >= m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame = static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		//m_tFrame.fFrameTime = 0.f;
		if (m_tFrame.fCurFrame < 0.f)
		{
			m_tFrame.fCurFrame = 0.f;
			return DEAD_OBJ;
		}
	}

	return NO_EVENT;
}
