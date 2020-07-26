#include "stdafx.h"
#include "ConversationImp.h"


CConversationImp::CConversationImp():
	m_wstrNameKey(L""),
	m_wstrPortraitKey(L""),
	m_wstrSpeaker(L""),
	m_wstrTalk(L""),
	m_eCurState(CConversationImp::END)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CConversationImp::~CConversationImp()
{
	Release();
}

int CConversationImp::Update()
{
	return StateUpdate();
}

void CConversationImp::LateUpdate()
{
}

void CConversationImp::Render(const D3DXMATRIX * pMatrix)
{
	const TEX_INFO* pTexInfo = nullptr;
	D3DXVECTOR3 vCenter;
	D3DXMATRIX matScale, matTrans;

	if (CConversationImp::STOP == m_eCurState)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrPortraitKey);
		NULL_CHECK(pTexInfo);
		D3DXMatrixScaling(&matScale, 0.75f, 0.75f, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f - 350.f, WINCY * 0.5f + 100.f, 0.f);
		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	wstring wstrDialogueBox = m_eCurState == CConversationImp::OUTRO ? L"DialogueBox-Out" : L"DialogueBox-In";

	pTexInfo = m_pTextureMgr->GetTexInfo(L"DialogueBox", wstrDialogueBox, static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);
	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f + 200.f, WINCY * 0.5f + 150.f, 0.f);
	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CConversationImp::STOP == m_eCurState)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrNameKey);
		NULL_CHECK(pTexInfo);
		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f + 50.f, WINCY * 0.5f + 70.f, 0.f);
		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrSpeaker, m_wstrTalk, m_iTalkNum);
		NULL_CHECK(pTexInfo);
		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f + 200.f, WINCY * 0.5f + 175.f, 0.f);
		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CConversationImp::Initialize()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 17.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;

	m_eCurState = CConversationImp::INTRO;

	CSoundMgr::GetInstance()->PlaySound(L"DialoguePanelIn.wav", CSoundMgr::EFFECT);

	return S_OK;
}

void CConversationImp::Release()
{
}

CConversationImp * CConversationImp::Create(const wstring & wstrNameKey, const wstring& wstrPortraitKey, const wstring & wstrSpeaker, const wstring & wstrTalk, int iTalkNum)
{
	CConversationImp* pInst = new CConversationImp;
	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrNameKey = wstrNameKey;
	pInst->m_wstrPortraitKey = wstrPortraitKey;
	pInst->m_wstrSpeaker = wstrSpeaker;
	pInst->m_wstrTalk = wstrTalk;
	pInst->m_iTalkNum = iTalkNum;

	return pInst;
}

int CConversationImp::StateUpdate()
{
	switch (m_eCurState)
	{
	case CConversationImp::INTRO:
		return IntroUpdate();
	case CConversationImp::STOP:
		return StopUpdate();
	case CConversationImp::OUTRO:
		return OutroUpdate();
	}

	return NO_EVENT;
}

int CConversationImp::IntroUpdate()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame += static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		m_tFrame.fFrameTime = 0.f;

		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
			m_eCurState = CConversationImp::STOP;
		}
	}

	return NO_EVENT;
}

int CConversationImp::StopUpdate()
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON))
	{
		m_eCurState = CConversationImp::OUTRO;
		m_tFrame.fCurFrame = 0.f;
		m_tFrame.fMaxFrame = 7.f;
		m_tFrame.fFrameSpeed = 1 / 30.f;
		m_tFrame.fFrameTime = 0.f;
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::EFFECT);
		CSoundMgr::GetInstance()->PlaySound(L"DialoguePanelOut.wav", CSoundMgr::EFFECT);

	}

	return NO_EVENT;
}

int CConversationImp::OutroUpdate()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame += static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		m_tFrame.fFrameTime = 0.f;

		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
			return DEAD_OBJ;
		}
	}
	return NO_EVENT;
}