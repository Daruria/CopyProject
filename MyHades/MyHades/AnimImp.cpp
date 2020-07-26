#include "stdafx.h"
#include "AnimImp.h"


CAnimImp::CAnimImp():
	m_wstrObjectKey(L"")
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CAnimImp::~CAnimImp()
{
	Release();
}

int CAnimImp::Update()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	//if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame = static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		//m_tFrame.fFrameTime = 0.f;

		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			switch (m_tFrame.eType)
			{
			case ANIM_ONCE:
				m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
				return DEAD_OBJ;
				break;
			case ANIM_REPEAT:
				m_tFrame.fCurFrame = 0;
				m_tFrame.fFrameTime = 0.f;
				break;
			}//switch (m_tFrame.eType)
		}//if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	}//if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)

	return NO_EVENT;
}

void CAnimImp::LateUpdate()
{
}

void CAnimImp::Render(const D3DXMATRIX * pMatrix)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width*0.5f, pTexInfo->tImgInfo.Height*0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(pMatrix);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);

}

HRESULT CAnimImp::Initialize()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameTime = 0.f;
	
	return S_OK;
}

void CAnimImp::Release()
{
}

CAnimImp * CAnimImp::Create(const wstring& wstrObjectKey, const wstring & wstrStateKey, const FRAME & tFrame, D3DCOLOR d3dColor)
{
	CAnimImp* pInst = new CAnimImp;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrObjectKey = wstrObjectKey;
	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_tFrame = tFrame;
	pInst->m_d3dColor = d3dColor;

	return pInst;

}

CAnimImp * CAnimImp::Create(const wstring & wstrObjectKey, const wstring & wstrStateKey, float fMaxFrame, float fFrameSpeed, D3DCOLOR d3dColor)
{
	CAnimImp* pInst = new CAnimImp;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrObjectKey = wstrObjectKey;
	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_tFrame.fMaxFrame = fMaxFrame;
	pInst->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInst->m_d3dColor = d3dColor;

	return pInst;
}
