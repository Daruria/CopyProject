#include "stdafx.h"
#include "BoonOrb.h"
#include "AnimatedObject.h"
#include "Player.h"
#include "Boon.h"

CBoonOrb::CBoonOrb():
	m_pBoonFlare(nullptr),
	m_pOrbBack(nullptr),
	m_pOrbFront(nullptr),
	m_pKeyMgr(CKeyMgr::GetInstance())
{
}


CBoonOrb::~CBoonOrb()
{
	Release();
}

int CBoonOrb::Update()
{
	m_pOrbBack->Update();
	m_pOrbFront->Update();
	m_pBoonFlare->Update();

	CPlayer* pPlayer = m_pObjectMgr->GetPlayer();
	D3DXVECTOR3 vDist = m_tInfo.vPos - pPlayer->GetInfo().vPos;

	float fDist = D3DXVec3Length(&vDist);

	if (fDist < 200.f)
	{
		if (m_pKeyMgr->KeyDown(KEY_E))
		{
			CBoon* pBoon = CBoon::Create(D3DXVECTOR3(WINCX * 0.5f, WINCY * 0.5f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), 0.f);
			m_pObjectMgr->AddObject(OBJECT_FRONT_UI, pBoon);
			pPlayer->SetCanKeyInput(false);
			return DEAD_OBJ;
		}
	}

	m_pObjectMgr->AddLayer(m_eRenderType, this);

	return NO_EVENT;
}

void CBoonOrb::LateUpdate()
{
	m_pOrbBack->LateUpdate();
	m_pOrbFront->LateUpdate();
	m_pBoonFlare->LateUpdate();

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CBoonOrb::Render()
{
	m_pOrbBack->Render();

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"ZeusSymbol");
	NULL_CHECK(pTexInfo);
	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pOrbFront->Render();
	m_pBoonFlare->Render();
	

}

HRESULT CBoonOrb::Initialize()
{
	m_eRenderType = RENDER_OBJECT;

	FRAME tFrame;
	tFrame.fCurFrame = 0.f;
	tFrame.fFrameSpeed = 1 / 30.f;
	tFrame.fFrameTime = 0.f;
	tFrame.eType = ANIM_REPEAT;

	tFrame.fMaxFrame = 20.f;
	m_pBoonFlare = CAnimatedObject::Create(L"Fx", L"BoonFlare", 20, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f));
	m_pOrbBack = CAnimatedObject::Create(L"Fx", L"BoonOrbBack", 30, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f));
	m_pOrbFront = CAnimatedObject::Create(L"Fx", L"BoonOrbFront", 30, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f));

	return S_OK;
}

void CBoonOrb::Release()
{
	SafeDelete(m_pBoonFlare);
	SafeDelete(m_pOrbBack);
	SafeDelete(m_pOrbFront);
}

CBoonOrb* CBoonOrb::Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale)
{
	CBoonOrb* pInst = new CBoonOrb;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_pBoonFlare->SetPos(vPos);
	pInst->m_pOrbBack->SetPos(vPos);
	pInst->m_pOrbFront->SetPos(vPos);

	return pInst;
}
