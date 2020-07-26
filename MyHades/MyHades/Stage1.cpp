#include "stdafx.h"
#include "Stage1.h"
#include "Player.h"
#include "Terrain.h"
#include "AnimatedObject.h"
#include "NonAnimatedObject.h"
#include "SummonRune.h"
#include "Crystal.h"
#include "Monster.h"
#include "Spawner.h"
#include "Mine.h"
#include "Door.h"
#include "AnimImp.h"
#include "NonAnimImp.h"
#include "ScreenEffect.h"
#include "NormalEffect.h"
#include "BoonOrb.h"
#include "PlayerUI.h"

bool CStage1::m_bShowTitle = false;

CStage1::CStage1():
	m_eCurPhase(CStage1::END),
	m_fPhaseDelay(0.f),
	m_fTimeCount(0.f),
	m_fTitleDelay(0.f),
	m_pDoor(nullptr)
{
}

CStage1::~CStage1()
{
	Release();
}

int CStage1::Update()
{
	TitleCheck();

	PhaseUpdate();

	if (m_pKeyMgr->KeyDown(KEY_F1))
	{
		AddBoonOrb();
	}


	if (m_pKeyMgr->KeyDown(KEY_6))
	{
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_STAGE2;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
		AddOutro();
	}

	m_pObjectMgr->Update();

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, NO_EVENT);

	CScrollMgr::Scrolling(pPlayer);

	if (m_eCurPhase == CStage1::END && m_pDoor->GetCanOpen())
	{
		m_bEButtonRender = true;
		if (m_pKeyMgr->KeyDown(KEY_E))
		{
			m_pDoor->DoorOpen();
			m_eCurState = CStage::OUTRO;
			m_eNextStage = SCENE_STAGE2;
			m_bEButtonRender = false;
			AddOutro();
		}
	}
	else
		m_bEButtonRender = false;

	//	Test
	if (m_pKeyMgr->KeyDown(KEY_F1))
	{
		CBoonOrb* pOrb = CBoonOrb::Create(pPlayer->GetInfo().vPos, D3DXVECTOR3(0.25f, 0.25f, 0.f));
		m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, pOrb);

		CAnimImp* pImp = CAnimImp::Create(L"Fx", L"PickupFlareA", 15);
		CNormalEffect* pEffect = CNormalEffect::Create(pImp, pPlayer->GetInfo().vPos, RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
	}

	if (pPlayer->GetDeadEffect() && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
		m_pSceneMgr->SceneChange(SCENE_STAGE1, SCENE_HOUSE);
		CStage1::m_bShowTitle = false;
		return	CHANGE_SCENE;
	}

	if (CStage::OUTRO == m_eCurState && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
		m_pSceneMgr->SceneChange(SCENE_STAGE1, m_eNextStage);
		return	CHANGE_SCENE;
	}

	return NO_EVENT;
}

void CStage1::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage1::Render()
{
	m_pObjectMgr->Render();
	EButtonRender();
}

HRESULT CStage1::Initialize()
{
	HRESULT hr = CStage::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	if (!m_pObjectMgr->GetPlayer())
	{
		CPlayer* pPlayer = CPlayer::Create();
		NULL_CHECK_MSG_RETURN(pPlayer, L"Player Create Failed", E_FAIL);
		m_pObjectMgr->AddObject(OBJECT_PLAYER, pPlayer);
	}

	if (!m_pObjectMgr->GetTerrain())
	{
		CTerrain* pTerrain = CTerrain::Create();
		NULL_CHECK_MSG_RETURN(pTerrain, L"Terrain Create Failed", E_FAIL);
		m_pObjectMgr->AddObject(OBJECT_TERRAIN, pTerrain);
	}

	m_pObjectMgr->GetPlayer()->SetPos(4650.f, 1350.f);
	
	LoadMap(L"../Data/MapData1.dat");

	CAstarMgr::GetInstance()->ReadyAstarTile();
	CAstarMgr::GetInstance()->ReadyGraph();

	m_eCurState = CStage::PLAY;
	m_eCurPhase = CStage1::PHASE_START;

	m_pDoor = CDoor::Create(D3DXVECTOR3(7010.f, 430.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f));
	m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, m_pDoor);

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CMine::Create(D3DXVECTOR3(5620.f, 1170.f, 0.f)));
	m_pObjectMgr->AddObject(OBJECT_EFFECT, CMine::Create(D3DXVECTOR3(5820.f, 1360.f, 0.f)));
	m_pObjectMgr->AddObject(OBJECT_EFFECT, CMine::Create(D3DXVECTOR3(5860.f, 1050.f, 0.f)));
	m_pObjectMgr->AddObject(OBJECT_EFFECT, CMine::Create(D3DXVECTOR3(6050.f, 1260.f, 0.f)));
	m_pObjectMgr->AddObject(OBJECT_EFFECT, CMine::Create(D3DXVECTOR3(6080.f, 940.f, 0.f)));
	m_pObjectMgr->AddObject(OBJECT_EFFECT, CMine::Create(D3DXVECTOR3(6330.f, 1130.f, 0.f)));

	m_fPhaseDelay = 2.f;

	CSoundMgr::GetInstance()->PlayBGM(L"ThanatosThemeMUSIC.wav");
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.4f);

	if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
		return S_OK;

	CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
	NULL_CHECK_RETURN(pPlayerUI, S_OK);

	pPlayerUI->UIComeIn();

	return S_OK;
}

void CStage1::Release()
{
}

void CStage1::PhaseStart()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	if (pPlayer->GetInfo().vPos.x > 6240.f &&
		pPlayer->GetInfo().vPos.y < 1000.f)
	{
		m_eCurPhase = CStage1::PHASE1;

		m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(6500.f, 740.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
			CCrystal::Create(D3DXVECTOR3(6500.f, 740.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

		m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(6820.f, 850.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
			CCrystal::Create(D3DXVECTOR3(6820.f, 850.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

		m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(6680.f, 670.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
			CSpawner::Create(D3DXVECTOR3(6680.f, 670.f, 0.f))));

		m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(7010.f, 759.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
			CSpawner::Create(D3DXVECTOR3(7010.f, 759.f, 0.f))));
	}
}

void CStage1::Phase1Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty() && m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(6500.f, 740.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CCrystal::Create(D3DXVECTOR3(6500.f, 740.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(6820.f, 850.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CCrystal::Create(D3DXVECTOR3(6820.f, 850.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(6680.f, 670.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CSpawner::Create(D3DXVECTOR3(6680.f, 670.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(7010.f, 759.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CSpawner::Create(D3DXVECTOR3(7010.f, 759.f, 0.f))));


			m_eCurPhase = CStage1::PHASE2;
			m_fTimeCount = 0.f;
		}
	}
}

void CStage1::Phase2Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty() && m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_eCurPhase = CStage1::END;
			AddBoonOrb();
		}
	}
}

void CStage1::Phase3Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty() && m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_eCurPhase = CStage1::END;
			AddBoonOrb();
		}
	}
}

void CStage1::TitleCheck()
{
	if (!m_bShowTitle)
	{
		m_fTitleDelay += m_pTimeMgr->GetDelta();
		if (m_fTitleDelay >= 1.f)
		{
			m_bShowTitle = true;

			CMathMgr::AddTitleEffect(L"TartarusFont");
		}
	}

}


void CStage1::PhaseUpdate()
{
	switch (m_eCurPhase)
	{
	case CStage1::PHASE_START:
		PhaseStart();
		break;
	case CStage1::PHASE1:
		Phase1Update();
		break;
	case CStage1::PHASE2:
		Phase2Update();
		break;
	case CStage1::PHASE3:
		Phase3Update();
		break;
	case CStage1::END:

		break;
	}
}