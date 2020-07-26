#include "stdafx.h"
#include "PlayerUI.h"
#include "PlayerObserver.h"

#define HP_BAR_X 150.f
#define HP_BAR_Y WINCY - 50.f

#define WRATH_BAR_X 150.f
#define WRATH_BAR_Y WINCY - 20.f

#define BOON_SLOT_X 50.f
#define BOON_SLOT_Y WINCY * 0.6f

CPlayerUI::CPlayerUI():
	m_pSubjectMgr(CSubjectMgr::GetInstance()),
	m_pFontMgr(CFontMgr::GetInstance()),
	m_pObserver(nullptr),
	m_wstrPlayerHp(L""),
	m_eShowState(CPlayerUI::SHOW_END),
	m_bCanReflectSkill(false),
	m_bCanDashSkill(false),
	m_bCanCall(false),
	m_fSpeed(0.f),
	m_bOnceAnimFinish(false),
	m_eWrathState(CPlayerUI::WRATH_END)
{
	ZeroMemory(&m_vBoonSlotPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vHpBarPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vWrathBarPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_tWrathFrame, sizeof(FRAME));
}


CPlayerUI::~CPlayerUI()
{
	Release();
}

int CPlayerUI::Update()
{
	switch (m_eShowState)
	{
	case CPlayerUI::COME_IN:
	{
		bool bHpBar = ComeInHpBar();
		bool bWrathBar = ComeInWrathBar();
		bool bBoonBar = ComeInBoolSlotBar();

		if (bHpBar && bWrathBar && bBoonBar)
			m_eShowState = CPlayerUI::SHOW;
	}
		m_pObjectMgr->AddLayer(RENDER_UI, this);
		break;
	case CPlayerUI::SHOW:
		m_pObjectMgr->AddLayer(RENDER_UI, this);
		break;
	case CPlayerUI::GO_OUT:
	{
		bool bHpBar = GoOutHpBar();
		bool bWrathBar = GoOutWrathBar();
		bool bBoonBar = GoOutBoolSlotBar();

		if (bHpBar && bWrathBar && bBoonBar)
			m_eShowState = CPlayerUI::NO_SHOW;
	}
		m_pObjectMgr->AddLayer(RENDER_UI, this);
		break;
	case CPlayerUI::NO_SHOW:
		break;
	}

	switch (m_eWrathState)
	{
	case CPlayerUI::NO_PIP:
		if (m_iWrathMaxPoint <= m_iWrathPoint)
		{
			m_eWrathState = CPlayerUI::FULL_START;
			m_tWrathFrame.fCurFrame = 0.f;
			m_tWrathFrame.fFrameTime = 0.f;
			m_tWrathFrame.fMaxFrame = 6.f;
			m_tWrathFrame.eType = ANIM_ONCE;
			m_tWrathFrame.fFrameSpeed = 1 / 30.f;
		}
		break;
	case CPlayerUI::FULL_START:
		Animation();
		if (m_bOnceAnimFinish)
		{
			m_bOnceAnimFinish = false;
			m_eWrathState = CPlayerUI::FULL_PIP;
			
			m_tWrathFrame.fCurFrame = 0.f;
			m_tWrathFrame.fFrameSpeed = 1 / 30.f;
			m_tWrathFrame.fFrameTime = 0.f;
			m_tWrathFrame.fMaxFrame = 20.f;
			m_tWrathFrame.eType = ANIM_REPEAT;
		}
		break;
	case CPlayerUI::FULL_PIP:
		Animation();
		if (m_iWrathPoint == 0)
			m_eWrathState = CPlayerUI::NO_PIP;
		break;
	}
	

	return NO_EVENT;
}

void CPlayerUI::LateUpdate()
{
	
}

void CPlayerUI::Render()
{
	D3DXMATRIX matScale, matRotateZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixIdentity(&matRotateZ);
	D3DXMatrixTranslation(&matTrans, m_vHpBarPos.x, m_vHpBarPos.y, 0.f);

	matWorld = matScale * matRotateZ * matTrans;

	//	HP Gage
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"HealthBar");
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	//	Hp Bar
	m_iPlayerHp = m_pObserver->GetPlayerHp();
	m_iPlayerMaxHp = m_pObserver->GetPlayerMaxHp();

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vHpBarPos.x + 7.f, m_vHpBarPos.y + 3.f, 0.f);

	matWorld = matScale * matTrans;

	pTexInfo = m_pTextureMgr->GetTexInfo(L"HealthBarFill");
	NULL_CHECK(pTexInfo);

	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	float fRatio = static_cast<float>(m_iPlayerHp) / m_iPlayerMaxHp;
	RECT rc = { 0, 0, pTexInfo->tImgInfo.Width * fRatio, pTexInfo->tImgInfo.Height };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//	Hp Text
	m_wstrPlayerHp = to_wstring(m_iPlayerHp) + L"/" + to_wstring(m_iPlayerMaxHp);
	m_pFontMgr->Render(D3DXVECTOR3(m_vHpBarPos.x + 150.f,m_vHpBarPos.y - 10.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), m_wstrPlayerHp, D3DCOLOR_ARGB(255, 255, 255, 255));


	//	Wrath Bar
	
	switch (m_eWrathState)
	{
	case CPlayerUI::NO_PIP:
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		pTexInfo = m_pTextureMgr->GetTexInfo(L"WrathBar");
		break;
	case CPlayerUI::FULL_START:
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		pTexInfo = m_pTextureMgr->GetTexInfo(L"WrathPipFull", L"WrathBarFullFxStart", static_cast<WORD>(m_tWrathFrame.fCurFrame));
		break;
	case CPlayerUI::FULL_PIP:
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		pTexInfo = m_pTextureMgr->GetTexInfo(L"WrathPipFull", L"WrathBarFullFx", static_cast<WORD>(m_tWrathFrame.fCurFrame));
		break;
	}
	D3DXMatrixTranslation(&matTrans, m_vWrathBarPos.x, m_vWrathBarPos.y, 0.f);
	matWorld = matScale * matTrans;

	NULL_CHECK(pTexInfo);

	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//	Pip Empty
	for (int i = 0; i < 4; ++i)
	{
		D3DXMatrixScaling(&matScale, 0.6f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vWrathBarPos.x - 72.f + i * 50.f, m_vWrathBarPos.y - 2.f, 0.f);

		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"WrathPipEmpty");
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_iWrathMaxPoint = m_pObserver->GetWrathMaxPoint();
	m_iWrathPoint = m_pObserver->GetWrathPoint();

	int iWrathPoint = m_iWrathPoint;

	for (int i = 0; i < m_iWrathPoint / 25 + 1; ++i)
	{


		switch (m_eWrathState)
		{
		case CPlayerUI::NO_PIP:
		case CPlayerUI::FULL_START:
			D3DXMatrixScaling(&matScale, 0.6f, 0.5f, 0.f);
			pTexInfo = m_pTextureMgr->GetTexInfo(L"WrathPipPartial");
			break;
		case CPlayerUI::FULL_PIP:
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			pTexInfo = m_pTextureMgr->GetTexInfo(L"WrathPipFull", L"WrathPipFull", static_cast<WORD>(m_tWrathFrame.fCurFrame));
			break;
		}
		D3DXMatrixTranslation(&matTrans, m_vWrathBarPos.x - 72.f + i * 50.f, m_vWrathBarPos.y - 2.f, 0.f);
		matWorld = matScale * matTrans;

		NULL_CHECK(pTexInfo);

		fRatio = iWrathPoint / 25.f;
		if (fRatio >= 1.f)
			fRatio = 1.f;
		iWrathPoint -= 25;

		rc = { 0, 0, static_cast<LONG>(pTexInfo->tImgInfo.Width * fRatio), static_cast<LONG>(pTexInfo->tImgInfo.Height) };

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//	BoonSlot

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vBoonSlotPos.x, m_vBoonSlotPos.y, 0.f);
	matWorld = matScale * matTrans;

	pTexInfo = m_pTextureMgr->GetTexInfo(L"BoonEmptySlot");
	NULL_CHECK(pTexInfo);

	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_pObserver->GetCanReflectSkill())
	{
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vBoonSlotPos.x, m_vBoonSlotPos.y - 202.f, 0.f);
		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"Zeus_01_Large");
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (m_pObserver->GetCanDashSkill())
	{
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vBoonSlotPos.x, m_vBoonSlotPos.y - 102.f, 0.f);
		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"Zeus_06_Large");
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (m_pObserver->GetCanCall())
	{
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vBoonSlotPos.x, m_vBoonSlotPos.y - 2.f, 0.f);
		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"Zeus_07_Large");
		NULL_CHECK(pTexInfo);

		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

HRESULT CPlayerUI::Initialize()
{
	m_pObserver = CPlayerObserver::Create();
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	m_pSubjectMgr->Subscribe(L"Player", m_pObserver);

	m_vHpBarPos		= { HP_BAR_X, HP_BAR_Y + 100.f, 0.f };
	m_vWrathBarPos	= { WRATH_BAR_X, WRATH_BAR_Y + 100.f, 0.f };
	m_vBoonSlotPos	= { BOON_SLOT_X - 100.f, BOON_SLOT_Y, 0.f };

	m_fSpeed = 200.f;

	m_iWrathMaxPoint = 100;
	m_iWrathPoint = 0;

	m_eShowState = CPlayerUI::NO_SHOW;
	m_eWrathState = CPlayerUI::NO_PIP;

	m_tWrathFrame.fCurFrame = 0.f;
	m_tWrathFrame.fFrameTime = 0.f;
	m_tWrathFrame.fMaxFrame = 6.f;
	m_tWrathFrame.eType = ANIM_ONCE;
	m_tWrathFrame.fFrameSpeed = 1 / 30.f;

	return S_OK;
}

void CPlayerUI::Release()
{
	m_pSubjectMgr->Unsubscribe(L"Player", m_pObserver);
	SafeDelete(m_pObserver);
}

CPlayerUI * CPlayerUI::Create()
{
	CPlayerUI* pInst = new CPlayerUI;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	return pInst;
}

void CPlayerUI::UIComeIn()
{
	switch (m_eShowState)
	{
	case CPlayerUI::COME_IN:
	case CPlayerUI::SHOW:
		return;
	case CPlayerUI::GO_OUT:
	case CPlayerUI::NO_SHOW:
		m_eShowState = CPlayerUI::COME_IN;
		break;
	}
}

void CPlayerUI::UIGoOut()
{
	switch (m_eShowState)
	{
	case CPlayerUI::COME_IN:
	case CPlayerUI::SHOW:
		m_eShowState = CPlayerUI::GO_OUT;
		return;
	case CPlayerUI::GO_OUT:
	case CPlayerUI::NO_SHOW:
		return;
	}
}

bool CPlayerUI::ComeInHpBar()
{
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };

	vDir.x = HP_BAR_X - m_vHpBarPos.x;
	vDir.y = HP_BAR_Y - m_vHpBarPos.y;

	bool bX, bY;

	if (fabsf(vDir.x) < 10.f)
	{
		bX = true;
		m_vHpBarPos.x = HP_BAR_X;
	}
	else
		bX = false;

	if (fabsf(vDir.y) < 10.f)
	{
		bY = true;
		m_vHpBarPos.y = HP_BAR_Y;
	}
	else
		bY = false;

	if (bX && bY)
		return true;

	if (D3DXVec3Length(&vDir) > 1.f)
		D3DXVec3Normalize(&vDir, &vDir);

	m_vHpBarPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;	

	return false;
}

bool CPlayerUI::ComeInWrathBar()
{
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };

	vDir.x = WRATH_BAR_X - m_vWrathBarPos.x;
	vDir.y = WRATH_BAR_Y - m_vWrathBarPos.y;

	bool bX, bY;

	if (fabsf(vDir.x) < 10.f)
	{
		bX = true;
		m_vWrathBarPos.x = WRATH_BAR_X;
	}
	else
		bX = false;

	if (fabsf(vDir.y) < 10.f)
	{
		bY = true;
		m_vWrathBarPos.y = WRATH_BAR_Y;
	}
	else
		bY = false;

	if (bX && bY)
		return true;

	if (D3DXVec3Length(&vDir) > 1.f)
		D3DXVec3Normalize(&vDir, &vDir);

	m_vWrathBarPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;

	return false;
}

bool CPlayerUI::ComeInBoolSlotBar()
{
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };

	vDir.x = BOON_SLOT_X - m_vBoonSlotPos.x;
	vDir.y = BOON_SLOT_Y - m_vBoonSlotPos.y;

	bool bX, bY;

	if (fabsf(vDir.x) < 10.f)
	{
		bX = true;
		m_vBoonSlotPos.x = BOON_SLOT_X;
	}
	else
		bX = false;

	if (fabsf(vDir.y) < 10.f)
	{
		bY = true;
		m_vBoonSlotPos.y = BOON_SLOT_Y;
	}
	else
		bY = false;

	if (bX && bY)
		return true;

	if (D3DXVec3Length(&vDir) > 1.f)
		D3DXVec3Normalize(&vDir, &vDir);

	m_vBoonSlotPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;

	return false;
}

bool CPlayerUI::GoOutHpBar()
{
	if (m_vHpBarPos.y > HP_BAR_Y + 200.f)
		return true;
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
	vDir.y = HP_BAR_Y + 200.f - m_vHpBarPos.y;
	D3DXVec3Normalize(&vDir, &vDir);

	m_vHpBarPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;
	return false;
}

bool CPlayerUI::GoOutWrathBar()
{
	if (m_vWrathBarPos.y > WRATH_BAR_Y + 200.f)
		return true;

	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
	vDir.y = WRATH_BAR_Y + 200.f - m_vWrathBarPos.y;
	D3DXVec3Normalize(&vDir, &vDir);

	m_vWrathBarPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;
	return false;
}

bool CPlayerUI::GoOutBoolSlotBar()
{
	if (m_vBoonSlotPos.x < BOON_SLOT_X - 200.f)
		return true;

	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
	vDir.x = BOON_SLOT_X - 200.f - m_vBoonSlotPos.x;
	D3DXVec3Normalize(&vDir, &vDir);

	m_vBoonSlotPos += vDir * m_pTimeMgr->GetDelta() * m_fSpeed;
	return false;
}

void CPlayerUI::Animation()
{
	m_tWrathFrame.fFrameTime += m_pTimeMgr->GetDelta();

	m_tWrathFrame.fCurFrame = static_cast<float>(m_tWrathFrame.fFrameTime / m_tWrathFrame.fFrameSpeed);
	//m_tFrame.fFrameTime = 0.f;
	switch (m_tWrathFrame.eType)
	{
	case ANIM_ONCE:
		if (m_tWrathFrame.fCurFrame >= m_tWrathFrame.fMaxFrame)
		{
			m_tWrathFrame.fCurFrame = m_tWrathFrame.fMaxFrame - 1;
			m_bOnceAnimFinish = true;
		}
		break;
	case ANIM_REPEAT:
		if (m_tWrathFrame.fCurFrame >= m_tWrathFrame.fMaxFrame)
		{
			m_tWrathFrame.fCurFrame = 0.f;
			m_tWrathFrame.fFrameTime = 0.f;
		}
		break;
	}
}
