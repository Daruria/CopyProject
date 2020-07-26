#include "stdafx.h"
#include "Stage.h"
#include "ScreenEffect.h"
#include "AnimImp.h"
#include "AnimReverse.h"
#include "Player.h"
#include "NormalEffect.h"
#include "BoonOrb.h"
#include "PlayerUI.h"

CStage::CStage():
	m_eCurState(CStage::END),
	m_bIsAddBoonOrb(false),
	m_bEButtonRender(false)
{
}

CStage::~CStage()
{
	Release();
}

int CStage::Update()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, NO_EVENT);

	m_pObjectMgr->Update();

	CScrollMgr::Scrolling(pPlayer);

	return NO_EVENT;
}

void CStage::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage::Render()
{	
}

HRESULT CStage::Initialize()
{
	m_eCurState = CStage::INTRO;

	AddIntro();

	return S_OK;
}

void CStage::Release()
{

}

void CStage::AddIntro()
{
	FRAME tFrame;
	tFrame.eType = ANIM_ONCE;
	tFrame.fMaxFrame = m_pTextureMgr->GetTextureMaxFrame(L"RoomTransition", L"RoomTransition");
	tFrame.fCurFrame = tFrame.fMaxFrame - 1.f;
	tFrame.fFrameTime = 0.f;
	tFrame.fFrameSpeed = 1 / 30.f;

	CScreenEffect* pEffect = CScreenEffect::Create(CAnimReverse::Create(L"RoomTransition", L"RoomTransition", tFrame, D3DCOLOR_ARGB(255, 255, 255, 255)),
		D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI);
	if (pEffect)
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);
	
	if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
		return;

	CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
	NULL_CHECK(pPlayerUI);

	pPlayerUI->UIComeIn();

}

void CStage::AddOutro()
{
	FRAME tFrame;
	tFrame.eType = ANIM_ONCE;
	tFrame.fMaxFrame = m_pTextureMgr->GetTextureMaxFrame(L"RoomTransition", L"RoomTransition");
	tFrame.fCurFrame = 0.f;
	tFrame.fFrameTime = 0.f;
	tFrame.fFrameSpeed = 1 / 30.f;

	CScreenEffect* pEffect = CScreenEffect::Create(CAnimImp::Create(L"RoomTransition", L"RoomTransition", tFrame, D3DCOLOR_ARGB(255, 255, 255, 255)),
		D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI, 0.f);
	if (pEffect)
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

	if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
		return;

	CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
	NULL_CHECK(pPlayerUI);

	pPlayerUI->UIGoOut();
}

void CStage::AddBoonOrb()
{
	if (!m_bIsAddBoonOrb)
	{
		m_bIsAddBoonOrb = true;
		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);

		CBoonOrb* pOrb = CBoonOrb::Create(pPlayer->GetInfo().vPos, D3DXVECTOR3(0.25f, 0.25f, 0.f));
		m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, pOrb);

		CAnimImp* pImp = CAnimImp::Create(L"Fx", L"PickupFlareA", 15);
		CNormalEffect* pEffect = CNormalEffect::Create(pImp, pPlayer->GetInfo().vPos, RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

		CSoundMgr::GetInstance()->PlaySound(L"ChoirZoomIn.wav", CSoundMgr::EFFECT);
	}
}

void CStage::EButtonRender()
{
	if (m_bEButtonRender)
	{
		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY - 100.f, 0.f);

		matWorld = matScale * matTrans;

		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"EButton");
		NULL_CHECK(pTexInfo);
		D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
