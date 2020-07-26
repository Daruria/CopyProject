#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect():
	m_bIsDead(false),
	m_wstrObjectKey(L""),
	m_wstrStateKey(L""),
	m_d3dColor(D3DCOLOR_ARGB(255, 255, 255, 255)),
	m_fTimeCount(0.f),
	m_fLifeTime(0.f),
	m_pObjectMgr(CObjectMgr::GetInstance())
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}


CEffect::~CEffect()
{
	Release();
}

int CEffect::Update()
{
	if (ANIM_REPEAT == m_tFrame.eType)
	{
		m_fTimeCount += m_pTimeMgr->GetDelta();
		if (m_fTimeCount >= m_fLifeTime)
			m_bIsDead = true;
	}

	if (m_bIsDead)
		return DEAD_OBJ;

	Animation();

	m_pObjectMgr->AddLayer(m_eRenderType, this);

	return NO_EVENT;
}

void CEffect::LateUpdate()
{
	D3DXMATRIX matScale, matRotateZ, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);

	float fAngle = m_fAngle == 0.f ? CMathMgr::CalculateAngle0To2PI(m_tInfo.vLook, D3DXVECTOR3(1.f, 0.f, 0.f)) : D3DXToRadian(m_fAngle);

	D3DXMatrixRotationZ(&matRotateZ, fAngle);
	MatTranslation(&matTrans, (m_tInfo.vPos - CScrollMgr::GetScrollPos()));
	
	m_tInfo.matWorld = matScale * matRotateZ * matTrans;
}

void CEffect::Render()
{
	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));
	NULL_CHECK_MSG(pTexInfo, L"Effect pTexInfo is nullptr");

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, m_d3dColor);
}

HRESULT CEffect::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameTime = 0.f;
	m_tFrame.eType = ANIM_ONCE;

	m_d3dColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_eRenderType = RENDER_FRONT_OBJECT;

	return S_OK;
}

void CEffect::Release()
{
}

CEffect * CEffect::Create(const INFO& tInfo, const wstring & wstrObjectKey, const wstring& wstrStateKey, const WORD & wMaxFrame, float fFrameSpeed, ANIM_TYPE eAnimationType, D3DCOLOR d3dColor)
{
	CEffect* pInstance = new CEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo = tInfo;
	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;
	
	pInstance->m_tFrame.fMaxFrame = static_cast<float>(wMaxFrame);
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_tFrame.eType = eAnimationType;
	pInstance->m_d3dColor = d3dColor;

	return pInstance;
}

CEffect * CEffect::Create(const INFO & tInfo, const wstring & wstrObjectKey, const wstring & wstrStateKey, const WORD & wMaxFrame, float fFrameSpeed, float fLifeTime, ANIM_TYPE eAnimationType, D3DCOLOR d3dColor)
{
	CEffect* pInstance = new CEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo = tInfo;
	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;

	pInstance->m_tFrame.fMaxFrame = static_cast<float>(wMaxFrame);
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_tFrame.eType = eAnimationType;
	pInstance->m_d3dColor = d3dColor;
	pInstance->m_fLifeTime = fLifeTime;


	return pInstance;
}

CEffect * CEffect::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, float fAngle, const wstring & wstrObjectKey, const wstring & wstrStateKey, const WORD & wMaxFrame, float fFrameSpeed, ANIM_TYPE eAnimationType, D3DCOLOR d3dColor)
{
	CEffect* pInstance = new CEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vScale = vScale;
	pInstance->m_fAngle = fAngle;

	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;

	pInstance->m_tFrame.fMaxFrame = static_cast<float>(wMaxFrame);
	pInstance->m_tFrame.fFrameSpeed = fFrameSpeed;
	pInstance->m_tFrame.eType = eAnimationType;

	pInstance->m_d3dColor = d3dColor;

	return pInstance;
}

void CEffect::Animation()
{
	//m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	//if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	//{
	//	m_tFrame.fCurFrame += (m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
	//	m_tFrame.fFrameTime = 0.f;
	//	switch (m_tFrame.eType)
	//	{
	//	case ANIM_ONCE:
	//		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	//		{
	//			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1.f;
	//			m_bIsDead = true;
	//		}
	//		break;
	//	case ANIM_REPEAT:
	//		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
	//			m_tFrame.fCurFrame = 0.f;
	//		break;
	//	}
	//}

	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();

	m_tFrame.fCurFrame = static_cast<float>(m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
	//m_tFrame.fFrameTime = 0.f;
	switch (m_tFrame.eType)
	{
	case ANIM_ONCE:
		if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 1;
			m_bIsDead = true;
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
