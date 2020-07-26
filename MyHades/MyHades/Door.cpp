#include "stdafx.h"
#include "Door.h"
#include "Player.h"

CDoor::CDoor():
	m_bCanOpen(false),
	m_eCurState(CDoor::END),
	m_iCX(0),
	m_iCY(0)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CDoor::~CDoor()
{
	Release();
}

int CDoor::Update()
{
	StateUpdate();

	m_pObjectMgr->AddLayer(m_eRenderType, this);

	return NO_EVENT;
}

void CDoor::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
}

void CDoor::Render()
{	
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"GameObject", L"DoorOpen", static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CDoor::Initialize()
{
	m_eCurState = CDoor::CLOSE;

	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_eRenderType = RENDER_OBJECT;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 23.f;
	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fFrameSpeed = 1 / 30.f;
	m_tFrame.fFrameTime = 0.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"GameObject", L"DoorOpen", 0);

	m_iCX = pTexInfo->tImgInfo.Width;
	m_iCY = pTexInfo->tImgInfo.Height;

	return S_OK;
}

void CDoor::Release()
{
}

CDoor * CDoor::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale)
{
	CDoor* pInst = new CDoor;
	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	return pInst;
}


void CDoor::StateUpdate()
{
	switch (m_eCurState)
	{
	case CDoor::CLOSE:
		CloseUpdate();
		break;
	case CDoor::OPENING:
		OpeningUpdate();
		break;
	case CDoor::OPENED:
		OpenUpdate();
		break;
	}
}

void CDoor::CloseUpdate()
{
	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	NULL_CHECK(pPlayer);

	RECT rc = {
		static_cast<LONG>(m_tInfo.vPos.x - m_iCX * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y - m_iCY * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.x + m_iCX * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y + m_iCY * 0.5f)
	};

	POINT pt = { static_cast<LONG>(pPlayer->GetInfo().vPos.x), static_cast<LONG>(pPlayer->GetInfo().vPos.y) };

	m_bCanOpen = PtInRect(&rc, pt) ? true : false;
}

void CDoor::OpeningUpdate()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame += (m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		m_tFrame.fFrameTime = 0.f;
		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
			m_eCurState = CDoor::OPENED;
		}
	}
}

void CDoor::OpenUpdate()
{

}
