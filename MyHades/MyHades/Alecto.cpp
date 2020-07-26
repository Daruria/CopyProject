#include "stdafx.h"
#include "Alecto.h"
#include "Player.h"
#include "NormalHitBox.h"
#include "NormalEffect.h"
#include "AnimImp.h"
#include "Boom.h"
#include "MegaeraRoom.h"
#include "ScreenEffect.h"
#include "NonAnimImp.h"
#include "BossObserver.h"
#include "BossUI.h"

CAlecto::CAlecto() :
	m_ePreState(CAlecto::STATE_END),
	m_eCurState(CAlecto::STATE_END),
	m_eCurPattern(CAlecto::PATTERN_END),
	m_wstrStateKey(L""),
	m_bPlayStart(false),
	m_fIdleCount(0.f),
	m_fIdleTime(0.f),
	m_fSpeed(0.f),
	m_bAddHitBox(false),
	m_pScene(nullptr),
	m_pBossUI(nullptr)
{
}


CAlecto::~CAlecto()
{
	Release();
}

int CAlecto::Update()
{
	if (m_tStatus.iHp <= 0)
	{
		dynamic_cast<CMegaeraRoom*>(m_pScene)->SetPhase(CMegaeraRoom::DIE);
		DieUpdate();

		return DEAD_OBJ;
	}

	ChangeState();
	
	Animation();

	StateUpdate();

	m_pObjectMgr->AddLayer(RENDER_OBJECT, this);

	return NO_EVENT;
}

void CAlecto::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CAlecto::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame) + m_tFrame.fMaxFrame * GetDir());
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#ifdef _DEBUG
	CMathMgr::DrawLineBox(m_pDeviceMgr, m_tInfo.vPos, m_tHitBox.vSize, 2.f, D3DCOLOR_ARGB(255, 255, 0, 255));
#endif // _DEBUG

}

HRESULT CAlecto::Initialize()
{
	HRESULT hr = CMonster::Initialize();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_wstrObjectKey = L"Alecto";

	m_eCurState = CAlecto::STATE_IDLE;
	m_eCurPattern = CAlecto::PATTERN_LUNGE;

	m_fIdleTime = 2.f;
	m_fSpeed = 600.f;

	m_tInfo.vDir = {-1.f, 1.f, 0.f};
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tStatus.iMaxHp = 2000;
	m_tStatus.iHp = 2000;
	m_tStatus.iAttack = 10;

	CSubject* pSubject = CSubject::Create();
	hr = m_pSubjectMgr->AddSubject(L"Boss", pSubject);
	if (FAILED(hr))
		SafeDelete(pSubject);
	
	m_pBossUI = CBossUI::Create();

	m_pObjectMgr->AddObject(OBJECT_BOSS_UI, m_pBossUI);
	m_pSubjectMgr->Notify(L"Boss", CBossObserver::MAX_HP, &m_tStatus.iMaxHp);
	m_pSubjectMgr->Notify(L"Boss", CBossObserver::HP, &m_tStatus.iHp);
	m_pSubjectMgr->Notify(L"Boss", CBossObserver::ATK, &m_tStatus.iAttack);
	wstring wstrNameKey = L"Alecto_HpBarName";
	m_pSubjectMgr->Notify(L"Boss", CBossObserver::NAME, &wstrNameKey);

	return S_OK;
}

void CAlecto::Release()
{
	m_pBossUI->Die();

	m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"Alecto", L"Enemy_Alecto_Idle");
	m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"Alecto", L"Enemy_Alecto_Lunge");
	m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"Alecto", L"Enemy_Alecto_Run");
	m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"Alecto", L"Enemy_Alecto_Shielded");
	m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"Alecto", L"Enemy_Alecto_WhipWhirl");

	CThreadMgr::GetInstance()->LoadingThread(L"../Data/RemBGPathInfo.txt");
}

CAlecto * CAlecto::Create(const D3DXVECTOR3 & vPos, CScene* pScene)
{
	CAlecto* pInst = new CAlecto;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_pScene = pScene;

}

void CAlecto::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	CMonster::Hit(vPos, iAttack);

	CSoundMgr::GetInstance()->PlaySound(L"BloodSplatter combo.wav", CSoundMgr::EFFECT);
	CSoundMgr::GetInstance()->PlaySound(L"Alecto_EmoteHurt_1.wav", CSoundMgr::MONSTER);


	m_pSubjectMgr->Notify(L"Boss", CBossObserver::HP, &m_tStatus.iHp);
}

void CAlecto::Start()
{
	m_bPlayStart = true;

	if (!m_pObjectMgr->GetObjectList(OBJECT_BOSS_UI)->empty())
	{
		CBossUI* pBossUI = dynamic_cast<CBossUI*>(m_pObjectMgr->GetObjectList(OBJECT_BOSS_UI)->front());
		NULL_CHECK(pBossUI);

		pBossUI->UIComeIn();
	}
}

void CAlecto::ChangeState()
{
	if (m_ePreState == m_eCurState)
		return;
	switch (m_eCurState)
	{
	case CAlecto::STATE_IDLE:
		IdleState();
		break;
	case CAlecto::STATE_RUN:
		RunState();
		break;
	case CAlecto::STATE_LUNGE:
		LungeState();
		break;
	case CAlecto::STATE_BOOM:
		BoomState();
		break;
	case CAlecto::STATE_WHIP_WHIRL:
		WhipWhirlState();
		break;
	case CAlecto::DIE:
		DieState();
		break;
	}


	m_ePreState = m_eCurState;
}

void CAlecto::IdleState()
{
	m_tFrame.eType = ANIM_REPEAT;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 30.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_wstrStateKey = L"Enemy_Alecto_Idle";
}

void CAlecto::RunState()
{
	m_tFrame.eType = ANIM_REPEAT;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 11.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_wstrStateKey = L"Enemy_Alecto_Run";
}

void CAlecto::LungeState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 38.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_wstrStateKey = L"Enemy_Alecto_Lunge";
}

void CAlecto::BoomState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 150.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_wstrStateKey = L"Enemy_Alecto_Shielded";
}

void CAlecto::WhipWhirlState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 45.f;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
	m_wstrStateKey = L"Enemy_Alecto_WhipWhirl";
}

void CAlecto::DieState()
{
}

void CAlecto::StateUpdate()
{
	switch (m_eCurState)
	{
	case CAlecto::STATE_IDLE:
		IdleUpdate();
		break;
	case CAlecto::STATE_RUN:
		RunUpdate();
		break;
	case CAlecto::STATE_LUNGE:
		LungeUpdate();
		break;
	case CAlecto::STATE_BOOM:
		BoomUpdate();
		break;
	case CAlecto::STATE_WHIP_WHIRL:
		WhipWhirlUpdate();
		break;
	case CAlecto::DIE:
		DieUpdate();
		break;
	}

}

void CAlecto::IdleUpdate()
{
	if (!m_bPlayStart)
		return;

	m_fIdleCount += m_pTimeMgr->GetDelta();

	if (m_fIdleCount >= m_fIdleTime)
	{
		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);

		D3DXVECTOR3 vDist = pPlayer->GetInfo().vPos - m_tInfo.vPos;

		D3DXVec3Normalize(&vDist, &vDist);
		m_tInfo.vDir = vDist;

		m_eCurState = CAlecto::STATE_RUN;
	}
}

void CAlecto::RunUpdate()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	D3DXVECTOR3 vDist = pPlayer->GetInfo().vPos - m_tInfo.vPos;
	float fDistance = D3DXVec3Length(&vDist);
	D3DXVec3Normalize(&vDist, &vDist);

	m_tInfo.vDir = vDist;
	m_tInfo.vLook = m_tInfo.vDir;

	m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetDelta() * m_fSpeed;

	switch (m_eCurPattern)
	{
	case CAlecto::PATTERN_LUNGE:
		if (fDistance <= 500.f) 
		{ 
			m_eCurState = CAlecto::STATE_LUNGE; 
			CSoundMgr::GetInstance()->PlaySound(L"Alecto_EmoteAttacking_1.wav", CSoundMgr::EFFECT);
			CSoundMgr::GetInstance()->PlaySound(L"AuraOff.wav", CSoundMgr::EFFECT);
		}
		break;
	case CAlecto::PATTERN_BOOM:
		if (fDistance <= 600.f)
		{
			m_eCurState = CAlecto::STATE_BOOM;
			CSoundMgr::GetInstance()->PlaySound(L"Alecto_EmoteLaugh_11.wav", CSoundMgr::EFFECT);
		}
		break;
	case CAlecto::PATTERN_WHIP_WHIRL:
		if (fDistance < 100.f)
		{
			m_eCurState = CAlecto::STATE_WHIP_WHIRL;
			CSoundMgr::GetInstance()->PlaySound(L"Alecto_EmoteAttacking_2.wav", CSoundMgr::EFFECT);
		}
		break;
	}
}

void CAlecto::LungeUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_bOnceAnimFinish = false;
		m_eCurPattern	= CAlecto::PATTERN_BOOM;
		m_eCurState		= CAlecto::STATE_IDLE;
		return;
	}

	int iFrame = static_cast<int>(m_tFrame.fCurFrame);

	if (iFrame >= 11 && iFrame <= 18)
		m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetDelta() * 1500.f;
	
	if (!m_bAddHitBox && 11 == iFrame)
	{
		m_bAddHitBox = true;

		CNormalHitBox* pHitBox = CNormalHitBox::Create(m_tInfo, D3DXVECTOR3(100.f, 100.f, 0.f), this, 8 / 30.f, m_tStatus.iAttack, 1500.f);
		m_pObjectMgr->AddObject(OBJECT_MONSTER_HITBOX, pHitBox);
	}
	else if (m_bAddHitBox && 12 == iFrame)
		m_bAddHitBox = false;
}

void CAlecto::BoomUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_bOnceAnimFinish = false;
		m_eCurPattern = CAlecto::PATTERN_WHIP_WHIRL;
		m_eCurState = CAlecto::STATE_IDLE;
		return;
	}

	int iFrame = static_cast<int>(m_tFrame.fCurFrame);

	if (!m_bAddHitBox && (15 == iFrame || 45 == iFrame || 75 == iFrame || 105 == iFrame || 135 == iFrame))
	{
		m_bAddHitBox = true;

		CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
		NULL_CHECK(pPlayer);
		D3DXVECTOR3 vPos = pPlayer->GetFoot();
		m_pObjectMgr->AddObject(OBJECT_EFFECT, CBoom::Create(vPos));
		m_pObjectMgr->AddObject(OBJECT_EFFECT, CBoom::Create(D3DXVECTOR3(vPos.x, vPos.y - 150.f, 0.f)));
		m_pObjectMgr->AddObject(OBJECT_EFFECT, CBoom::Create(D3DXVECTOR3(vPos.x, vPos.y + 150.f, 0.f)));
		m_pObjectMgr->AddObject(OBJECT_EFFECT, CBoom::Create(D3DXVECTOR3(vPos.x - 300.f, vPos.y, 0.f)));
		m_pObjectMgr->AddObject(OBJECT_EFFECT, CBoom::Create(D3DXVECTOR3(vPos.x + 300.f, vPos.y, 0.f)));

	}
	else if (m_bAddHitBox && (16 == iFrame || 45 == iFrame || 76 == iFrame || 106 == iFrame || 136 == iFrame))
		m_bAddHitBox = false;

}

void CAlecto::WhipWhirlUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_bOnceAnimFinish = false;
		m_eCurPattern = CAlecto::PATTERN_LUNGE;
		m_eCurState = CAlecto::STATE_IDLE;
		return;
	}

	int iFrame = static_cast<int>(m_tFrame.fCurFrame);

	if (!m_bAddHitBox && 30 == iFrame)
	{
		m_bAddHitBox = true;
		
		CNormalHitBox* pHitBox = CNormalHitBox::Create(m_tInfo.vPos, D3DXVECTOR3(300.f, 150.f, 0.f), this, 0.5f, m_tStatus.iAttack);
		m_pObjectMgr->AddObject(OBJECT_MONSTER_HITBOX, pHitBox);

		CNormalEffect* pEffect = CNormalEffect::Create(CAnimImp::Create(L"Fx", L"FuryWhipWhirl", 35.f), m_tInfo.vPos, RENDER_FRONT_OBJECT);
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
		CSoundMgr::GetInstance()->PlaySound(L"ArrowImpactSplatter.wav", CSoundMgr::EFFECT);
	}
	else if (m_bAddHitBox && 31 == iFrame)
		m_bAddHitBox = false;
}

void CAlecto::DieUpdate()
{
	CScreenEffect* pEffect = CScreenEffect::Create(CNonAnimImp::Create(L"BlackScreen", 3.5f, D3DCOLOR_ARGB(255, 255, 255, 255)),
		D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI, 1.f);
	pEffect->SetScale(10.f);
	if (pEffect)
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

	FRAME tFrame;
	tFrame.eType = ANIM_ONCE;
	tFrame.fMaxFrame = m_pTextureMgr->GetTextureMaxFrame(L"Fx", L"Enemy_Fury2_Dead");
	tFrame.fCurFrame = 0.f;
	tFrame.fFrameTime = 0.f;
	tFrame.fFrameSpeed = 1 / 30.f;

	pEffect = CScreenEffect::Create(CAnimImp::Create(L"Fx", L"Enemy_Fury2_Dead", tFrame, D3DCOLOR_ARGB(255, 255, 255, 255)), D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI, 2.f);
	if (pEffect)
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

	tFrame.eType = ANIM_ONCE;
	tFrame.fMaxFrame = m_pTextureMgr->GetTextureMaxFrame(L"Fx", L"ZagreusDeathFlash");
	tFrame.fCurFrame = 0.f;
	tFrame.fFrameTime = 0.f;
	tFrame.fFrameSpeed = 1 / 30.f;

	pEffect = CScreenEffect::Create(CAnimImp::Create(L"Fx", L"ZagreusDeathFlash", tFrame, D3DCOLOR_ARGB(255, 255, 255, 255)), D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI, 3.f);
	if (pEffect)
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

	CMathMgr::AddTitleEffect(L"FuryVanquished", 4.f);

	//CSoundMgr::GetInstance()->PlaySound(L"Alecto_EmoteDying_1.wav", CSoundMgr::EFFECT);
	m_pObjectMgr->ObjectListClear(OBJECT_EFFECT);
	CSoundMgr::GetInstance()->StopAll();
	CSoundMgr::GetInstance()->PlaySound(L"Alecto_Why you.wav", CSoundMgr::EFFECT);
	CSoundMgr::GetInstance()->PlaySound(L"StabSplatter2.wav", CSoundMgr::EFFECT);
	CSoundMgr::GetInstance()->PlaySound(L"HadesThemeStinger.wav", CSoundMgr::EFFECT);
}

int CAlecto::GetDir()
{
	switch (m_eCurState)
	{
	case CAlecto::STATE_IDLE:
		return CMathMgr::AngleTo8Dir(D3DXVECTOR3(1.f, 0.f, 0.f), m_tInfo.vDir);
	case CAlecto::STATE_RUN:
		return CMathMgr::AngleTo16Dir(D3DXVECTOR3(1.f, 0.f, 0.f), m_tInfo.vDir);
	case CAlecto::STATE_LUNGE:
		return CMathMgr::AngleTo16Dir(D3DXVECTOR3(1.f, 0.f, 0.f), m_tInfo.vDir);
		break;
	case CAlecto::STATE_BOOM:
	case CAlecto::STATE_WHIP_WHIRL:
		return 0;
	}

	return 0;
}
