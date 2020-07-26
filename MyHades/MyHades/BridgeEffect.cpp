#include "stdafx.h"
#include "BridgeEffect.h"
#include "EffectImp.h"

CBridgeEffect::CBridgeEffect(CEffectImp* pImp):
	m_pImp(pImp),
	m_pObjectMgr(CObjectMgr::GetInstance())
{
}


CBridgeEffect::~CBridgeEffect()
{
	Release();
}

void CBridgeEffect::Release()
{
	SafeDelete(m_pImp);
}
