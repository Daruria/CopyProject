#include "stdafx.h"
#include "Boon.h"
#include "ScreenEffect.h"
#include "AnimImp.h"
#include "BoonButton.h"
#include "Player.h"
#include "PlayerUI.h"

CBoon::CBoon():
	m_bIsDead(false),
	m_eCurState(CBoon::END),
	m_wstrStateKey(L""),
	m_fSort(0.f),
	m_pBoonButton1(nullptr),
	m_pBoonButton2(nullptr),
	m_pBoonButton3(nullptr),
	m_bOnceAnimFinish(false),
	m_fSelectEffectWaiting(0.f),
	m_bSelected(false)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CBoon::~CBoon()
{
	Release();
}

int CBoon::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	StateUpdate();

	m_pObjectMgr->AddLayer(m_eRenderType, this);

	return NO_EVENT;
}

void CBoon::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	if (CBoon::SELECT == m_eCurState)
	{
		m_pBoonButton1->LateUpdate();
		m_pBoonButton2->LateUpdate();
		m_pBoonButton3->LateUpdate();
	}
}

void CBoon::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Boon", m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CBoon::SELECT == m_eCurState)
	{
		m_pBoonButton1->Render();
		m_pBoonButton2->Render();
		m_pBoonButton3->Render();
	}
}

HRESULT CBoon::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	
	m_eCurState = CBoon::INTRO;
	m_wstrStateKey = L"BoonSelectIn";
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 12.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;

	CAnimImp* pImp = CAnimImp::Create(L"Boon", L"BoonSelectInFrontFx", 7);
	CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY* 0.5f, 0.f), RENDER_FRONT_UI, 1.f);
	m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

	m_pBoonButton1 = CBoonButton::Create(L"BoonZeusReflect",D3DXVECTOR3(0.f, -200.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), D3DXVECTOR3(590.f, 145.f, 0.f));
	m_pBoonButton2 = CBoonButton::Create(L"BoonZeusDash", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), D3DXVECTOR3(590.f, 145.f, 0.f));
	m_pBoonButton3 = CBoonButton::Create(L"BoonZeusCall", D3DXVECTOR3(0.f, 200.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), D3DXVECTOR3(590.f, 145.f, 0.f));

	m_eRenderType = RENDER_FRONT_UI;

	CSoundMgr::GetInstance()->PlaySound(L"DialoguePanelIn.wav", CSoundMgr::EFFECT);

	if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
		return S_OK;

	CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
	NULL_CHECK_RETURN(pPlayerUI, S_OK);

	pPlayerUI->UIGoOut();

	return S_OK;
}

void CBoon::Release()
{
	SafeDelete(m_pBoonButton1);
	SafeDelete(m_pBoonButton2);
	SafeDelete(m_pBoonButton3);
}

CBoon * CBoon::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, float fSort)
{
	CBoon* pInst = new CBoon;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_fSort = fSort;
	pInst->m_pBoonButton1->SetPos(vPos + D3DXVECTOR3(0.f, -150.f, 0.f));
	pInst->m_pBoonButton2->SetPos(vPos + D3DXVECTOR3(0.f, 0.f, 0.f));
	pInst->m_pBoonButton3->SetPos(vPos + D3DXVECTOR3(0.f, 150.f, 0.f));

	return pInst;
}

void CBoon::StateUpdate()
{
	switch (m_eCurState)
	{
	case CBoon::INTRO:
		IntroUpdate();
		break;
	case CBoon::SELECT:
		SelectUpdate();
		break;
	case CBoon::OUTRO:
		OutroUpdate();
		break;
	}
}

void CBoon::IntroUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_eCurState = CBoon::SELECT;
		m_bOnceAnimFinish = false;
	}
	Animation();
}

void CBoon::SelectUpdate()
{

	if (m_pBoonButton1->Update() && !m_bSelected)	//	Reflect
	{
		m_bSelected = true;

		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);

		pPlayer->SetCanReflectSkill(true);
	}

	if (m_pBoonButton2->Update() && !m_bSelected)	//	Dash
	{
		m_bSelected = true;

		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);

		pPlayer->SetCanDashSkill(true);
	}

	if (m_pBoonButton3->Update() && !m_bSelected)	//	Call
	{
		m_bSelected = true;

		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);

		pPlayer->SetCanCall(true);
	}

	if (m_bSelected)
	{
		m_fSelectEffectWaiting += m_pTimeMgr->GetDelta();

		if (m_fSelectEffectWaiting >= 0.5f)
		{
			m_eCurState = CBoon::OUTRO;
			m_wstrStateKey = L"BoonSelectOut";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 8.f;
			m_tFrame.fFrameSpeed = 1 / 30.f;
			m_tFrame.fFrameTime = 0.f;
			CSoundMgr::GetInstance()->PlaySound(L"DialoguePanelOut.wav", CSoundMgr::EFFECT);

			if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
				return;

			CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
			NULL_CHECK(pPlayerUI);

			pPlayerUI->UIComeIn();
		}
	}

}

void CBoon::OutroUpdate()
{

	if (m_bOnceAnimFinish)
	{
		m_bIsDead = true;
		m_bOnceAnimFinish = false;
		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);

		pPlayer->SetCanKeyInput(true);
	}

	Animation();
}

void CBoon::Animation()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame += static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		m_tFrame.fFrameTime = 0.f;

		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
			m_bOnceAnimFinish = true;
		}//if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	}//if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)

}
