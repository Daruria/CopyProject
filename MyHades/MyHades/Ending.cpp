#include "stdafx.h"
#include "Ending.h"
#include "Player.h"


CEnding::CEnding():
	m_eCurState(CEnding::END),
	m_wstrStateKey(L""),
	m_bOnceAnimFinish(false)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CEnding::~CEnding()
{
	Release();
}

int CEnding::Update()
{
	int iRet = StateUpdate();

	return NO_EVENT;
}

void CEnding::LateUpdate()
{
	if (CEnding::PLAYER_DIE == m_eCurState)
		m_pObjectMgr->LateUpdate();
}

void CEnding::Render()
{
	switch (m_eCurState)
	{
	case CEnding::REM_BG_INTRO:
	case CEnding::REM_BG:
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"RemBG", m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));
		NULL_CHECK(pTexInfo);
		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, static_cast<float>(WINCX) / pTexInfo->tImgInfo.Width, static_cast<float>(WINCY) / pTexInfo->tImgInfo.Height, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		
		D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
		break;
	case CEnding::PLAYER_DIE:
		m_pObjectMgr->Render();
		break;
	}
}

HRESULT CEnding::Initialize()
{
	CThreadMgr::GetInstance()->WaitForThread();

	m_eCurState = CEnding::REM_BG_INTRO;

	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.fMaxFrame = 11.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;

	m_wstrStateKey = L"RemBGIntro";

	return S_OK;
}

void CEnding::Release()
{
}

void CEnding::Animation()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();

	m_tFrame.fCurFrame = static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
	//m_tFrame.fFrameTime = 0.f;
	switch (m_tFrame.eType)
	{
	case ANIM_ONCE:
		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
			m_bOnceAnimFinish = true;
		}
		break;
	case ANIM_REPEAT:
		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fFrameTime = 0.f;
		}
		break;
	}

}

int CEnding::StateUpdate()
{
	switch (m_eCurState)
	{
	case CEnding::REM_BG_INTRO:
		Animation();
		if (m_bOnceAnimFinish)
		{
			m_bOnceAnimFinish = false;
			m_tFrame.eType = ANIM_REPEAT;
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fFrameTime = 0.f;
			m_tFrame.fFrameSpeed = 1 / 30.f;
			m_tFrame.fMaxFrame = 60.f;
			m_eCurState = CEnding::REM_BG;
			m_wstrStateKey = L"RemBG";

			CSoundMgr::GetInstance()->PlaySound(L"Storyteller_0186.wav", CSoundMgr::EFFECT);
		}
		break;
	case CEnding::REM_BG:
		Animation();
		if (m_pKeyMgr->KeyDown(KEY_ENTER))
		{
			m_eCurState = CEnding::PLAYER_DIE;
			m_pObjectMgr->GetPlayer()->Die();
		}
		break;
	case CEnding::PLAYER_DIE:
		m_pObjectMgr->Update();
		if (m_pObjectMgr->GetPlayer()->GetDeadEffect() && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
		{
			m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
			m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
			m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
			m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
			m_pSceneMgr->SceneChange(SCENE_STAGE1, SCENE_HOUSE);
			m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"RemBG", L"RemBG");
			m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"RemBG", L"RemBGIntro");
			return	CHANGE_SCENE;
		}

		break;
	}
	return NO_EVENT;
}