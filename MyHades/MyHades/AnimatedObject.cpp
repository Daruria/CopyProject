#include "stdafx.h"
#include "AnimatedObject.h"
#include "Texture.h"
#include "MultiTexture.h"


CAnimatedObject::CAnimatedObject():
	m_bOnceFinish(false)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CAnimatedObject::~CAnimatedObject()
{
	Release();
}

int CAnimatedObject::Update()
{
	Animation();

	if (m_bIsDead)
		return DEAD_OBJ;

	if (m_bRenderListPush)
		CObjectMgr::GetInstance()->AddLayer(m_eRenderType, this);

	return 0;
}

void CAnimatedObject::LateUpdate()
{
	//D3DXMATRIX matScale, matTrans;
	//D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	//D3DXVECTOR3 vScrolledPos = m_tInfo.vPos - CScrollMgr::GetScrollPos();
	//MatTranslation(&matTrans, vScrolledPos);
	////D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	//m_tInfo.matWorld = matScale * matTrans;

	D3DXMATRIX matScale, matTrans, matRotate;
	MatScaling(&matScale, m_tInfo.vScale);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x,
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		0.f);
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(m_fDegree));

	m_tInfo.matWorld = matScale * matRotate * matTrans;
}

void CAnimatedObject::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CAnimatedObject::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tInfo.vScale = {1.f, 1.f, 0.f};

	m_tFrame.eType = ANIM_ONCE;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fFrameSpeed = 0.f;
	m_tFrame.fFrameTime = 0.f;

	return S_OK;
}

void CAnimatedObject::Release()
{
}

CAnimatedObject * CAnimatedObject::Create(const wstring & wstrObjectKey, const wstring & wstrStateKey, const WORD iCnt, const D3DXVECTOR3 & vPos,
	const D3DXVECTOR3 & vScale /*= D3DXVECTOR3(1.f, 1.f, 0.f)*/, bool bRenderListPush/* = false*/, float fDegree/* = 0.f*/, float fSort/* = 1.f*/,
	RENDER_TYPE eType /*= RENDER_OBJECT*/)
{
	CAnimatedObject* pInstance = new CAnimatedObject;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vScale = vScale;
	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;
	pInstance->m_eRenderType = eType;
	pInstance->m_eImgObjType = CImageObject::ANIM_OBJ;
	pInstance->m_fDegree = fDegree;
	pInstance->m_bRenderListPush = bRenderListPush;

	pInstance->m_tFrame.eType = ANIM_REPEAT;
	pInstance->m_tFrame.fCurFrame = 0.f;
	pInstance->m_tFrame.fMaxFrame = static_cast<float>(pInstance->m_pTextureMgr->GetTextureMaxFrame(wstrObjectKey, wstrStateKey));
	pInstance->m_tFrame.fFrameSpeed = 1 / 30.f;
	pInstance->m_tFrame.fFrameTime = 0.f;

	return pInstance;
}

CAnimatedObject * CAnimatedObject::Create(const wstring & wstrObjectKey, float fx, float fy, float fScale, RENDER_TYPE eType/* = RENDER_OBJECT*/)
{
	CAnimatedObject* pInstance = new CAnimatedObject;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = { fx, fy, 0.f };
	pInstance->m_tInfo.vScale = {fScale, fScale, 0.f};
	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_eRenderType = eType;

	return pInstance;
}

void CAnimatedObject::SetState(const wstring & wstrStateKey, ANIM_TYPE eType, float fFrameSpeed)
{
	if (m_wstrStateKey == wstrStateKey)
		return;

	CTexture* pTexture = m_pTextureMgr->GetTexture(m_wstrObjectKey);
	NULL_CHECK(pTexture);

	size_t iStateMaxFrame = dynamic_cast<CMultiTexture*>(pTexture)->GetStateSize(wstrStateKey);

	if (0 == iStateMaxFrame)
		return;

	m_wstrStateKey = wstrStateKey;
	m_tFrame.eType = eType;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = static_cast<float>(iStateMaxFrame);
	m_tFrame.fFrameSpeed = fFrameSpeed;
	m_tFrame.fFrameTime = 0.f;

	m_bOnceFinish = false;
}

const TEX_INFO * CAnimatedObject::GetTexInfo() const
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, static_cast<WORD>(m_tFrame.fCurFrame));

	NULL_CHECK_RETURN(pTexInfo, nullptr);

	return pTexInfo;
}

bool CAnimatedObject::Animation()
{
	m_tFrame.fFrameTime += m_pTimeMgr->GetDelta();
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fCurFrame += (m_tFrame.fFrameTime / m_tFrame.fFrameSpeed);
		m_tFrame.fFrameTime = 0.f;
		switch (m_tFrame.eType)
		{
		case ANIM_ONCE:
			if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
			{
				m_tFrame.fCurFrame = m_tFrame.fMaxFrame - 0.1f;
				m_bOnceFinish = true;
				return true;
			}
			break;
		case ANIM_REPEAT:
			if (m_tFrame.fCurFrame >= m_tFrame.fMaxFrame)
				m_tFrame.fCurFrame = 0.f;
			break;
		}
	}
	return false;
}
