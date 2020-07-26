#include "stdafx.h"
#include "RotatedHitBox.h"


CRotatedHitBox::CRotatedHitBox()
{
}


CRotatedHitBox::~CRotatedHitBox()
{
}

int CRotatedHitBox::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDelta();
	if (m_fTimeCount >= m_fLifeTime)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CRotatedHitBox::LateUpdate()
{
}

void CRotatedHitBox::Render()
{
#ifdef _DEBUG



#endif // _DEBUG

}

HRESULT CRotatedHitBox::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };



	return S_OK;
}

void CRotatedHitBox::Release()
{
}

CRotatedHitBox * CRotatedHitBox::Create(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vSize, CGameObject * pParent, float fLifeTime)
{
	CRotatedHitBox* pInst = new CRotatedHitBox;
	if (FAILED(pInst->Initialize()))
	{
		SafeDelete(pInst);
		return nullptr;
	}

	pInst->m_tInfo.vPos = vPos;
	pInst->m_vSize = vSize;
	pInst->m_pParent = pParent;
	pInst->m_fLifeTime = fLifeTime;


	return pInst;
}
