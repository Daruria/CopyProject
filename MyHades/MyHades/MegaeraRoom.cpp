#include "stdafx.h"
#include "MegaeraRoom.h"
#include "Player.h"
#include "Alecto.h"
#include "ConversationImp.h"
#include "ScreenEffect.h"
#include "PlayerUI.h"
#include "BossUI.h"

CMegaeraRoom::CMegaeraRoom():
	m_eCurPhase(CMegaeraRoom::PHASE_END),
	m_eCurTalk(CMegaeraRoom::TALK_END),
	m_pAlecto(nullptr),
	m_bScrollMoving(false)
{
}


CMegaeraRoom::~CMegaeraRoom()
{
	Release();
}

int CMegaeraRoom::Update()
{
	if (m_pKeyMgr->KeyDown(KEY_5))
	{
		AddOutro();
		m_eCurState = CStage::OUTRO;
		m_eNextStage = SCENE_STAGE3;
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
	}

	//if (m_pKeyMgr->KeyDown(KEY_F6))
	//{
	//	AddOutro();
	//	m_eCurState = CStage::OUTRO;
	//	m_eNextStage = SCENE_MEGAERA;
	//}

	

	m_pObjectMgr->Update();

	PhaseUpdate();
	
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, NO_EVENT);

	if (CMegaeraRoom::DIE == m_eCurPhase)
	{
		if (pPlayer->GetInfo().vPos.x > 5200.f && pPlayer->GetInfo().vPos.y < 700.f)
		{
			m_bEButtonRender = true;
			if (m_pKeyMgr->KeyDown(KEY_E))
			{
				m_eCurState = CStage::OUTRO;
				m_eNextStage = SCENE_ENDING;
				AddOutro();
			}
		}
		else
		{
			m_bEButtonRender = false;
		}
	}

	if (m_bScrollMoving)
		Scrolling();
	else
		CScrollMgr::Scrolling(pPlayer);

	if (pPlayer->GetDeadEffect() && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);
		m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
		m_pSceneMgr->SceneChange(SCENE_STAGE1, SCENE_HOUSE);
		return	CHANGE_SCENE;
	}

	if (CStage::OUTRO == m_eCurState && m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{
		m_pSceneMgr->SceneChange(SCENE_MEGAERA, m_eNextStage);
		return	CHANGE_SCENE;
	}

	return NO_EVENT;
}

void CMegaeraRoom::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CMegaeraRoom::Render()
{
	m_pObjectMgr->Render();

	EButtonRender();
}

HRESULT CMegaeraRoom::Initialize()
{
	HRESULT hr = CStage::Initialize();

	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::GetInstance()->PlayBGM(L"Hades - Scourge of the Furies.mp3");
	
	LoadMap(L"../Data/MapData4.dat");
	CAstarMgr::GetInstance()->ReadyAstarTile();
	CAstarMgr::GetInstance()->ReadyGraph();
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	pPlayer->SetPos(3490.f, 1520.f);

	m_pAlecto = CAlecto::Create(D3DXVECTOR3(4290.f, 1160.f, 0.f), this);

	m_pObjectMgr->AddObject(OBJECT_MONSTER, m_pAlecto);

	m_eCurPhase = CMegaeraRoom::MEET;
	m_eCurTalk = CMegaeraRoom::Alecto0;

	CThreadMgr::GetInstance()->WaitForThread();

	return S_OK;
}

void CMegaeraRoom::Release()
{
	
}

void CMegaeraRoom::PhaseUpdate()
{
	switch (m_eCurPhase)
	{
	case CMegaeraRoom::MEET:
		MeetUpdate();
		break;
	case CMegaeraRoom::GREET:
		GreetUpdate();
		break;
	case CMegaeraRoom::BATTLE:
		BattleUpdate();
		break;
	case CMegaeraRoom::DIE:
		DieUpdate();
		break;
	}
}

void CMegaeraRoom::MeetUpdate()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	if (pPlayer->GetInfo().vPos.x >= 3830.f && pPlayer->GetInfo().vPos.y <= 1360.f)
	{
		m_bScrollMoving = true;
		m_vScrollTarget = m_pAlecto->GetInfo().vPos;
		m_vScroll = pPlayer->GetInfo().vPos;
		m_eCurPhase = CMegaeraRoom::GREET;
		m_eCurTalk = CMegaeraRoom::Alecto0;

		CConversationImp* pImp = CConversationImp::Create(L"Alecto_Name", L"Portraits_Alecto", L"Conversation", L"Alecto", 0);
		CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);
		pPlayer->SetCanKeyInput(false);
		pPlayer->SetState(CPlayer::IDLE);
		CSoundMgr::GetInstance()->PlaySound(L"Alecto_0002.wav", CSoundMgr::EFFECT);

		if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
			return ;

		CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
		NULL_CHECK(pPlayerUI);

		pPlayerUI->UIGoOut();
	}
}

void CMegaeraRoom::GreetUpdate()
{
	if (!(m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty()))
		return;

	switch (m_eCurTalk)
	{
	case CMegaeraRoom::Alecto0:
	{
		CConversationImp* pImp = CConversationImp::Create(L"Zagreus_Name", L"Portraits_Zagreus", L"Conversation", L"Zagreus", 0);
		CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);
		m_eCurTalk = CMegaeraRoom::Zagreus0;
		m_vScrollTarget = m_pObjectMgr->GetPlayer()->GetInfo().vPos;
		
		CSoundMgr::GetInstance()->PlaySound(L"ZagreusField_You Must.wav", CSoundMgr::EFFECT);
	}
		break;
	case CMegaeraRoom::Zagreus0:
	{
		CConversationImp* pImp = CConversationImp::Create(L"Alecto_Name", L"Portraits_Alecto", L"Conversation", L"Alecto", 1);
		CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);
		m_eCurTalk = CMegaeraRoom::Alecto1;
		m_vScrollTarget = m_pAlecto->GetInfo().vPos;
		CSoundMgr::GetInstance()->PlaySound(L"Alecto_0003.wav", CSoundMgr::EFFECT);
	}
		break;
	case CMegaeraRoom::Alecto1:
	{
		CConversationImp* pImp = CConversationImp::Create(L"Zagreus_Name", L"Portraits_Zagreus", L"Conversation", L"Zagreus", 1);
		CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);
		m_eCurTalk = CMegaeraRoom::Zagreus1;
		m_vScrollTarget = m_pObjectMgr->GetPlayer()->GetInfo().vPos;
		CSoundMgr::GetInstance()->PlaySound(L"ZagreusField_I get the feeling.wav", CSoundMgr::EFFECT);
	}
		break;
	case CMegaeraRoom::Zagreus1:
	{
		CConversationImp* pImp = CConversationImp::Create(L"Alecto_Name", L"Portraits_Alecto", L"Conversation", L"Alecto", 2);
		CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI);
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);
		m_eCurTalk = CMegaeraRoom::Alecto2;
		m_vScrollTarget = m_pAlecto->GetInfo().vPos;
		CSoundMgr::GetInstance()->PlaySound(L"Alecto_0004.wav", CSoundMgr::EFFECT);
	}
		break;
	case CMegaeraRoom::Alecto2:
		m_eCurPhase = CMegaeraRoom::BATTLE;
		m_vScrollTarget = m_pObjectMgr->GetPlayer()->GetInfo().vPos;
		m_pObjectMgr->GetPlayer()->SetCanKeyInput(true);
		m_pAlecto->Start();

		if (m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->empty())
			return;

		CPlayerUI* pPlayerUI = dynamic_cast<CPlayerUI*>(m_pObjectMgr->GetObjectList(OBJECT_PLAYER_UI)->front());
		NULL_CHECK(pPlayerUI);

		pPlayerUI->UIComeIn();
		break;
	}
}

void CMegaeraRoom::BattleUpdate()
{
	if (m_bScrollMoving)
	{
		m_vScrollTarget = m_pObjectMgr->GetPlayer()->GetInfo().vPos;
		D3DXVECTOR3 vDist = m_vScrollTarget - m_vScroll;
		float fLength = D3DXVec3Length(&vDist);

		if (fLength < 20.f)
			m_bScrollMoving = false;
	}

}

void CMegaeraRoom::DieUpdate()
{
	if (m_pAlecto)
		m_pAlecto = nullptr;

	if (m_pObjectMgr->GetObjectList(OBJECT_FRONT_UI)->empty())
	{

	}
}

void CMegaeraRoom::Scrolling()
{
	D3DXVECTOR3 vDir = m_vScrollTarget - m_vScroll;
	float	fDist = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_vScroll += vDir * m_pTimeMgr->GetDelta() * fDist;
	CScrollMgr::Scrolling(m_vScroll);
}