#include "stdafx.h"
#include "Stage3.h"
#include "Player.h"
#include "Door.h"
#include "SummonRune.h"
#include "Crystal.h"
#include "Monster.h"
#include "Spawner.h"

CStage3::CStage3():
	m_eCurPhase(CStage3::END),
	m_fPhaseDelay(0.f),
	m_fTimeCount(0.f),
	m_pDoor(nullptr)
{
	Release();
}


CStage3::~CStage3()
{
	Release();
}

int CStage3::Update()
{
	PhaseUpdate();



	if (m_pKeyMgr->KeyDown(KEY_5))
	{
		AddOutro();
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_STAGE2;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
	}

	if (m_pKeyMgr->KeyDown(KEY_6))
	{
		AddOutro();
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_MEGAERA;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pObjectMgr->ObjectListClear(OBJECT_PLAYER_HITBOX);
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER_HITBOX);
	}

	m_pObjectMgr->Update();

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, NO_EVENT);

	CScrollMgr::Scrolling(pPlayer);

	if (m_eCurPhase == CStage3::END && m_pDoor->GetCanOpen())
	{
		m_bEButtonRender = true;
		if (m_pKeyMgr->KeyDown(KEY_E))
		{
			m_pDoor->DoorOpen();
			m_eCurState = CStage::OUTRO;
			m_eNextStage = SCENE_MEGAERA;
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
		m_pSceneMgr->SceneChange(SCENE_STAGE3, m_eNextStage);
		return	CHANGE_SCENE;
	}

}

void CStage3::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage3::Render()
{
	m_pObjectMgr->Render();

	EButtonRender();
}

HRESULT CStage3::Initialize()
{
	HRESULT hr = CStage::Initialize();

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	pPlayer->SetPos(4790.f, 1342.f);

	LoadMap(L"../Data/MapData3.dat");
	CAstarMgr::GetInstance()->ReadyAstarTile();
	CAstarMgr::GetInstance()->ReadyGraph();

	m_eCurState = CStage::PLAY;
	m_eCurPhase = CStage3::PHASE_START;

	m_pDoor = CDoor::Create(D3DXVECTOR3(5132.f, 656.f, 0.f), D3DXVECTOR3(0.9f, 0.9f, 0.f));
	m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, m_pDoor);

	m_fPhaseDelay = 2.f;

	return S_OK;
}

void CStage3::Release()
{

}

void CStage3::PhaseStart()
{
	m_eCurPhase = CStage3::PHASE1;

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5030.f, 950.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CCrystal::Create(D3DXVECTOR3(5030.f, 950.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5200.f, 1050.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CCrystal::Create(D3DXVECTOR3(5200.f, 1050.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5440.f, 1240.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CSpawner::Create(D3DXVECTOR3(5440.f, 1240.f, 0.f))));

	m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5280.f, 1450.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
		CSpawner::Create(D3DXVECTOR3(5280.f, 1450.f, 0.f))));
}

void CStage3::Phase1Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5030.f, 950.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CCrystal::Create(D3DXVECTOR3(5030.f, 950.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5200.f, 1050.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CCrystal::Create(D3DXVECTOR3(5200.f, 1050.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5440.f, 1240.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CSpawner::Create(D3DXVECTOR3(5440.f, 1240.f, 0.f))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CSummonRune::Create(D3DXVECTOR3(5280.f, 1450.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f),
				CSpawner::Create(D3DXVECTOR3(5280.f, 1450.f, 0.f))));


			m_eCurPhase = CStage3::PHASE2;
			m_fTimeCount = 0.f;
		}
	}
}

void CStage3::Phase2Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_eCurPhase = CStage3::END;
			AddBoonOrb();
		}
	}
}

void CStage3::Phase3Update()
{
	if (m_pObjectMgr->GetObjectList(OBJECT_MONSTER)->empty())
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fPhaseDelay)
		{
			m_eCurPhase = CStage3::END;
			AddBoonOrb();
		}
	}
}

void CStage3::PhaseUpdate()
{
	switch (m_eCurPhase)
	{
	case CStage3::PHASE_START:
		PhaseStart();
		break;
	case CStage3::PHASE1:
		Phase1Update();
		break;
	case CStage3::PHASE2:
		Phase2Update();
		break;
	case CStage3::PHASE3:
		Phase3Update();
		break;
	case CStage3::END:

		break;
	}
}