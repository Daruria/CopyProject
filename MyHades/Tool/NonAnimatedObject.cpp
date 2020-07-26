#include "stdafx.h"
#include "NonAnimatedObject.h"
#include "ToolView.h"

CNonAnimatedObject::CNonAnimatedObject():
	m_iDrawID(0)
{
}

CNonAnimatedObject::~CNonAnimatedObject()
{
	Release();
}

int CNonAnimatedObject::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	if (m_bRenderListPush)
		CObjectMgr::GetInstance()->AddLayer(m_eRenderType, this);

	return NO_EVENT;
}

void CNonAnimatedObject::LateUpdate()
{
	D3DXMATRIX matScale, matTrans, matRotate;
	MatScaling(&matScale, m_tInfo.vScale);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - m_pView->GetScrollPos(0),
		m_tInfo.vPos.y - m_pView->GetScrollPos(1),
		0.f);
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(m_fDegree));

	m_tInfo.matWorld = matScale * matRotate * matTrans;
}

void CNonAnimatedObject::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, m_iDrawID);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CNonAnimatedObject::Initialize()
{
	m_tInfo.vScale	= { 1.f, 1.f, 0.f };
	m_tInfo.vDir	= { 1.f, 0.f, 0.f };
	m_tInfo.vLook	= { 1.f, 0.f, 0.f };

	return S_OK;
}

void CNonAnimatedObject::Release()
{
}

CNonAnimatedObject * CNonAnimatedObject::Create(const wstring & wstrObjectKey, const wstring& wstrStateKey, const D3DXVECTOR3 & vPos, 
	const D3DXVECTOR3 & vScale, bool bRenderListPush, float fDegree/* = 0.f*/, float fSort /*= 1.f*/, int iDrawID/*= 0*/, RENDER_TYPE eRenderType /*= RENDER_OBJECT*/, CToolView* pView /*= nullptr*/)
{
	CNonAnimatedObject* pInst = new CNonAnimatedObject;

	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}
	pInst->m_wstrObjectKey = wstrObjectKey;
	pInst->m_wstrStateKey = wstrStateKey;
	pInst->m_tInfo.vPos = vPos;
	pInst->m_tInfo.vScale = vScale;
	pInst->m_iDrawID = iDrawID;
	pInst->m_pView = pView;
	pInst->m_eImgObjType = CImageObject::NON_ANIM_OBJ;
	pInst->m_fDegree = fDegree;
	pInst->m_bRenderListPush = bRenderListPush;
	pInst->m_eRenderType = eRenderType;
	pInst->m_fSort = fSort;

	return pInst;
}

const TEX_INFO * CNonAnimatedObject::GetTexInfo() const
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, m_iDrawID);

	NULL_CHECK_RETURN(pTexInfo, nullptr);

	return pTexInfo;
}
