#include "stdafx.h"
#include "BoonButton.h"
#include "AnimImp.h"
#include "ScreenEffect.h"

CBoonButton::CBoonButton() :
	m_bIsOn(false),
	m_wstrStateKey(L""),
	m_ePreState(CBoonButton::END),
	m_eCurState(CBoonButton::END),
	m_pKeyMgr(CKeyMgr::GetInstance())
{
	ZeroMemory(&m_vSize, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_tOnFrame, sizeof(FRAME));
}


CBoonButton::~CBoonButton()
{
	Release();
}

int CBoonButton::Update()
{
	IsOnMouse();
	OnUpdate();
	ChangeState();

	if (m_bIsOn && m_pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		CAnimImp* pImp = CAnimImp::Create(L"Boon", L"BoonGet", 15, 1/30.f, D3DCOLOR_ARGB(255, 255, 255, 0));
		CScreenEffect* pEffect = CScreenEffect::Create(pImp, m_tInfo.vPos, RENDER_FRONT_UI, 5.f);
		m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pEffect);

		return 1;
	}

	return NO_EVENT;
}

void CBoonButton::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CBoonButton::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrStateKey);
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bIsOn)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Boon", L"BoonHighlightButton", static_cast<WORD>(m_tOnFrame.fCurFrame));
		NULL_CHECK(pTexInfo);

		vCenter = {pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f};
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

HRESULT CBoonButton::Initialize()
{
	m_eCurState = CBoonButton::OFF;

	m_tOnFrame.fCurFrame = 0.f;
	m_tOnFrame.fMaxFrame = 4.f;
	m_tOnFrame.eType = ANIM_ONCE;
	m_tOnFrame.fFrameSpeed = 1 / 30.f;
	m_tOnFrame.fFrameTime = 0.f;

	return S_OK;
}

void CBoonButton::Release()
{
}

CBoonButton * CBoonButton::Create(const wstring & wstrStateKey, const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, const D3DXVECTOR3 & vSize)
{
	CBoonButton* pInst = new CBoonButton;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_vSize = vSize;

	return pInst;
}

void CBoonButton::IsOnMouse()
{
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc = {
		static_cast<LONG>(m_tInfo.vPos.x - m_vSize.x * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y - m_vSize.y * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.x + m_vSize.x * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y + m_vSize.y * 0.5f)
	};

	m_bIsOn = PtInRect(&rc, pt);
}

void CBoonButton::OnUpdate()
{
	if (m_bIsOn)
	{
		m_tOnFrame.fFrameTime += m_pTimeMgr->GetDelta();
		if (m_tOnFrame.fFrameTime > m_tOnFrame.fFrameSpeed)
		{
			m_tOnFrame.fCurFrame += static_cast<DWORD>(m_tOnFrame.fFrameTime / m_tOnFrame.fFrameSpeed);
			m_tOnFrame.fFrameTime = 0.f;

			if (m_tOnFrame.fCurFrame >= m_tOnFrame.fMaxFrame)
				m_tOnFrame.fCurFrame = m_tOnFrame.fMaxFrame - 1;
		}


	}
	else
		m_eCurState = CBoonButton::OFF;
	
}

void CBoonButton::ChangeState()
{
	if (m_ePreState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case CBoonButton::ON_START:
		m_tOnFrame.fCurFrame = 0.f;
		m_tOnFrame.fMaxFrame = 4.f;
		m_tOnFrame.fFrameSpeed = 1 / 30.f;
		m_tOnFrame.eType = ANIM_ONCE;
		m_tOnFrame.fFrameTime = 0.f;
		break;
	case CBoonButton::ON:

		break;
	case CBoonButton::OFF:
		break;
	}
	m_ePreState = m_eCurState;
}
