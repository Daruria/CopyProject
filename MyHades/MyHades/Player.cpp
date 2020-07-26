#include "stdafx.h"
#include "Player.h"
#include "Mouse.h"
#include "Effect.h"
#include "HitBox.h"
#include "NormalHitBox.h"
#include "LineHitBox.h"
#include "FadeInOutImp.h"
#include "ScreenEffect.h"
#include "Subject.h"
#include "PlayerUI.h"
#include "PlayerObserver.h"
#include "NonAnimImp.h"
#include "AnimImp.h"
#include "FontImp.h"
#include "NormalEffect.h"
#include "MoveEffect.h"
#include "Monster.h"
#include "Terrain.h"

CPlayer::CPlayer() :
	m_wstrObjectKey(L""),
	m_ePreState(CPlayer::END),
	m_eCurState(CPlayer::END),
	m_fSpeed(0.f),
	m_bCanKeyInput(true),
	m_bIsEffect(false),
	m_bCanAttack(true),
	m_iAttackCount(0),
	m_fDashSpeed(0.f),
	m_bCanHit(true),
	m_fHitCoolTime(0.f),
	m_fHitCoolDown(0.f),
	m_bNoDie(false),
	m_bDeadEffect(false),
	m_bRender(true),
	m_bCanDashSkill(false),
	m_bCanReflectSkill(false),
	m_bCanCall(false),
	m_bCalling(false),
	m_iWrathPoint(0),
	m_iWrathMaxPoint(0),
	m_fFootStepTimeCount(0.f),
	m_fFootStepTime(0.f),
	m_bAutoPlay(false),
	m_eAutoState(AUTO_END),
	m_bAttackInput(false),
	m_bDashInput(false),
	m_bDashAttackInput(false),
	m_bParryInput(false),
	m_bCallInput(false),
	m_bAstarMoving(false),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance())
{
	ZeroMemory(&m_tStatus, sizeof(STATUS));
}


CPlayer::~CPlayer()
{
	Release();
}

int CPlayer::Update()
{
	if (!m_bNoDie && !m_bDeadEffect && m_tStatus.iHp <= 0)
	{
		m_bDeadEffect = true;

		DeadEffect();

		m_eCurState = CPlayer::DEAD_WALK;

		m_bCanColl = false;

		CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
		CSoundMgr::GetInstance()->PlaySound(L"PlayerDeath.wav", CSoundMgr::EFFECT);
		CSoundMgr::GetInstance()->PlaySound(L"PlayerDeathBloodComponents.wav", CSoundMgr::EFFECT);

		m_pObjectMgr->ObjectListClear(OBJECT_MONSTER);

		return NO_EVENT;
	}

	if (m_bDeadEffect)
	{
		//	임시


		return NO_EVENT;
	}

	WrathFill();

	HitCoolDown();

	if (m_pKeyMgr->KeyDown(KEY_F4))
	{
		m_bAutoPlay = !m_bAutoPlay;
		AutoEffect();
	}
	
	if (!m_bAutoPlay)
		KeyInput();
	else
		AutoKeyInput();

	Calling();

	ChangeState();

	StateUpdate();

	Animation();

	if (m_bRender)
		m_pObjectMgr->AddLayer(m_eRenderType, this);

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	D3DXMATRIX matScale, matRotateZ, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXVECTOR3 vScrolledPos = m_tInfo.vPos + m_vecRenderOffset[m_eCurState] - CScrollMgr::GetScrollPos();
	MatTranslation(&matTrans, vScrolledPos);

	m_tInfo.matWorld = matScale * matTrans;

}

void CPlayer::Render()
{
	int iCurFrame = static_cast<int>(m_tFrame.fCurFrame + AngleToDir() * m_vecStateMaxFrame[m_eCurState]);
	const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_vecStateKey[m_eCurState], iCurFrame);

	NULL_CHECK(pTexInfo);
	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width *0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CPlayer::DASH == m_eCurState)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, L"ZagreusDashVFX", iCurFrame);
		NULL_CHECK(pTexInfo);
		D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width *0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

#ifdef _DEBUG
	m_pDeviceMgr->GetSprite()->End();

	D3DXVECTOR3 vPos = m_tInfo.vPos + m_tHitBox.vOffset - CScrollMgr::GetScrollPos();

	D3DXVECTOR2 vPoint[5]
	{
		D3DXVECTOR2(vPos.x - m_tHitBox.vSize.x * 0.5f,vPos.y - m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x + m_tHitBox.vSize.x * 0.5f,vPos.y - m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x + m_tHitBox.vSize.x * 0.5f,vPos.y + m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x - m_tHitBox.vSize.x * 0.5f,vPos.y + m_tHitBox.vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x - m_tHitBox.vSize.x * 0.5f,vPos.y - m_tHitBox.vSize.y * 0.5f)
	};

	m_pDeviceMgr->GetLine()->SetWidth(2.f); // 선의 굵기
	m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	D3DXVECTOR2 vPos2 = m_tInfo.vPos + m_vFootPos - CScrollMgr::GetScrollPos();
	vPoint[0] = vPos2 + D3DXVECTOR2(-5.f, -5.f);
	vPoint[1] = vPos2 + D3DXVECTOR2(5.f, -5.f);
	vPoint[2] = vPos2 + D3DXVECTOR2(5.f, 5.f);
	vPoint[3] = vPos2 + D3DXVECTOR2(-5.f, 5.f);
	vPoint[4] = vPos2 + D3DXVECTOR2(-5.f, -5.f);

	m_pDeviceMgr->GetLine()->SetWidth(2.f); // 선의 굵기
	m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	//	Player Info 
	wstring wstrText = L"";
	D3DXMATRIX matTrans;

	wstrText = L"Player X : " + to_wstring(m_tInfo.vPos.x);
	D3DXMatrixTranslation(&matTrans, 10.f, 50.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(), wstrText.c_str(), wstrText.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 255));

	wstrText = L"Player Y : " + to_wstring(m_tInfo.vPos.y);
	D3DXMatrixTranslation(&matTrans, 10.f, 90.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(), wstrText.c_str(), wstrText.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 255));

	wstrText = L"Current Frame : " + to_wstring(m_tFrame.fCurFrame);
	D3DXMatrixTranslation(&matTrans, 10.f, 130.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(), wstrText.c_str(), wstrText.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 255));

#endif // _DEBUG

}

HRESULT CPlayer::Initialize()
{
	m_eRenderType = RENDER_OBJECT;

	m_tInfo.vPos = { 4650.f, 1350.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 0.f };

	m_tHitBox.vOffset = { 0.f, 10.f, 0.f };
	m_tHitBox.vSize = { 50.f, 100.f, 0.f };

	m_vFootPos = { 0.f, 50.f, 0.f };

	m_tStatus.iHp = 50;
	m_tStatus.iMaxHp = 50;
	m_tStatus.iAttack = 20;

	m_iWrathMaxPoint = 100;
	m_iWrathPoint = 0;

	m_wstrObjectKey = L"Zagreus";

	m_vecStateKey.resize(CPlayer::END);
	m_vecDirCnt.resize(CPlayer::END);
	m_vecStateMaxFrame.resize(CPlayer::END);
	m_vecRenderOffset.resize(CPlayer::END);

	m_vecStateKey[CPlayer::IDLE] = L"ZagreusIdle";
	m_vecDirCnt[CPlayer::IDLE] = 8;
	m_vecStateMaxFrame[CPlayer::IDLE] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusIdle") / m_vecDirCnt[CPlayer::IDLE];
	m_vecRenderOffset[CPlayer::IDLE] = { 0.f, 0.f, 0.f };

	m_vecStateKey[CPlayer::RUN] = L"ZagreusRun";//L"ZagreusSwordRun";
	m_vecDirCnt[CPlayer::RUN] = 8;
	m_vecStateMaxFrame[CPlayer::RUN] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusRun"/*L"ZagreusSwordRun"*/) / m_vecDirCnt[CPlayer::RUN];
	m_vecRenderOffset[CPlayer::RUN] = { 0.f, 0.f, 0.f };

	m_vecStateKey[CPlayer::DASH] = L"ZagreusDash";
	m_vecDirCnt[CPlayer::DASH] = 8;
	m_vecStateMaxFrame[CPlayer::DASH] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusDash") / m_vecDirCnt[CPlayer::DASH];
	m_vecRenderOffset[CPlayer::DASH] = { 0.f, 0.f, 0.f };

	m_vecStateKey[CPlayer::ATTACK] = L"ZagreusSword";
	m_vecDirCnt[CPlayer::ATTACK] = 16;
	m_vecStateMaxFrame[CPlayer::ATTACK] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusSword") / m_vecDirCnt[CPlayer::ATTACK];
	m_vecRenderOffset[CPlayer::ATTACK] = { 0.f, -30.f, 0.f };

	m_vecStateKey[CPlayer::RETURN_TO_IDLE] = L"ZagreusSwordReturnToIdle";
	m_vecDirCnt[CPlayer::RETURN_TO_IDLE] = 16;
	m_vecStateMaxFrame[CPlayer::RETURN_TO_IDLE] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusSwordReturnToIdle") / m_vecDirCnt[CPlayer::RETURN_TO_IDLE];
	m_vecRenderOffset[CPlayer::RETURN_TO_IDLE] = { 0.f, -30.f, 0.f };

	m_vecStateKey[CPlayer::PARRY] = L"ZagreusSwordParry";
	m_vecDirCnt[CPlayer::PARRY] = 16;
	m_vecStateMaxFrame[CPlayer::PARRY] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusSwordParry") / m_vecDirCnt[CPlayer::PARRY];
	m_vecRenderOffset[CPlayer::PARRY] = { 0.f, -30.f, 0.f };

	m_vecStateKey[CPlayer::DASH_ATTACK] = L"ZagreusSwordDashAttack";
	m_vecDirCnt[CPlayer::DASH_ATTACK] = 8;
	m_vecStateMaxFrame[CPlayer::DASH_ATTACK] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusSwordDashAttack") / m_vecDirCnt[CPlayer::DASH_ATTACK];
	m_vecRenderOffset[CPlayer::DASH_ATTACK] = { 0.f, 0.f, 0.f };

	m_vecStateKey[CPlayer::DEAD_WALK] = L"ZagreusDeadWalk";
	m_vecDirCnt[CPlayer::DEAD_WALK] = 1;
	m_vecStateMaxFrame[CPlayer::DEAD_WALK] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusDeadWalk") / m_vecDirCnt[CPlayer::DEAD_WALK];
	m_vecRenderOffset[CPlayer::DEAD_WALK] = { 0.f, 0.f, 0.f };

	m_vecStateKey[CPlayer::ON_HIT] = L"ZagreusOnHit";
	m_vecDirCnt[CPlayer::ON_HIT] = 8;
	m_vecStateMaxFrame[CPlayer::ON_HIT] = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusOnHit") / m_vecDirCnt[CPlayer::ON_HIT];
	m_vecRenderOffset[CPlayer::ON_HIT] = { 0.f, 0.f, 0.f };

	m_eCurState = CPlayer::IDLE;

	m_fSpeed = 500.f;

	m_fDashSpeed = 1000.f;

	m_fHitCoolTime = 1.f;

	m_pSubjectMgr->AddSubject(L"Player", CSubject::Create());
	m_pObjectMgr->AddObject(OBJECT_PLAYER_UI, CPlayerUI::Create());
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::MAX_HP, &m_tStatus.iMaxHp);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::HP, &m_tStatus.iHp);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::ATK, &m_tStatus.iAttack);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::WRATH_POINT, &m_iWrathPoint);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::WRATH_MAX_POINT, &m_iWrathMaxPoint);

	m_fFootStepTime = 0.2f;

	return S_OK;
}

void CPlayer::Release()
{
}

void CPlayer::Die()
{
	m_bNoDie = false;
	m_tStatus.iHp = 0;
}

void CPlayer::Animation()
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

void CPlayer::Hit(const D3DXVECTOR3 & vPos, int iAttack)
{
	if (m_bCanHit)
	{
		m_bCanHit = false;
		m_fHitCoolDown = 0.f;
		m_tStatus.iHp -= iAttack;
		if (m_tStatus.iHp < 0)
			m_tStatus.iHp = 0;

		CFadeInOutImp* pImp = CFadeInOutImp::Create(L"BloodFrame", 0.f, 0.f, 0.5f, D3DCOLOR_ARGB(0, 255, 255, 255));

		CScreenEffect* pEffect = CScreenEffect::Create(pImp, D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_UI, 0.f);
		NULL_CHECK(pEffect);
		m_pObjectMgr->AddObject(OBJECT_UI, pEffect);

		m_pSubjectMgr->Notify(L"Player", CPlayerObserver::HP, &m_tStatus.iHp);

		Bleeding(vPos);

		m_eCurState = CPlayer::ON_HIT;
		m_bAstarMoving = false;

		if (m_bCanReflectSkill)
		{
			CAnimImp* pImp = CAnimImp::Create(L"Fx", L"LightningBolt", 16);
			CNormalEffect* pEffect = CNormalEffect::Create(pImp, vPos + D3DXVECTOR3(0.f, -75.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), RENDER_UI);
			m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

			pImp = CAnimImp::Create(L"Fx", L"ZeusLightningStrikeGroundNova", 10);
			pEffect = CNormalEffect::Create(pImp, vPos + m_vFootPos, D3DXVECTOR3(1.f, 1.f, 0.f), RENDER_OBJECT);
			m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

			CNormalHitBox* pHitBox = CNormalHitBox::Create(vPos + m_vFootPos, D3DXVECTOR3(200.f, 150.f, 0.f), pEffect, 0.2f, m_tStatus.iAttack);
			m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, pHitBox);

			CSoundMgr::GetInstance()->PlaySound(L"ZeusLightningImpact_1.wav", CSoundMgr::EFFECT);
		}
	}
}

void CPlayer::Reset()
{
	m_tStatus.iHp = 50;
	m_tStatus.iMaxHp = 50;
	m_tStatus.iAttack = 20;
	//m_eCurState = CPlayer::IDLE;
	m_tInfo.vLook = { 1.f, -1.f, 0.f };
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);
	m_bCanKeyInput = true;

	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::HP, &m_tStatus.iHp);

	m_bDeadEffect = false;

	m_bCanDashSkill = false;
	m_bCanReflectSkill = false;
	m_bCanCall = false;

	m_iWrathPoint = 0;
	m_iWrathMaxPoint = 100;

	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::WRATH_POINT, &m_iWrathPoint);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::WRATH_MAX_POINT, &m_iWrathMaxPoint);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::DASH_SKILL, &m_bCanDashSkill);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::REFLECT_SKILL, &m_bCanReflectSkill);
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::CALL, &m_bCanCall);
}

void CPlayer::SetCanDashSkill(bool bDashSkill)
{
	m_bCanDashSkill = bDashSkill;
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::DASH_SKILL, &m_bCanDashSkill);
}

void CPlayer::SetCanReflectSkill(bool bReflectSkill)
{
	m_bCanReflectSkill = bReflectSkill;
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::REFLECT_SKILL, &m_bCanReflectSkill);
}

void CPlayer::SetCanCall(bool bCanCall)
{
	m_bCanCall = bCanCall;
	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::CALL, &m_bCanCall);
}

void CPlayer::KeyInput()
{
	if (m_pKeyMgr->KeyDown(KEY_F8))
	{
		m_bNoDie = !m_bNoDie;
		wstring wstrText = m_bNoDie ? L"No Die" : L"Can Die";
		D3DXVECTOR3 vSize = { 0.5f, 0.5f, 0.f };
		CFontImp* pImp = CFontImp::Create(wstrText, vSize, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));

		CMoveEffect* pEffect = CMoveEffect::Create(pImp, m_tInfo.vPos, D3DXVECTOR3(0.f, -1.f, 0.f), 100.f, RENDER_FRONT_UI);
		pEffect->SetScale(0.5f);

		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
	}

	if (!m_bCanKeyInput)
		return;

	switch (m_eCurState)
	{
	case CPlayer::IDLE:
	case CPlayer::RUN:
		//	Move
		KeyInputMove();
		//  Attack 
		KeyInputAttack();
		//	Dash
		KeyInputDash();
		//	Parry
		KeyInputParry();
		//	Call
		KeyInputCall();
		break;
	case CPlayer::DASH:
		if (m_tFrame.fCurFrame > 8.f)
		{
			KeyInputMove();
			KeyInputAttack();
			KeyInputParry();
			KeyInputCall();
		}
		if (m_tFrame.fCurFrame <= 8.f)
			KeyInputDashAttack();

		break;
	case CPlayer::ATTACK:
		if (m_tFrame.fCurFrame > 35.f)
		{
			KeyInputAttack();
			KeyInputMove();
			KeyInputDash();
		}
		break;
	case CPlayer::DASH_ATTACK:
		if (m_tFrame.fCurFrame > 10.f)
		{
			KeyInputMove();
			KeyInputAttack();
			KeyInputDash();
		}
		break;
	case CPlayer::PARRY:
		if (m_tFrame.fCurFrame > 13.f)
		{
			KeyInputMove();
			KeyInputAttack();
			KeyInputDash();
		}
		break;
	case CPlayer::RETURN_TO_IDLE:
	case CPlayer::ON_HIT:
		KeyInputMove();
		KeyInputAttack();
		KeyInputDash();
		KeyInputParry();
		break;
	case CPlayer::DEAD_WALK:

		break;
	}

}

void CPlayer::KeyInputMove()
{
	if (m_pKeyMgr->KeyDown(KEY_RBUTTON))
	{
		if (m_pAstarMgr->AstarProceeding())
			return;
		
		m_pAstarMgr->StartAstar(m_tInfo.vPos + m_vFootPos, CMouse::GetInstance()->GetMousePos() + CScrollMgr::GetScrollPos());
		m_eCurState = RUN;
		m_bAstarMoving = true;
	}

	if (m_bAstarMoving)
		return;

	if (m_eCurState == CPlayer::RUN)
		m_eCurState = CPlayer::IDLE;
	D3DXVECTOR3 vKeyLook = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vMove = { 0.f, 0.f, 0.f };
	if (m_pKeyMgr->KeyPressing(KEY_W))
	{
		vKeyLook.y -= 1.f;
		vMove.y -= static_cast<float>(TILECY);
		m_eCurState = CPlayer::RUN;
	}
	if (m_pKeyMgr->KeyPressing(KEY_S))
	{
		vKeyLook.y += 1.f;
		vMove.y += static_cast<float>(TILECY);
		m_eCurState = CPlayer::RUN;
	}

	if (m_pKeyMgr->KeyPressing(KEY_A))
	{
		vKeyLook.x -= 1.f;
		vMove.x -= static_cast<float>(TILECX);
		m_eCurState = CPlayer::RUN;
	}
	if (m_pKeyMgr->KeyPressing(KEY_D))
	{
		vKeyLook.x += 1.f;
		vMove.x += static_cast<float>(TILECX);
		m_eCurState = CPlayer::RUN;
	}

	if (0.f != vKeyLook.x || 0.f != vKeyLook.y)
	{
		m_iAttackCount = 0;
		m_bCanAttack = true;
		D3DXVec3Normalize(&vKeyLook, &vKeyLook);
		m_tInfo.vLook = vKeyLook;
	}

	if (0.f != vMove.x || 0.f != vMove.y)
	{
		m_iAttackCount = 0;
		m_bCanAttack = true;
		D3DXVec3Normalize(&vMove, &vMove);
		m_tInfo.vDir = vMove;
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();
	}
}

void CPlayer::KeyInputAttack()
{
	if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
		m_bAttackInput = true;

	if (m_bCanAttack && m_bAttackInput)
	{
		D3DXVECTOR3 vClickPos = CMouse::GetInstance()->GetMousePos() + CScrollMgr::GetScrollPos();

		AttackAction(vClickPos);
	}
	
	m_bAttackInput = false;
}

void CPlayer::KeyInputDash()
{
	if (m_pKeyMgr->KeyDown(KEY_SPACE))
		m_bDashInput = true;

	if (m_bDashInput)
		DashAction();

	m_bDashInput = false;
}

void CPlayer::KeyInputDashAttack()
{
	if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		DashAttackAction();
	}
}

void CPlayer::KeyInputParry()
{
	if (m_pKeyMgr->KeyDown(KEY_Q))
	{
		ParryAction();
	}
}

void CPlayer::KeyInputCall()
{
	if (m_bCanCall)
	{
		if (!m_bCalling && m_iWrathPoint / 25 >= 1 && m_pKeyMgr->KeyDown(KEY_F))
		{
			m_bCalling = true;
			CallEffect();

		}
	}
}

void CPlayer::AutoKeyInput()
{
	list<CGameObject*>* pMonsterList = m_pObjectMgr->GetObjectList(OBJECT_MONSTER);

	if (pMonsterList->empty())
	{
		m_bAutoPlay = false;
		AutoEffect();
		return;
	}

	float fDist = INFINITE;
	float fMonDist = 0;
	
	CMonster* pNearest = nullptr;
	D3DXVECTOR3 vDist, vNearestDir;
	for (auto pMonster : *pMonsterList)
	{
		vDist = pMonster->GetInfo().vPos - m_tInfo.vPos;

		fMonDist = D3DXVec3Length(&vDist);

		if (fDist > fMonDist)
		{
			fDist = fMonDist;
			pNearest = dynamic_cast<CMonster*>(pMonster);
			vNearestDir = vDist;
		}
	}

	//if (fDist > 500.f)	//	Dash
	//{
	//	m_eAutoState = CPlayer::A_DASH;
	//	m_bAstarMoving = false;
	//}
	//else 
	if (fDist > 200.f)//	Run
	{
		m_eAutoState = CPlayer::A_RUN;
	}
	else//	Attack or Calling or DashAttack or Parry
	{
		m_bAstarMoving = false;
		int iRandom = rand() % 10;
		switch (iRandom)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:case 8:case 9:
			m_eAutoState = CPlayer::A_ATTACK;
			break;
		case 5:
			m_eAutoState = CPlayer::A_DASH;
			break;
		case 6:
			m_eAutoState = CPlayer::A_PARRY;
			break;
		case 7:
			m_eAutoState = CPlayer::A_CALL;
			break;
		}
		if (CPlayer::A_CALL == m_eAutoState)
		{
			if (!m_bCanCall || m_iWrathPoint < 25)
				m_eAutoState = CPlayer::A_ATTACK;
		}
		
			
	}
	

	switch (m_eCurState)
	{
	case CPlayer::IDLE:
	case CPlayer::RUN:
		//AutoMove(fDist, vNearestDir);
		AutoMoveAstar(pNearest->GetInfo().vPos);
		AutoAttack(pNearest->GetInfo().vPos);
		AutoDash(pNearest->GetInfo().vPos);
		AutoParry(fDist);
		AutoCall(fDist);
		break;
	case CPlayer::DASH:
		if (m_tFrame.fCurFrame > 8.f)
		{
			//AutoMove(fDist, vNearestDir);
			AutoMoveAstar(pNearest->GetInfo().vPos);
			AutoAttack(pNearest->GetInfo().vPos);
			AutoParry(fDist);
			AutoCall(fDist);
		}
		if (m_tFrame.fCurFrame <= 8.f)
			AutoDashAttack(fDist);

		break;
	case CPlayer::ATTACK:
		if (m_tFrame.fCurFrame > 35.f)
		{
			AutoAttack(pNearest->GetInfo().vPos);
			//AutoMove(fDist, vNearestDir);
			AutoMoveAstar(pNearest->GetInfo().vPos);
			AutoDash(pNearest->GetInfo().vPos);
		}
		break;
	case CPlayer::DASH_ATTACK:
		if (m_tFrame.fCurFrame > 10.f)
		{
			//AutoMove(fDist, vNearestDir);
			AutoMoveAstar(pNearest->GetInfo().vPos);
			AutoAttack(pNearest->GetInfo().vPos);
			AutoDash(pNearest->GetInfo().vPos);
		}
		break;
	case CPlayer::PARRY:
		if (m_tFrame.fCurFrame > 13.f)
		{
			//AutoMove(fDist, vNearestDir);
			AutoMoveAstar(pNearest->GetInfo().vPos);
			AutoAttack(pNearest->GetInfo().vPos);
			AutoDash(pNearest->GetInfo().vPos);
		}
		break;
	case CPlayer::RETURN_TO_IDLE:
	case CPlayer::ON_HIT:
		//AutoMove(fDist, vNearestDir);
		AutoMoveAstar(pNearest->GetInfo().vPos);
		AutoAttack(pNearest->GetInfo().vPos);
		AutoDash(pNearest->GetInfo().vPos);
		AutoParry(fDist);
		break;
	case CPlayer::DEAD_WALK:

		break;
	}



}

void CPlayer::RunAction()
{

}

void CPlayer::AttackAction(const D3DXVECTOR3& vClickPos)
{
	if (m_iAttackCount > 3)
		return;

	m_iAttackCount++;
	m_bCanAttack = false;


	m_tInfo.vLook = vClickPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	float fRadian = RadianForAngle3(AngleToDir());
	D3DXMATRIX matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, fRadian);

	D3DXVec3TransformCoord(&m_tInfo.vDir, &D3DXVECTOR3(1.f, 0.f, 0.f), &matRotateZ);

	m_eCurState = CPlayer::ATTACK;

	CSoundMgr::GetInstance()->PlaySound(L"ZagreusEmoteAttackingSword_1.wav", CSoundMgr::EFFECT);
	CSoundMgr::GetInstance()->PlaySound(L"ZagreusSwordSwipe.wav", CSoundMgr::EFFECT);
}

void CPlayer::DashAction()
{
	m_iAttackCount = 0;

	m_bCanAttack = true;
	m_eCurState = CPlayer::DASH;

	CSoundMgr::GetInstance()->PlaySound(L"ZagreusDash.wav", CSoundMgr::EFFECT);

	if (m_bCanDashSkill)
	{
		CAnimImp* pImp = CAnimImp::Create(L"Fx", L"LightningBolt", 16);
		CNormalEffect* pEffect = CNormalEffect::Create(pImp, m_tInfo.vPos + D3DXVECTOR3(0.f, -75.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), RENDER_UI);
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

		pImp = CAnimImp::Create(L"Fx", L"ZeusLightningStrikeGroundNova", 10);
		pEffect = CNormalEffect::Create(pImp, m_tInfo.vPos + m_vFootPos, D3DXVECTOR3(1.f, 1.f, 0.f), RENDER_OBJECT);
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

		CNormalHitBox* pHitBox = CNormalHitBox::Create(m_tInfo.vPos + m_vFootPos, D3DXVECTOR3(200.f, 150.f, 0.f), pEffect, 0.2f, m_tStatus.iAttack);
		m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, pHitBox);


		CSoundMgr::GetInstance()->PlaySound(L"ZeusLightningImpact_1.wav", CSoundMgr::EFFECT);
	}
}

void CPlayer::DashAttackAction()
{
	m_eCurState = CPlayer::DASH_ATTACK;
	m_iAttackCount = 4;

	D3DXVECTOR3 vPos = m_tInfo.vPos + m_tInfo.vDir * 100.f;
	vPos.y += 30.f;

	D3DXVECTOR3 vStd = { 1.f, 0.f, 0.f };
	float fDot = D3DXVec3Dot(&vStd, &m_tInfo.vDir);
	float fRadian = acosf(fDot);
	fDot = D3DXVec3Dot(&D3DXVECTOR3(0.f, 1.f, 0.f), &m_tInfo.vDir);
	if (fDot < 0)
		fRadian = -fRadian;

	CAnimImp* pImp = CAnimImp::Create(L"Fx", L"Backstab", 16);
	CNormalEffect* pEffect = CNormalEffect::Create(pImp, vPos, D3DXVECTOR3(2.f, 2.f, 0.f), fRadian, RENDER_UI);

	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

	CNormalHitBox* pHitBox = CNormalHitBox::Create(vPos + m_tInfo.vDir * 50.f, D3DXVECTOR3(150.f, 150.f, 0.f), this, 0.2f, m_tStatus.iAttack);
	m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, pHitBox);

	CSoundMgr::GetInstance()->PlaySound(L"ZagreusSwordSwipe.wav", CSoundMgr::EFFECT);
	CSoundMgr::GetInstance()->PlaySound(L"ZagreusEmoteAttackingSword_3.wav", CSoundMgr::EFFECT);

}

void CPlayer::ParryAction()
{
	m_bCanAttack = true;
	m_eCurState = CPlayer::PARRY;

	CSoundMgr::GetInstance()->PlaySound(L"ZagreusEmoteParrying_3.wav", CSoundMgr::EFFECT);
}

void CPlayer::CallAction()
{
	if (!m_bCalling)
	{
		m_bCalling = true;
	}
}

void CPlayer::AutoAttack(const D3DXVECTOR3& vNearest)
{
	if (CPlayer::A_ATTACK == m_eAutoState)
		AttackAction(vNearest);
}

void CPlayer::AutoMove(float fDist, const D3DXVECTOR3& vNearestDir)
{
	//	Run
	if (CPlayer::A_RUN == m_eAutoState)
	{
		m_eCurState = CPlayer::RUN;
		m_iAttackCount = 0;
		m_bCanAttack = true;

		D3DXVec3Normalize(&m_tInfo.vDir, &vNearestDir);
		D3DXVec3Normalize(&m_tInfo.vLook, &vNearestDir);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();
	}
}

void CPlayer::AutoMoveAstar(const D3DXVECTOR3 & vNearest)
{
	if (CPlayer::A_RUN == m_eAutoState)
	{
		m_eCurState = CPlayer::RUN;
		m_iAttackCount = 0;
		m_bCanAttack = true;

		if (!m_bAstarMoving && !m_pAstarMgr->AstarProceeding())
		{
			D3DXVECTOR3 vNear = AstarNearPos(vNearest);

			m_pAstarMgr->StartAstar(m_tInfo.vPos + m_vFootPos, vNear);
			m_bAstarMoving = true;

			if (CThreadMgr::GetInstance()->GetAstarThreading())
			{
				m_bAstarMoving = false;
				m_eAutoState = CPlayer::A_DASH;
				return;
			}

			bool bAstarThread = CThreadMgr::GetInstance()->AstarThread(m_tInfo.vPos + m_vFootPos, vNear);
			if (!bAstarThread)
			{
				//::MessageBoxA(g_hWnd, "AstarThread Fail", "Thread Fail", MB_OK);
				m_bAstarMoving = false;
				m_eAutoState = CPlayer::A_DASH;
			}
			else
				m_bAstarMoving = true;
		}
		//else
		//{
		//	m_bAstarMoving = false;
		//	m_eAutoState = CPlayer::A_DASH;
		//}
	}
}

void CPlayer::AutoDash(const D3DXVECTOR3& vNearest)
{
	if (CPlayer::A_DASH == m_eAutoState)
	{
		D3DXVECTOR3 vDir = vNearest - m_tInfo.vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		float fDot = D3DXVec3Dot(&vDir, &D3DXVECTOR3(1.f, 0.f, 0.f));
		float fRadian = acosf(fDot);
		if (D3DXVec3Dot(&vDir, &D3DXVECTOR3(0.f, 1.f, 0.f)) < 0.f)
			fRadian = -fRadian;

		int iDir = static_cast<int>(fRadian / (PI * 0.25f));

		fRadian = iDir * PI * 0.25f;

		D3DXMATRIX matRotateZ;
		D3DXMatrixRotationZ(&matRotateZ, fRadian);
		D3DXVec3TransformCoord(&m_tInfo.vLook, &D3DXVECTOR3(1.f, 0.f, 0.f), &matRotateZ);
		D3DXVec3TransformCoord(&m_tInfo.vDir, &D3DXVECTOR3(1.f, 0.f, 0.f), &matRotateZ);

		DashAction();
	}
}

void CPlayer::AutoDashAttack(float fDist)
{
	if (CPlayer::A_ATTACK == m_eAutoState)
		DashAttackAction();
} 

void CPlayer::AutoParry(float fDist)
{
	if (CPlayer::A_PARRY == m_eAutoState)
		ParryAction();
}

void CPlayer::AutoCall(float fDist)
{
	if (CPlayer::A_CALL == m_eAutoState)
	{
		CallEffect();
		CallAction();
	}
}

const D3DXVECTOR3 CPlayer::AstarNearPos(const D3DXVECTOR3 & vPos)
{
	CTerrain* pTerrain = m_pObjectMgr->GetTerrain();
	NULL_CHECK_RETURN(pTerrain, vPos);
	vector<TILE_INFO*>* pvecTile = pTerrain->GetVecTile();

	if (0 == (*pvecTile)[pTerrain->GetTileIndex(vPos)]->byOption)
		return vPos;

	D3DXVECTOR3 vDiffer[8] = {
		D3DXVECTOR3(-TILECX * 0.5f, -TILECY * 0.5f, 0.f),
		D3DXVECTOR3(TILECX * 0.5f, -TILECY * 0.5f, 0.f),
		D3DXVECTOR3(TILECX * 0.5f, TILECY * 0.5f, 0.f),
		D3DXVECTOR3(-TILECX * 0.5f, TILECY * 0.5f, 0.f),
		D3DXVECTOR3(-TILECX, 0.f, 0.f),
		D3DXVECTOR3(TILECX, 0.f, 0.f),
		D3DXVECTOR3(0.f, -TILECY, 0.f),
		D3DXVECTOR3(0.f, TILECY, 0.f)
	};

	for (auto vOffset : vDiffer)
	{
		if (0 == (*pvecTile)[pTerrain->GetTileIndex(vPos + vOffset)]->byOption)
			return vPos + vOffset;
	}
	
	return vPos;
}

void CPlayer::StateUpdate()
{
	switch (m_eCurState)
	{
	case CPlayer::IDLE:
		break;
	case CPlayer::RUN:
		RunUpdate();
		break;
	case CPlayer::DASH:
		DashUpdate();
		break;
	case CPlayer::ATTACK:
		AttackUpdate();
		break;
	case CPlayer::DASH_ATTACK:
		DashAttackUpdate();
		break;
	case CPlayer::PARRY:
		ParryUpdate();
		break;
	case CPlayer::RETURN_TO_IDLE:
		break;
	case CPlayer::ON_HIT:
		OnHitUpdate();
		break;
	case CPlayer::DEAD_WALK:
		DeadUpdate();
		break;
	}
}

void CPlayer::AttackUpdate()
{
	switch (m_iAttackCount)
	{
	case 1:
		if (m_tFrame.fCurFrame >= 8.f)
			m_bCanAttack = true;
		break;
	case 2:
		if (m_tFrame.fCurFrame >= 20.f)
			m_bCanAttack = true;
		break;
	}

	switch (m_iAttackCount)
	{
	case 1:
		if (!m_bIsEffect && 6.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 7.f)
		{
			m_bIsEffect = true;

			INFO tInfo = m_tInfo;
			tInfo.vPos += tInfo.vLook * 50.f;
			tInfo.vScale.y = -1;

			float fAngle = -360.f * AngleToDir() / 16;
			tInfo.vScale.x *= (1.f - 0.5f * fabs(sinf(D3DXToRadian(fAngle))));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CEffect::Create(tInfo.vPos, tInfo.vScale, fAngle, L"Fx", L"SwordSwipeA",
				9, 1 / 30.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255)));

			//	Add HitBox
			m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, CNormalHitBox::Create(tInfo.vPos, D3DXVECTOR3(200.f, 200.f, 0.f), this, 9 / 30.f, m_tStatus.iAttack));


		}
		else if (m_bIsEffect && 7.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 8.f)
			m_bIsEffect = false;

		if (6.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 9.f)
			m_tInfo.vPos += m_tInfo.vLook * m_fSpeed * m_pTimeMgr->GetDelta();

		break;
	case 2:
		if (!m_bIsEffect && 19.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame <= 20.f)
		{
			m_bIsEffect = true;

			D3DXVECTOR3 vPos = m_tInfo.vPos + m_tInfo.vLook * 50.f;
			D3DXVECTOR3 vScale = { 1.f, 1.f, 0.f };

			float fAngle = -360.f * AngleToDir() / 16;

			const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"SwordSwipeB", 0);


			if (-270 <= fAngle && fAngle <= -90.f)
				vScale.x *= -1.f;

			vScale.y *= (1.f + fabs(sinf(D3DXToRadian(fAngle))));
			vScale.x *= fabs(cosf(D3DXToRadian(fAngle)));

			m_pObjectMgr->AddObject(OBJECT_EFFECT, CEffect::Create(vPos, vScale, 0.f, L"Fx", L"SwordSwipeB",
				9, 1 / 30.f, ANIM_ONCE, D3DCOLOR_ARGB(255, 255, 255, 255)));

			//	Add HitBox
			m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, CNormalHitBox::Create((vPos + m_tInfo.vLook * 90.f), D3DXVECTOR3(150.f, 150.f, 0.f), this, 9 / 30.f, m_tStatus.iAttack));

		}
		else if (m_bIsEffect && 20.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 21.f)
			m_bIsEffect = false;

		if (19.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 22.f)
			m_tInfo.vPos += m_tInfo.vLook * m_fSpeed * m_pTimeMgr->GetDelta();
		break;
	case 3:
		if (!m_bIsEffect && 28.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 29.f)
		{
			m_bIsEffect = true;

			int iDir = AngleToDir();

			D3DXVECTOR3 vPos = m_tInfo.vPos + m_tInfo.vDir * 50.f;
			vPos.y += 20.f;

			CAnimImp* pImp = CAnimImp::Create(L"Fx", L"Backstab", 16);
			CNormalEffect* pEffect = CNormalEffect::Create(pImp, vPos, D3DXVECTOR3(2.f, 2.f, 0.f), RadianForAngle3(iDir), RENDER_UI);

			m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

			CNormalHitBox* pHitBox = CNormalHitBox::Create(vPos + m_tInfo.vLook * 50.f, D3DXVECTOR3(150.f, 150.f, 0.f), this, 0.2f, m_tStatus.iAttack);
			m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, pHitBox);

		}
		else if (m_bIsEffect && 29.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 30.f)
			m_bIsEffect = false;

		if (28.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 31.f)
			m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDelta();
		break;
	}
}

void CPlayer::RunUpdate()
{
	if (m_bAstarMoving && !m_pAstarMgr->AstarProceeding())
	{
		CThreadMgr::GetInstance()->WaitForAstarThread();

		if (m_pAstarMgr->GetBestPath().empty())
		{
			m_bAstarMoving = false;
			return;
		}

		D3DXVECTOR3 vBestPathFront = m_pAstarMgr->GetBestPath().front()->vPos;

		if (vBestPathFront.x < 0.f || vBestPathFront.x >= TILECX * TILEX)
		{
			m_bAstarMoving = false;
			return;
		}

		m_tInfo.vDir = m_pAstarMgr->GetBestPath().front()->vPos - (m_tInfo.vPos + m_vFootPos);
		//if (m_tInfo.vDir.x == nan)

		float fLength = D3DXVec3Length(&m_tInfo.vDir);
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
		m_tInfo.vLook = m_tInfo.vDir;

		float fMoveSpeed = m_fSpeed * m_pTimeMgr->GetDelta();
		m_tInfo.vPos += m_tInfo.vDir * fMoveSpeed;

		if (fLength <= fMoveSpeed)
			m_pAstarMgr->GetBestPath().pop_front();
	}

	m_fFootStepTimeCount += m_pTimeMgr->GetDelta();

	if (m_fFootStepTimeCount >= 0.3f)
	{
		m_fFootStepTimeCount = 0.f;
		CSoundMgr::GetInstance()->PlaySound(L"ZagFootstepsHardSurface_1.wav", CSoundMgr::EFFECT);
	}
}

void CPlayer::DashUpdate()
{
	if (m_tFrame.fCurFrame < 4.f)
		m_tInfo.vPos += m_tInfo.vDir * m_fDashSpeed * 2.f * m_pTimeMgr->GetDelta();

}

void CPlayer::DashAttackUpdate()
{
	if (m_tFrame.fCurFrame < 4.f)
		m_tInfo.vPos += m_tInfo.vDir * 400.f * m_pTimeMgr->GetDelta();
}

void CPlayer::ParryUpdate()
{
	if (m_tFrame.fCurFrame < 9.f)
	{
		m_tInfo.vPos += m_tInfo.vDir * 400.f * m_pTimeMgr->GetDelta();
	}

	if (!m_bIsEffect && 9.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 10.f)
	{
		m_bIsEffect = true;

		D3DXVECTOR3 vPos = m_tInfo.vPos + m_tInfo.vDir * 70.f;
		vPos.y += 50.f;

		CAnimImp* pImp = CAnimImp::Create(L"Fx", L"RadialNova", 44);
		CNormalEffect* pEffect = CNormalEffect::Create(pImp, vPos, D3DXVECTOR3(1.f, 0.5f, 0.f), RENDER_OBJECT);
		m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

		CNormalHitBox* pHitBox = CNormalHitBox::Create(vPos, D3DXVECTOR3(400.f, 200.f, 0.f), this, 0.2f, m_tStatus.iAttack);
		m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, pHitBox);

		CSoundMgr::GetInstance()->PlaySound(L"ZagreusSwordParry.wav", CSoundMgr::EFFECT);
	
	}
	else if (m_bIsEffect && 10.f <= m_tFrame.fCurFrame && m_tFrame.fCurFrame < 11.f)
	{
		m_bIsEffect = false;
	}
}

void CPlayer::OnHitUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_bOnceAnimFinish = false;
		m_eCurState = CPlayer::IDLE;
	}
}

void CPlayer::ChangeState()
{
	if (m_bOnceAnimFinish)	//	일회성 애니메이션이 끝났을 경우 상태변화.
	{
		switch (m_eCurState)
		{
		case CPlayer::ATTACK:
		case CPlayer::DASH_ATTACK:
			if (m_iAttackCount < 3)
				m_eCurState = CPlayer::RETURN_TO_IDLE;
			else
			{
				m_iAttackCount = 0;
				m_eCurState = CPlayer::IDLE;
			}
			m_bCanAttack = true;
			m_bOnceAnimFinish = false;
			break;
		case CPlayer::RETURN_TO_IDLE:
			m_iAttackCount = 0;
		case CPlayer::DASH:
		case CPlayer::ON_HIT:
		case CPlayer::PARRY:
			m_eCurState = CPlayer::IDLE;
			m_bOnceAnimFinish = false;
			break;
		}
	}

	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CPlayer::IDLE:
		IdleState();
		break;
	case CPlayer::RUN:
		RunState();
		break;
	case CPlayer::DASH:
		DashState();
		break;
	case CPlayer::ATTACK:
		AttackState();
		break;
	case CPlayer::DASH_ATTACK:
		DashAttackState();
		break;
	case CPlayer::PARRY:
		ParryState();
		break;
	case CPlayer::RETURN_TO_IDLE:
		ReturnToIdleState();
		break;
	case CPlayer::ON_HIT:
		OnHitState();
		break;
	case CPlayer::DEAD_WALK:
		DeadWalkState();
		break;
	}
	m_ePreState = m_eCurState;
}

void CPlayer::HitCoolDown()
{
	if (m_bCanHit)
		return;

	m_fHitCoolDown += m_pTimeMgr->GetDelta();
	if (m_fHitCoolDown >= m_fHitCoolTime)
		m_bCanHit = true;
}

void CPlayer::DeadEffect()
{
	CScreenEffect* pEffect = CScreenEffect::Create(CNonAnimImp::Create(L"BlackScreen", 10.f, D3DCOLOR_ARGB(255, 255, 255, 255)),
		D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI, 1.f);
	pEffect->SetScale(20.f);
	if (pEffect)
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

	FRAME tFrame;
	tFrame.eType = ANIM_ONCE;
	tFrame.fMaxFrame = m_pTextureMgr->GetTextureMaxFrame(L"Zagreus", L"ZagreusDeathFullscreen");
	tFrame.fCurFrame = 0.f;
	tFrame.fFrameTime = 0.f;
	tFrame.fFrameSpeed = 1 / 30.f;

	pEffect = CScreenEffect::Create(CAnimImp::Create(L"Zagreus", L"ZagreusDeathFullscreen", tFrame, D3DCOLOR_ARGB(255, 255, 255, 255)), D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), RENDER_FRONT_UI, 2.f);
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

	CMathMgr::AddTitleEffect(L"DeadImage", 4.f);

	m_tInfo.vDir = { 1.f, -0.6f, 0.f };
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_tInfo.vLook = m_tInfo.vDir;
}

int CPlayer::AngleToDir()
{
	float fAngle = CMathMgr::CalculateAngle0To2PI(D3DXVECTOR3(1.f, 0.f, 0.f), m_tInfo.vLook);
	float fInterval = PI / (m_vecDirCnt[m_eCurState] * 0.5f);

	int iDir = static_cast<int>((fAngle + PI / m_vecDirCnt[m_eCurState]) / fInterval);
	if (iDir < 0) iDir = 0;
	if (iDir > m_vecDirCnt[m_eCurState] - 1) iDir = 0;
	return iDir;
}

float CPlayer::RadianForAngle3(int iDir)
{
	switch (iDir)
	{
	case 0: return PI * 2 / 360 * (0.f);
	case 1: return PI * 2 / 360 * (-15.f);
	case 2: return PI * 2 / 360 * (-30.f);
	case 3: return PI * 2 / 360 * (-55.f);
	case 4: return PI * 2 / 360 * (-90.f);
	case 5:	return PI * 2 / 360 * (-125.f);
	case 6:	return PI * 2 / 360 * (-150.f);
	case 7:	return PI * 2 / 360 * (-165.f);
	case 8:	return PI * 2 / 360 * (-180.f);
	case 9:	return PI * 2 / 360 * (-195.f);
	case 10:return PI * 2 / 360 * (-210.f);
	case 11:return PI * 2 / 360 * (-235.f);
	case 12:return PI * 2 / 360 * (-270.f);
	case 13:return PI * 2 / 360 * (-305.f);
	case 14:return PI * 2 / 360 * (-330.f);
	case 15:return PI * 2 / 360 * (-345.f);
	}

	return 0.0f;
}

void CPlayer::DeadUpdate()
{
	if (m_bOnceAnimFinish)
	{
		m_bOnceAnimFinish = false;
		m_eCurState = CPlayer::IDLE;
		CSoundMgr::GetInstance()->PlaySound(L"ZagreusField_Wasnt Planning on it.wav", CSoundMgr::EFFECT);
	}

	m_tInfo.vPos += m_tInfo.vDir * 60.f * m_pTimeMgr->GetDelta();

	m_fFootStepTimeCount += m_pTimeMgr->GetDelta();
	if (m_fFootStepTimeCount >= 0.6f)
	{
		CSoundMgr::GetInstance()->PlaySound(L"ZagFootstepsHardSurface_1.wav", CSoundMgr::EFFECT);
		m_fFootStepTimeCount = 0.f;
	}
}

void CPlayer::WrathFill()
{
	if (m_bCalling || m_iWrathPoint >= m_iWrathMaxPoint)
		return;

	m_iWrathPoint += 1;

	m_pSubjectMgr->Notify(L"Player", CPlayerObserver::WRATH_POINT, &m_iWrathPoint);
}

void CPlayer::Calling()
{
	if (m_bCalling)
	{
		m_iWrathPoint -= m_pTimeMgr->GetDelta();
		m_pSubjectMgr->Notify(L"Player", CPlayerObserver::WRATH_POINT, &m_iWrathPoint);

		if (m_iWrathPoint % 5 == 0)
		{
			if (!m_bZeusCalled)
			{
				m_bZeusCalled = true;

				D3DXVECTOR3 vPos = { static_cast<float>(rand() % 300) - 150.f, static_cast<float>(rand() % 300) - 150.f, 0.f };
				vPos += m_tInfo.vPos + m_vFootPos;

				CAnimImp* pImp = CAnimImp::Create(L"Fx", L"LightningBolt", 16);
				CNormalEffect* pEffect = CNormalEffect::Create(pImp, vPos + D3DXVECTOR3(0.f, -187.5f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), RENDER_UI);
				m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

				pImp = CAnimImp::Create(L"Fx", L"ZeusLightningStrikeGroundNova", 10);
				pEffect = CNormalEffect::Create(pImp, vPos + m_vFootPos, D3DXVECTOR3(2.5f, 2.5f, 0.f), RENDER_OBJECT);
				m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

				CNormalHitBox* pHitBox = CNormalHitBox::Create(vPos + m_vFootPos, D3DXVECTOR3(500.f, 400.f, 0.f), pEffect, 0.2f, m_tStatus.iAttack);
				m_pObjectMgr->AddObject(OBJECT_PLAYER_HITBOX, pHitBox);

				CSoundMgr::GetInstance()->PlaySound(L"ZeusLightningImpact_1.wav", CSoundMgr::EFFECT);
			}
		}
		else
			m_bZeusCalled = false;

		if (m_iWrathPoint <= 0)
		{
			m_bCalling = false;
			m_bZeusCalled = false;
			m_iWrathPoint = 0;
		}
	}

}

void CPlayer::Bleeding(const D3DXVECTOR3& vPos)
{
	CAnimImp* pAImp = CAnimImp::Create(L"Fx", L"BloodImpactA", 17);

	D3DXVECTOR3 vDir = - vPos + m_tInfo.vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	float fDot = D3DXVec3Dot(&D3DXVECTOR3(1.f, 0.f, 0.f), &vDir);
	float fRadian = acosf(fDot);

	if (D3DXVec3Dot(&D3DXVECTOR3(0.f, 1.f, 0.f), &vDir) < 0.f)
		fRadian = -fRadian;

	CNormalEffect* pBlood = CNormalEffect::Create(pAImp, m_tInfo.vPos, D3DXVECTOR3(1.f, 1.f, 0.f), fRadian, RENDER_UI);
	m_pObjectMgr->AddObject(OBJECT_EFFECT, pBlood);
}

void CPlayer::AutoEffect()
{
	wstring wstrText = m_bAutoPlay ? L"Auto" : L"Play";

	CFontImp* pImp = CFontImp::Create(wstrText, D3DXVECTOR3(1.f, 1.f, 0.f), 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
	CMoveEffect* pEffect = CMoveEffect::Create(pImp, m_tInfo.vPos, D3DXVECTOR3(0.f, -1.f, 0.f), 100.f, RENDER_UI);
	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);
}

void CPlayer::CallEffect()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"WrathPresentationStreak", 0);
	D3DXVECTOR3 vPos = { WINCX * 0.5f, WINCY - pTexInfo->tImgInfo.Height * 0.25f, 0.f };
	D3DXVECTOR3 vScale = { static_cast<float>(WINCX) / pTexInfo->tImgInfo.Width, 1.f, 0.f };

	CAnimImp* pImp = CAnimImp::Create(L"Fx", L"WrathPresentationStreak", 31);
	CScreenEffect* pEffect = CScreenEffect::Create(pImp, vPos, vScale, RENDER_FRONT_UI, 2.f);
	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

	pTexInfo = m_pTextureMgr->GetTexInfo(L"Fx", L"WrathPresentationStreaksTop", 0);
	vPos = { WINCX * 0.5f, pTexInfo->tImgInfo.Height * 0.25f, 0.f };
	vScale = { static_cast<float>(WINCX) / pTexInfo->tImgInfo.Width, 1.f, 0.f };

	pImp = CAnimImp::Create(L"Fx", L"WrathPresentationStreaksTop", 30);
	pEffect = CScreenEffect::Create(pImp, vPos, vScale, RENDER_FRONT_UI, 2.f);
	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

	vPos = { 200.f, WINCY * 0.5f, 0.f };
	vScale = { 0.7f, 0.7f, 0.5 };
	CNonAnimImp* NonAnimImp = CNonAnimImp::Create(L"Portraits_Zeus_01", 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
	pEffect = CScreenEffect::Create(NonAnimImp, vPos, vScale, RENDER_FRONT_UI, 1.f);
	m_pObjectMgr->AddObject(OBJECT_EFFECT, pEffect);

	CSoundMgr::GetInstance()->PlaySound(L"Zeus_0069.wav", CSoundMgr::GOD_VOICE);
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::GOD_VOICE, 2.f);
}

void CPlayer::IdleState()
{
	m_tFrame.eType = ANIM_REPEAT;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::IDLE]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CPlayer::RunState()
{
	m_tFrame.eType = ANIM_REPEAT;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::RUN]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CPlayer::DashState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::DASH]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CPlayer::AttackState()
{
	m_tFrame.eType = ANIM_ONCE;

	switch (m_iAttackCount)	//	Sword 상태
	{
	case 1:
		m_tFrame.fCurFrame = 0.f;	// 0 ~ 11	8이면 다음 동작 가능.
		m_tFrame.fMaxFrame = 12.f;
		break;
	case 2:
		m_tFrame.fCurFrame = 12.f;	//	12 ~ 20	20이면 다음 동작 가능
		m_tFrame.fMaxFrame = 21.f;
		break;
	case 3:
		m_tFrame.fCurFrame = 21.f;	//	21 ~ 50
		m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::ATTACK]);

		break;
	}

	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = m_tFrame.fCurFrame * m_tFrame.fFrameSpeed;
}

void CPlayer::ReturnToIdleState()
{
	m_tFrame.eType = ANIM_ONCE;

	switch (m_iAttackCount)
	{
	case 1:
		m_tFrame.fCurFrame = 0.f;
		m_tFrame.fMaxFrame = m_vecStateMaxFrame[CPlayer::RETURN_TO_IDLE] * 0.5f;
		break;
	case 2:
		m_tFrame.fCurFrame = m_vecStateMaxFrame[CPlayer::RETURN_TO_IDLE] * 0.5f;
		m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::RETURN_TO_IDLE]);
		break;
	}

	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = m_tFrame.fCurFrame * m_tFrame.fFrameSpeed;
}

void CPlayer::DashAttackState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::DASH_ATTACK]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CPlayer::ParryState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::PARRY]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CPlayer::OnHitState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::ON_HIT]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}

void CPlayer::DeadWalkState()
{
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(m_vecStateMaxFrame[CPlayer::DEAD_WALK]);
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;
}
