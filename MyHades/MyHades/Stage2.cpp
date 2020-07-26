#include "stdafx.h"
#include "Stage2.h"
#include "Player.h"
#include "Door.h"
#include "SummonRune.h"
#include "Crystal.h"
#include "Monster.h"
#include "Spawner.h"


CStage2::CStage2():
	m_eCurPhase(CStage2::END),
	m_fPhaseDelay(0.f),
	m_fTimeCount(0.f),
	m_pDoor(nullptr)
{
}


CStage2::~CStage2()
{
	Release();
}

int CStage2::Update()
{
	PhaseUpdate();

	if (m_pKeyMgr->KeyDown(KEY_5))
	{
		AddOutro();
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_STAGE1;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
	}

	if (m_pKeyMgr->KeyDown(KEY_6))
	{
		AddOutro();
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_STAGE3;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
	}

	m_pObjectMgr->Update();

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, NO_EVENT);
	CScrollMgr::Scrolling(pPlayer);

	if (m_eCurPhase == CStage2::END && m_pDoor->GetCanOpen())
	{
		m_bEButtonRender = true;
		if (m_pKeyMgr->KeyDown(KEY_E))
		{
			m_pDoor->DoorOpen();
			m_eCurState = CStage::OUTRO;
			m_eNextStage = SCENE_STAGE3;
			AddOutro();
		}
	}
	else
		m_bEButtonRender = false;




	if (pPlayer->GetDeadEffect() && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
		m_pSceneMgr->SceneChange(SCENE_STAGE1, SCENE_HOUSE);
		return	CHANGE_SCENE;
	}

	if (CStage::OUTRO == m_eCurState && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
		m_pSceneMgr->SceneChange(SCENE_STAGE2, m_eNextStage);
		return	CHANGE_SCENE;
	}

	return NO_EVENT;
}

void CStage2::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage2::Render()
{
	m_pObjectMgr->Render();
	EButtonRender();
}

HRESULT CStage2::Initialize()
{
	HRESULT hr = CStage::Initialize();

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	
	pPlayer->SetPos(4480.f, 1470.f);

	LoadMap(L"../Data/MapData2.dat");
	CAstarMgr::GetInstance()->ReadyAstarTile();
	CAstarMgr::GetInstance()->ReadyGraph();

	m_eCurState = CStage::PLAY;
	m_eCurPhase = CStage2::PHASE_START;

	m_pDoor = CDoor::Create(D3DXVECTOR3(5462.f, 1225.f, 0.f), D3DXVECTOR3(0.6f, 0.6f, 0.f));
	m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, m_pDoor);

	m_fPhaseDelay = 2.f;

	CThreadMgr::GetInstance()->LoadingThread(L"../Data/AlectoLoading.txt");

	return S_OK;
}

void CStage2::Release()
{
}

void CStage2::PhaseUpdate()
{
	switch (m_eCurPhase)
	{
	case CStage2::PHASE_START:
		PhaseStart();
		break;
	case CStage2::PHASE1:
		Phase1Update();
		break;
	case CStage2::PHASE2:
		Phase2Update();
		break;
	case CStage2::END:

		break;
	}
}

void CStage2::PhaseStart()
{
	m_eCurPhase = CStage2::PHASE1;

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(4740.f, 1130.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CCrystal::Create(D3DXVECTOR3(4740.f, 1130.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5330.f, 1470.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CCrystal::Create(D3DXVECTOR3(5330.f, 1470.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5750.f, 1530.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CSpawner::Create(D3DXVECTOR3(5750.f, 1530.f, 0.f))));

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5380.f, 1690.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CSpawner::Create(D3DXVECTOR3(5380.f, 1690.f, 0.f))));
}

void CStage2::Phase1Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(4740.f, 1130.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CCrystal::Create(D3DXVECTOR3(4740.f, 1130.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5330.f, 1470.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CCrystal::Create(D3DXVECTOR3(5330.f, 1470.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5750.f, 1530.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CSpawner::Create(D3DXVECTOR3(5750.f, 1530.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5380.f, 1690.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CSpawner::Create(D3DXVECTOR3(5380.f, 1690.f, 0.f))));

			m_eCurPhase = CStage2::PHASE2;
			m_fTimeCount = 0.f;
		}
	}
}

void CStage2::Phase2Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_eCurPhase = CStage2::END;
			AddBoonOrb();
		}
	}
}