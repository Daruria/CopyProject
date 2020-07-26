#include "stdafx.h"
#include "House.h"
#include "Player.h"
#include "MoveEffect.h"
#include "AnimImp.h"

CHouse::CHouse():
	m_bShowTitle(false)
{
}


CHouse::~CHouse()
{
	Release();
}

int CHouse::Update()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();

	if (!m_bShowTitle && pPlayer->GetState() == CPlayer::IDLE)
	{
		pPlayer->SetColl(true);
		CMathMgr::AddTitleEffect(L"TheHouseOfHades");
		m_bShowTitle = true;
	}

	if (pPlayer->GetInfo().vPos.x > 5740.f && pPlayer->GetInfo().vPos.y > 1240.f &&
		m_pKeyMgr->KeyDown(KEY_E))
	{
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_STAGE1;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		AddOutro();
	}

	m_pObjectMgr->Update();

	CScrollMgr::Scrolling(pPlayer);

	if (CStage::OUTRO == m_eCurState && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pSceneMgr->SceneChange(SCENE_HOUSE, m_eNextStage);
		return	CHANGE_SCENE;
	}

	return NO_EVENT;
}

void CHouse::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CHouse::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CHouse::Initialize()
{
	HRESULT hr = CStage::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	pPlayer->Reset();
	pPlayer->SetPos(4330.f, 1460.f);
	pPlayer->SetColl(false);

	LoadMap(L"../Data/MapHouse.dat");
	CAstarMgr::GetInstance()->ReadyAstarTile();
	CAstarMgr::GetInstance()->ReadyGraph();

	m_eCurState = CStage::PLAY;

	return S_OK;
}

void CHouse::Release()
{
}
